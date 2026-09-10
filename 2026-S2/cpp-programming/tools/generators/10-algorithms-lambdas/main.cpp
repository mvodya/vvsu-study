#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

struct Film {
  std::string title;
  std::string country;
  int year;
  std::string genre;
  int duration;
  int ageRating;
  double juryScore;
};

std::filesystem::path
requireOutputDirectory(const std::filesystem::path &path) {
  if (path.empty()) {
    throw std::runtime_error{"Output directory is empty"};
  }

  // Try to make dir
  std::filesystem::create_directories(path);

  // Error, if target is not dir
  if (!std::filesystem::is_directory(path)) {
    throw std::runtime_error{"Output path is not a directory: " +
                             path.string()};
  }

  // Return full path
  return std::filesystem::absolute(path);
}

// Generate sequential film title
std::string makeTitle(int number, std::mt19937 &random) {
  const std::array<std::string, 16> adjectives{{
      "Silent",
      "Neon",
      "Northern",
      "Hidden",
      "Golden",
      "Last",
      "Blue",
      "Broken",
      "Distant",
      "Wild",
      "Bright",
      "Cold",
      "Summer",
      "Midnight",
      "Paper",
      "Electric",
  }};

  const std::array<std::string, 16> nouns{{
      "Harbor",
      "District",
      "Lights",
      "River",
      "Garden",
      "Signal",
      "Road",
      "Window",
      "Forest",
      "Archive",
      "Bridge",
      "Ocean",
      "Station",
      "Memory",
      "Village",
      "Orbit",
  }};

  // Random adjective index
  std::uniform_int_distribution<int> adjectiveDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(adjectives.size()) - 1};

  // Random noun index
  std::uniform_int_distribution<int> nounDistribution =
      std::uniform_int_distribution<int>{0, static_cast<int>(nouns.size()) - 1};

  return adjectives[static_cast<std::size_t>(adjectiveDistribution(random))] +
         " " + nouns[static_cast<std::size_t>(nounDistribution(random))] + " " +
         std::to_string(number);
}

// Round score to one decimal digit
double roundScore(double value) { return std::round(value * 10.0) / 10.0; }

// Generate film catalog
std::vector<Film> generateFilms() {
  constexpr int filmCount = 240;

  const std::array<std::string, 14> countries{{
      "France",
      "Japan",
      "Canada",
      "Brazil",
      "Germany",
      "Italy",
      "Spain",
      "India",
      "Norway",
      "Mexico",
      "China",
      "Korea",
      "Poland",
      "Chile",
  }};

  const std::array<std::string, 10> genres{{
      "Drama",
      "Sci-Fi",
      "Documentary",
      "Comedy",
      "Thriller",
      "Animation",
      "History",
      "Adventure",
      "Family",
      "Mystery",
  }};

  const std::array<int, 5> ageRatings{{0, 6, 12, 16, 18}};

  // Fixed seed for stable generation
  std::mt19937 random = std::mt19937{20260910U + 10U};

  // Random country index
  std::uniform_int_distribution<int> countryDistribution =
      std::uniform_int_distribution<int>{0, static_cast<int>(countries.size()) -
                                                1};

  // Random genre index
  std::uniform_int_distribution<int> genreDistribution =
      std::uniform_int_distribution<int>{0,
                                         static_cast<int>(genres.size()) - 1};

  // Random year in recent festival range
  std::uniform_int_distribution<int> yearDistribution =
      std::uniform_int_distribution<int>{2016, 2026};

  // Random film duration
  std::uniform_int_distribution<int> durationDistribution =
      std::uniform_int_distribution<int>{72, 168};

  // Random age rating index
  std::uniform_int_distribution<int> ageRatingDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(ageRatings.size()) - 1};

  // Most scores stay near festival average
  std::normal_distribution<double> scoreDistribution =
      std::normal_distribution<double>{7.4, 1.1};

  std::vector<Film> films;
  films.reserve(filmCount);

  // Make records
  for (int index = 1; index <= filmCount; index++) {
    films.push_back({
        makeTitle(index, random),
        countries[static_cast<std::size_t>(countryDistribution(random))],
        yearDistribution(random),
        genres[static_cast<std::size_t>(genreDistribution(random))],
        durationDistribution(random),
        ageRatings[static_cast<std::size_t>(ageRatingDistribution(random))],
        std::clamp(roundScore(scoreDistribution(random)), 4.0, 9.8),
    });
  }

  // Anti-random guarantee some cases
  // Required film by title
  films[0] = {"Neon District", "Japan", 2024, "Sci-Fi", 118, 12, 9.1};

  // First high score candidate
  films[1] = {"The Silent Harbor", "France", 2022, "Drama", 104, 16, 8.7};

  // Documentary candidate
  films[2] = {"Northern Lights", "Canada", 2021, "Documentary", 92, 6, 7.9};

  // Evening program candidate
  films[3] = {"Golden Bridge", "Italy", 2023, "Drama", 115, 12, 8.6};

  // Highest score candidate
  films[4] = {"Paper Orbit", "Norway", 2025, "Animation", 87, 6, 9.7};

  // Long high score outside evening duration
  films[5] = {"Distant Archive", "Germany", 2020, "History", 151, 16, 9.2};

  // More documentary records
  for (int index = 20; index < 44; index++) {
    films[static_cast<std::size_t>(index)].genre = "Documentary";
  }

  // More family-friendly records
  for (int index = 44; index < 76; index++) {
    films[static_cast<std::size_t>(index)].ageRating = 12;
  }

  // More evening program records
  for (int index = 76; index < 110; index++) {
    films[static_cast<std::size_t>(index)].duration = 90 + index % 31;
    films[static_cast<std::size_t>(index)].juryScore =
        8.0 + (index % 16) / 10.0;
  }

  return films;
}

// Save films to CSV file
void writeFilms(const std::filesystem::path &filePath,
                const std::vector<Film> &films) {
  std::ofstream output{filePath};

  if (!output) {
    throw std::runtime_error{"Failed to open output file: " +
                             filePath.string()};
  }

  output << "title,country,year,genre,duration,age_rating,jury_score\n";
  output << std::fixed << std::setprecision(1);

  for (const auto &film : films) {
    output << film.title << ',' << film.country << ',' << film.year << ','
           << film.genre << ',' << film.duration << ',' << film.ageRating << ','
           << film.juryScore << '\n';
  }
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_10_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} / "10-algorithms-lambdas";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));
    const auto outputFile = outputDirectory / "films.csv";

    writeFilms(outputFile, generateFilms());

    std::cout << "Generated " << outputFile << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
