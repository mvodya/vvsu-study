#pragma once

#include <filesystem>
#include <string>

struct DocumentInfo {
  std::string title;
  std::string number;
  std::string sourceHash;
};

// Convert lab Markdown to a temporary Typst file
DocumentInfo convertMarkdownToTypst(const std::filesystem::path &markdownPath,
                                    const std::filesystem::path &typstPath,
                                    const std::filesystem::path &templatePath);
