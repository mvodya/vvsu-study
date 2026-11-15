#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using Row = std::vector<std::string>;
using Table = std::map<std::string, Row>;

// IMDb Non-Commercial Datasets:
// https://developer.imdb.com/non-commercial-datasets/
Table readTable(const fs::path &path, std::size_t columns) {
  std::ifstream input{path};
  std::string line;
  if (!std::getline(input, line))
    throw std::runtime_error{"Cannot read " + path.string()};

  Table table;
  while (std::getline(input, line)) {
    std::istringstream stream{line};
    Row row;
    std::string field;
    while (std::getline(stream, field, '\t'))
      row.push_back(field);
    if (row.size() != columns || !table.emplace(row.front(), row).second)
      throw std::runtime_error{"Invalid record in " + path.string()};
  }
  if (input.bad())
    throw std::runtime_error{"Read failed: " + path.string()};
  return table;
}

int main(int argc, char *argv[]) {
  try {
    if (argc > 2)
      throw std::runtime_error{
          "Usage: generate_lab_10_data [output-directory]"};
    const fs::path source{LAB_10_SOURCE_DIR};
    const auto titles = readTable(source / "source/title.basics.tsv", 9);
    const auto ratings = readTable(source / "source/title.ratings.tsv", 3);
    std::ifstream selection{source / "selection.tsv"};
    std::string line;
    if (!std::getline(selection, line))
      throw std::runtime_error{"Cannot read selection.tsv"};

    std::ostringstream catalog;
    catalog << "id\ttitle\ttype\tgenres\ttag\tyear\trating\tvotes\n";
    while (std::getline(selection, line)) {
      std::istringstream fields{line};
      std::string id, tag;
      std::getline(fields, id, '\t');
      std::getline(fields, tag, '\t');
      if (!titles.contains(id) || !ratings.contains(id))
        throw std::runtime_error{"Missing IMDb record: " + id};
      const auto &title = titles.at(id);
      const auto &rating = ratings.at(id);
      catalog << id << '\t' << title[2] << '\t' << title[1] << '\t' << title[8]
              << '\t' << tag << '\t' << title[5] << '\t' << rating[1] << '\t'
              << rating[2] << '\n';
    }
    if (selection.bad())
      throw std::runtime_error{"Cannot read selection.tsv"};

    fs::path directory =
        argc == 2 ? fs::path{argv[1]}
                  : fs::path{CPP_COURSE_LABS_DIR} / "10-algorithms-lambdas";
    fs::create_directories(directory);
    auto path = directory / "titles.tsv";
    std::ofstream output{path};
    output << catalog.str();
    output.close();
    if (!output)
      throw std::runtime_error{"Cannot write " + path.string()};
    std::cout << "Generated " << path << '\n';
  } catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
