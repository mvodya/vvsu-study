#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

struct SensorFile {
  std::string fileName;
  double baseValue;
  double dailyWave;
  double noise;
  double minValue;
  double maxValue;
  int count;
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

// Generate raw measurements for one sensor
std::vector<double> generateMeasurements(const SensorFile &sensor,
                                         std::mt19937 &random) {
  // Small random measurement noise
  std::normal_distribution<double> noiseDistribution =
      std::normal_distribution<double>{0.0, sensor.noise};

  // Rare noticeable spikes
  std::bernoulli_distribution spikeChance = std::bernoulli_distribution{0.08};
  std::normal_distribution<double> spikeDistribution =
      std::normal_distribution<double>{0.0, sensor.noise * 4.0};

  std::vector<double> measurements;
  measurements.reserve(static_cast<std::size_t>(sensor.count));

  // Make records
  for (int index = 0; index < sensor.count; index++) {
    const double phase =
        static_cast<double>(index) / static_cast<double>(sensor.count - 1);

    // Smooth day-like value change
    double value =
        sensor.baseValue +
        std::sin(phase * 2.0 * std::numbers::pi - std::numbers::pi / 2.0) *
            sensor.dailyWave +
        noiseDistribution(random);

    // Add several values that stand out from normal trend
    if (spikeChance(random)) {
      value += spikeDistribution(random);
    }

    measurements.push_back(std::clamp(value, sensor.minValue, sensor.maxValue));
  }

  return measurements;
}

// Save measurements to file
void writeMeasurements(const std::filesystem::path &filePath,
                       const std::vector<double> &measurements) {
  std::ofstream output{filePath};

  if (!output) {
    throw std::runtime_error{"Failed to open output file: " +
                             filePath.string()};
  }

  output << std::fixed << std::setprecision(1);

  for (const double value : measurements) {
    output << value << '\n';
  }
}

// Generate all sensor files
void generateSensorFiles(const std::filesystem::path &outputDirectory) {
  // Sensor files from lab table
  const std::array<SensorFile, 5> sensors{{
      {"temp_air_01.txt", 22.0, 4.0, 0.5, 14.0, 35.0, 96},
      {"temp_air_02.txt", 21.4, 3.6, 0.6, 14.0, 35.0, 96},
      {"humidity_air_01.txt", 67.0, 8.0, 2.0, 35.0, 95.0, 120},
      {"light_east_01.txt", 470.0, 330.0, 35.0, 0.0, 950.0, 144},
      {"light_west_01.txt", 430.0, 300.0, 40.0, 0.0, 920.0, 144},
  }};

  // Fixed seed for stable generation
  std::mt19937 random = std::mt19937{20260910U + 3U};

  for (const auto &sensor : sensors) {
    writeMeasurements(outputDirectory / sensor.fileName,
                      generateMeasurements(sensor, random));
  }
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_03_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} / "03-lifetime-raii";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));

    generateSensorFiles(outputDirectory);

    std::cout << "Generated sensor files in " << outputDirectory << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
