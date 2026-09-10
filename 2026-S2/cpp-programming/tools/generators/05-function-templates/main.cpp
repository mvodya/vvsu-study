#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct WeatherRecord {
  std::string time;
  double temperature;
  int humidity;
  double pressure;
  float wind;
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

// Convert measurement index to HH:MM
std::string formatTime(int index) {
  const int totalMinutes = index * 30;
  const int hours = totalMinutes / 60;
  const int minutes = totalMinutes % 60;

  std::ostringstream stream;
  stream << std::setw(2) << std::setfill('0') << hours << ':' << std::setw(2)
         << std::setfill('0') << minutes;

  return stream.str();
}

// Generate weather records for one day
std::vector<WeatherRecord> generateWeatherRecords() {
  constexpr int recordCount = 48;

  // Fixed seed for stable generation
  std::mt19937 random = std::mt19937{20260910U + 5U};

  // Small random temperature noise
  std::normal_distribution<double> temperatureNoise =
      std::normal_distribution<double>{0.0, 0.7};

  // Small random humidity noise
  std::normal_distribution<double> humidityNoise =
      std::normal_distribution<double>{0.0, 3.0};

  // Small random pressure noise
  std::normal_distribution<double> pressureNoise =
      std::normal_distribution<double>{0.0, 1.4};

  // Wind noise with occasional stronger gusts
  std::normal_distribution<float> windNoise =
      std::normal_distribution<float>{0.0F, 0.8F};

  // Rare noticeable weather anomaly
  std::bernoulli_distribution anomalyChance = std::bernoulli_distribution{0.08};

  std::vector<WeatherRecord> records;
  records.reserve(recordCount);

  // Make records
  for (int index = 0; index < recordCount; index++) {
    const double phase =
        static_cast<double>(index) / static_cast<double>(recordCount - 1);

    // Smooth day-like temperature change
    double temperature =
        22.0 +
        std::sin(phase * 2.0 * std::numbers::pi - std::numbers::pi / 2.0) *
            5.0 +
        temperatureNoise(random);

    // Humidity goes down when temperature goes up
    int humidity = static_cast<int>(std::lround(
        67.0 -
        std::sin(phase * 2.0 * std::numbers::pi - std::numbers::pi / 2.0) *
            13.0 +
        humidityNoise(random)));

    // Pressure changes slowly during the day
    double pressure = 1010.0 + std::sin(phase * std::numbers::pi) * 6.0 +
                      pressureNoise(random);

    // Wind has a base value and noise
    float wind = static_cast<float>(
                     3.5 + std::sin(phase * 3.0 * std::numbers::pi) * 2.0) +
                 windNoise(random);

    // Add several values outside normal ranges
    if (anomalyChance(random)) {
      temperature += 9.0;
      wind += 8.0F;
    }

    records.push_back({
        formatTime(index),
        temperature,
        std::clamp(humidity, 20, 100),
        pressure,
        std::clamp(wind, 0.0F, 25.0F),
    });
  }

  // Anti-random guarantee some cases
  // Low temperature
  records[3].temperature = 13.8;

  // High temperature
  records[25].temperature = 31.4;

  // Low humidity
  records[8].humidity = 35;

  // High humidity
  records[39].humidity = 87;

  // Low pressure
  records[12].pressure = 987.6;

  // High pressure
  records[34].pressure = 1033.2;

  // Strong wind
  records[18].wind = 13.7F;

  return records;
}

// Save weather records to CSV file
void writeWeatherCsv(const std::filesystem::path &filePath,
                     const std::vector<WeatherRecord> &records) {
  std::ofstream output{filePath};

  if (!output) {
    throw std::runtime_error{"Failed to open output file: " +
                             filePath.string()};
  }

  output << "time,temperature,humidity,pressure,wind\n";
  output << std::fixed << std::setprecision(1);

  for (const auto &record : records) {
    output << record.time << ',' << record.temperature << ',' << record.humidity
           << ',' << record.pressure << ',' << record.wind << '\n';
  }
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_05_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} / "05-function-templates";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));
    const auto outputFile = outputDirectory / "weather_day.csv";

    writeWeatherCsv(outputFile, generateWeatherRecords());

    std::cout << "Generated " << outputFile << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
