#include "pdf_builder.hpp"

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <string_view>

// Quote one filesystem path for a POSIX shell command
std::string shellQuote(const std::filesystem::path &path) {
  std::string result = "'";

  for (const char character : path.string()) {
    if (character == '\'') {
      result += "'\\''";
    } else {
      result.push_back(character);
    }
  }

  result.push_back('\'');
  return result;
}

// Run a shell command and turn any non-zero exit code into an exception
void runCommand(const std::string &command, const std::string &errorMessage) {
  const auto exitCode = std::system(command.c_str());
  if (exitCode != 0) {
    throw std::runtime_error(errorMessage);
  }
}

// Parse positional paths and optional flags for this tool
Options parseOptions(int argc, char **argv) {
  if (argc < 4) {
    throw std::runtime_error("usage: md_to_pdf <labs-dir> <template-path> "
                             "<intermediate-dir> [--compile] [--root <dir>]");
  }

  Options options{
      .labsDirectory = argv[1],
      .templatePath = argv[2],
      .intermediateDirectory = argv[3],
  };

  // Default root is the repository root when the template is under
  // tools/md-to-pdf
  options.rootDirectory =
      options.templatePath.parent_path().parent_path().parent_path();

  for (int index = 4; index < argc; index++) {
    const std::string_view argument = argv[index];
    if (argument == "--compile") {
      options.compilePdf = true;
    } else if (argument == "--root") {
      index++;
      if (index >= argc) {
        throw std::runtime_error("missing value for --root");
      }

      options.rootDirectory = argv[index];
    } else {
      throw std::runtime_error("unknown argument: " + std::string(argument));
    }
  }

  return options;
}

// Collect lab README files in stable order
std::vector<std::filesystem::path>
collectLabReadmes(const std::filesystem::path &labsDirectory) {
  std::vector<std::filesystem::path> readmes;

  for (const auto &entry : std::filesystem::directory_iterator(labsDirectory)) {
    if (!entry.is_directory()) {
      continue;
    }

    const auto readmePath = entry.path() / "README.md";
    if (std::filesystem::exists(readmePath)) {
      readmes.push_back(readmePath);
    }
  }

  std::ranges::sort(readmes);
  return readmes;
}

// Put generated Typst files under build while preserving lab folder names
std::filesystem::path
makeIntermediateTypstPath(const std::filesystem::path &readmePath,
                          const std::filesystem::path &labsDirectory,
                          const std::filesystem::path &intermediateDirectory) {
  // Mirror the lab directory names inside build so generated Typst stays out of
  // the repository tree
  const auto labRelativePath =
      std::filesystem::relative(readmePath.parent_path(), labsDirectory);
  return intermediateDirectory / labRelativePath / "README.typ";
}

// Rewrite PDF metadata after Typst compilation
void rewritePdfMetadata(const std::filesystem::path &pdfPath) {
  // Typst writes the requested Author, but macOS Spotlight may display the
  // producer as the author Rewriting Creator/Producer with ExifTool makes
  // Finder/mdls show Mark Vodyanitskiy consistently
  runCommand("command -v exiftool >/dev/null",
             "exiftool is required to rewrite PDF metadata");
  const auto command = "exiftool -overwrite_original"
                       " -Author='Mark Vodyanitskiy'"
                       " -Creator='Mark Vodyanitskiy'"
                       " -Producer='Mark Vodyanitskiy' " +
                       shellQuote(pdfPath) + " >/dev/null";
  runCommand(command, "failed to rewrite PDF metadata: " + pdfPath.string());
}

// Compile a generated Typst file to the final lab PDF
void compileTypst(const std::filesystem::path &typstPath,
                  const std::filesystem::path &pdfPath,
                  const std::filesystem::path &rootDirectory) {
  // --root keeps imports sandboxed while allowing generated files to import the
  // shared template
  const auto command = "typst compile --root " + shellQuote(rootDirectory) +
                       ' ' + shellQuote(typstPath) + ' ' + shellQuote(pdfPath);
  runCommand(command, "typst compile failed for: " + typstPath.string());
  rewritePdfMetadata(pdfPath);
}
