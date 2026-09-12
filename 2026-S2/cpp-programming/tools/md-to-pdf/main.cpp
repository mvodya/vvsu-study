#include "document_converter.hpp"
#include "pdf_builder.hpp"

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

std::ofstream openHashManifest(const std::filesystem::path &intermediateDirectory) {
  const auto manifestPath = intermediateDirectory.parent_path() / "README-hashes.txt";
  std::filesystem::create_directories(manifestPath.parent_path());

  std::ofstream output(manifestPath);
  if (!output) {
    throw std::runtime_error("failed to open hash manifest: " + manifestPath.string());
  }

  output << "lab\thash\ttitle\n";
  std::cout << "README hashes: " << manifestPath.string() << '\n';
  return output;
}

int main(int argc, char **argv) {
  try {
    const auto options = parseOptions(argc, argv);
    const auto readmes = collectLabReadmes(options.labsDirectory);
    auto hashManifest = openHashManifest(options.intermediateDirectory);

    // Each lab gets one temporary Typst file and one final PDF next to
    // README.md
    for (const auto &readmePath : readmes) {
      const auto typstPath = makeIntermediateTypstPath(
          readmePath, options.labsDirectory, options.intermediateDirectory);
      const auto labName = readmePath.parent_path().filename();
      const auto pdfPath =
          readmePath.parent_path() / (labName.string() + ".pdf");

      const auto documentInfo = convertMarkdownToTypst(readmePath, typstPath, options.templatePath);
      hashManifest << labName.string() << '\t' << documentInfo.sourceHash << '\t' << documentInfo.title << '\n';

      std::cout << "Generated \"" << typstPath.string() << "\"\n";
      std::cout << "README hash " << labName.string() << ": " << documentInfo.sourceHash << "\n";

      if (options.compilePdf) {
        compileTypst(typstPath, pdfPath, options.rootDirectory);
        std::cout << "Generated \"" << pdfPath.string() << "\"\n";
      }
    }
  } catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
  }

  return 0;
}
