#include "document_converter.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// Check a small fixed prefix without allocating a temporary string
bool startsWith(std::string_view value, std::string_view prefix) {
  return value.size() >= prefix.size() &&
         value.substr(0, prefix.size()) == prefix;
}

// Remove trailing whitespace kept by getline on CRLF files
std::string trimRight(std::string value) {
  while (!value.empty() && (value.back() == ' ' || value.back() == '\t' ||
                            value.back() == '\r')) {
    value.pop_back();
  }

  return value;
}

// Remove outer whitespace from one Markdown field
std::string trim(std::string_view value) {
  const auto first = value.find_first_not_of(" \t\r");
  if (first == std::string_view::npos) {
    return {};
  }

  const auto last = value.find_last_not_of(" \t\r");
  return std::string(value.substr(first, last - first + 1));
}

// Escape characters that have special meaning in Typst markup text
std::string escapeTypstText(std::string_view value) {
  std::string result;
  result.reserve(value.size());

  for (const char character : value) {
    switch (character) {
    case '\\':
    case '#':
    case '[':
    case ']':
    case '{':
    case '}':
      result.push_back('\\');
      break;
    default:
      break;
    }

    result.push_back(character);
  }

  return result;
}

// Escape a value that will be written inside a Typst string literal
std::string escapeTypstString(std::string_view value) {
  std::string result;
  result.reserve(value.size());

  for (const char character : value) {
    if (character == '\\' || character == '"') {
      result.push_back('\\');
    }

    result.push_back(character);
  }

  return result;
}

// Replace ASCII arrows in regular prose
std::string replaceTextArrows(std::string_view value) {
  std::string result;
  result.reserve(value.size());

  for (auto index = 0U; index < value.size(); index++) {
    if (index + 1 < value.size() && value[index] == '-' &&
        value[index + 1] == '>') {
      result += "→";
      index++;
    } else if (index + 1 < value.size() && value[index] == '<' &&
               value[index + 1] == '-') {
      result += "←";
      index++;
    } else {
      result.push_back(value[index]);
    }
  }

  return result;
}

// Escape text around inline code spans
std::string escapeInlineMarkdown(std::string_view line) {
  std::string result;
  bool inInlineCode = false;
  std::string textPart;

  auto flushTextPart = [&]() {
    result += escapeTypstText(replaceTextArrows(textPart));
    textPart.clear();
  };

  for (const char character : line) {
    if (character == '`') {
      if (!inInlineCode) {
        flushTextPart();
      }

      result.push_back(character);
      inInlineCode = !inInlineCode;
      continue;
    }

    if (inInlineCode) {
      result.push_back(character);
    } else {
      textPart.push_back(character);
    }
  }

  flushTextPart();
  return result;
}

// Read Markdown as trimmed lines so parsing is stable across platforms
std::vector<std::string> readLines(const std::filesystem::path &path) {
  std::ifstream input(path);
  if (!input) {
    throw std::runtime_error("failed to open input file: " + path.string());
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(input, line)) {
    lines.push_back(trimRight(line));
  }

  return lines;
}

// Use the first H1 as the lab title
std::string detectTitle(const std::vector<std::string> &lines,
                        const std::filesystem::path &markdownPath) {
  for (const auto &line : lines) {
    if (startsWith(line, "# ")) {
      return trim(std::string_view(line).substr(2));
    }
  }

  return markdownPath.parent_path().filename().string();
}

// Drop the old text prefix because the template already shows the lab number
std::string makeDisplayTitle(std::string title) {
  const std::string prefix = "Лабораторная работа ";
  if (!startsWith(title, prefix)) {
    return title;
  }

  auto index = prefix.size();
  while (index < title.size() &&
         std::isdigit(static_cast<unsigned char>(title[index])) != 0) {
    index++;
  }

  if (index < title.size() && title[index] == '.') {
    index++;
  }

  while (index < title.size() && title[index] == ' ') {
    index++;
  }

  return title.substr(index);
}

// Extract the two-digit lab number from the folder name
std::string detectLabNumber(const std::filesystem::path &markdownPath) {
  const auto labName = markdownPath.parent_path().filename().string();
  if (labName.size() >= 2 &&
      std::isdigit(static_cast<unsigned char>(labName[0])) != 0 &&
      std::isdigit(static_cast<unsigned char>(labName[1])) != 0) {
    return labName.substr(0, 2);
  }

  return "--";
}

// Create a short stable id for the original README.md content
std::string shortReadmeHash(const std::vector<std::string> &lines) {
  auto hash = 14695981039346656037ULL;

  // FNV-1a is enough here: the footer needs a compact content identity, not
  // cryptographic integrity
  for (const auto &line : lines) {
    for (const auto character : line) {
      hash ^= static_cast<unsigned char>(character);
      hash *= 1099511628211ULL;
    }

    hash ^= static_cast<unsigned char>('\n');
    hash *= 1099511628211ULL;
  }

  std::ostringstream stream;
  stream << std::hex << std::setfill('0') << std::setw(16) << hash;
  return stream.str().substr(0, 8);
}

// Make the generated Typst file import the shared template by relative path
std::filesystem::path
makeRelativeImportPath(const std::filesystem::path &fromDirectory,
                       const std::filesystem::path &targetPath) {
  const auto relative = std::filesystem::relative(targetPath, fromDirectory);
  return relative.empty() ? targetPath : relative;
}

