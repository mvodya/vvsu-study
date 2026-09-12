#pragma once

#include <filesystem>
#include <vector>

// Command line options for the md_to_pdf tool
struct Options {
  std::filesystem::path labsDirectory;
  std::filesystem::path templatePath;
  std::filesystem::path intermediateDirectory;
  std::filesystem::path rootDirectory;
  bool compilePdf = false;
};

// Parse tool options
Options parseOptions(int argc, char **argv);

// Find lab README.md files
std::vector<std::filesystem::path>
collectLabReadmes(const std::filesystem::path &labsDirectory);

// Build a temporary Typst path under build
std::filesystem::path
makeIntermediateTypstPath(const std::filesystem::path &readmePath,
                          const std::filesystem::path &labsDirectory,
                          const std::filesystem::path &intermediateDirectory);

// Compile Typst and normalize PDF metadata
void compileTypst(const std::filesystem::path &typstPath,
                  const std::filesystem::path &pdfPath,
                  const std::filesystem::path &rootDirectory);