// Split one Markdown table row into trimmed cell values
std::vector<std::string> splitTableRow(std::string_view line) {
  std::vector<std::string> cells;

  if (!line.empty() && line.front() == '|') {
    line.remove_prefix(1);
  }

  if (!line.empty() && line.back() == '|') {
    line.remove_suffix(1);
  }

  auto cellStart = 0U;
  for (auto index = 0U; index <= line.size(); index++) {
    if (index == line.size() || line[index] == '|') {
      cells.push_back(trim(line.substr(cellStart, index - cellStart)));
      cellStart = index + 1;
    }
  }

  return cells;
}

// Detect cells like --- or :---: from the Markdown separator row
bool isTableSeparatorCell(std::string_view cell) {
  const auto trimmedCell = trim(cell);
  if (trimmedCell.empty()) {
    return false;
  }

  for (const auto character : trimmedCell) {
    if (character != '-' && character != ':') {
      return false;
    }
  }

  return trimmedCell.find('-') != std::string::npos;
}

// Detect the whole Markdown table separator row
bool isTableSeparator(std::string_view line) {
  const auto cells = splitTableRow(line);
  return !cells.empty() && std::ranges::all_of(cells, isTableSeparatorCell);
}

// Check whether the current line starts a Markdown table
bool isTableStart(const std::vector<std::string> &lines, std::size_t index) {
  return index + 1 < lines.size() && startsWith(trim(lines[index]), "|") &&
         isTableSeparator(lines[index + 1]);
}

// Write one Typst table cell with optional header styling
void writeTableCell(std::ostream &output, std::string_view cell, bool header) {
  if (header) {
    output << "    [#strong[" << escapeInlineMarkdown(cell) << "]],\n";
  } else {
    output << "    [" << escapeInlineMarkdown(cell) << "],\n";
  }
}

// Convert a Markdown table to a non-breaking Typst table block
std::size_t writeTable(std::ostream &output,
                       const std::vector<std::string> &lines,
                       std::size_t startIndex) {
  const auto headerCells = splitTableRow(lines[startIndex]);
  auto index = startIndex + 2;

  // Keep every source table as a single visual unit because course tables are
  // small
  output << "#block(breakable: false)[\n";
  output << "  #table(\n";
  output << "    columns: " << headerCells.size() << ",\n";
  output << "    inset: (x: 6pt, y: 5pt),\n";
  output << "    stroke: rgb(\"#d8e2f0\"),\n";
  output << "    fill: (_, y) => if y == 0 { rgb(\"#eaf2ff\") },\n";

  for (const auto &cell : headerCells) {
    writeTableCell(output, cell, true);
  }

  while (index < lines.size() && startsWith(trim(lines[index]), "|")) {
    for (const auto &cell : splitTableRow(lines[index])) {
      writeTableCell(output, cell, false);
    }

    index++;
  }

  output << "  )\n";
  output << "]\n";
  return index;
}

// Convert one non-table Markdown line
void writeConvertedLine(std::ostream &output, const std::string &line,
                        bool &skippedDocumentTitle) {
  auto headingLevel = 0;
  while (headingLevel < static_cast<int>(line.size()) &&
         line[headingLevel] == '#') {
    headingLevel++;
  }

  if (headingLevel > 0 && headingLevel < static_cast<int>(line.size()) &&
      line[headingLevel] == ' ') {
    const auto headingText = std::string_view(line).substr(headingLevel + 1);
    if (headingLevel == 1 && !skippedDocumentTitle) {
      skippedDocumentTitle = true;
      return;
    }

    const auto typstHeadingLevel = std::max(1, headingLevel - 1);
    output << std::string(typstHeadingLevel, '=') << ' '
           << escapeInlineMarkdown(headingText) << '\n';
    return;
  }

  if (startsWith(line, "- ")) {
    output << "- " << escapeInlineMarkdown(std::string_view(line).substr(2))
           << '\n';
    return;
  }

  output << escapeInlineMarkdown(line) << '\n';
}

// Convert a lab README.md file to a Typst source file
DocumentInfo convertMarkdownToTypst(const std::filesystem::path &markdownPath,
                                    const std::filesystem::path &typstPath,
                                    const std::filesystem::path &templatePath) {
  const auto lines = readLines(markdownPath);
  const auto title = makeDisplayTitle(detectTitle(lines, markdownPath));
  const auto labNumber = detectLabNumber(markdownPath);
  const auto sourceHash = shortReadmeHash(lines);
  const auto importPath =
      makeRelativeImportPath(typstPath.parent_path(), templatePath);

  std::filesystem::create_directories(typstPath.parent_path());

  std::ofstream output(typstPath);
  if (!output) {
    throw std::runtime_error("failed to open output file: " +
                             typstPath.string());
  }

  output << "#import \"" << escapeTypstString(importPath.generic_string())
         << "\": lab\n";
  output << "#show: lab.with(\n";
  output << "  title: \"" << escapeTypstString(title) << "\",\n";
  output << "  number: \"" << escapeTypstString(labNumber) << "\",\n";
  output << "  source-hash: \"" << escapeTypstString(sourceHash) << "\",\n";
  output << ")\n\n";

  bool inCodeBlock = false;
  bool skippedDocumentTitle = false;

  for (std::size_t index = 0; index < lines.size();) {
    const auto &line = lines[index];

    // Keep fenced code blocks byte-for-byte so examples never get typography
    // substitutions
    if (startsWith(line, "```")) {
      inCodeBlock = !inCodeBlock;
      output << line << '\n';
      index++;
      continue;
    }

    if (inCodeBlock || line.empty()) {
      output << line << '\n';
      index++;
      continue;
    }

    if (isTableStart(lines, index)) {
      index = writeTable(output, lines, index);
      continue;
    }

    writeConvertedLine(output, line, skippedDocumentTitle);
    index++;
  }

  return DocumentInfo{
      .title = title,
      .number = labNumber,
      .sourceHash = sourceHash,
  };
}
