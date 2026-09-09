#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct ScooterModel {
  std::string name;
  double minMileage;
  double maxMileage;
};

struct ScooterRecord {
  std::string id;
  std::string model;
  int batteryPercent;
  double mileage;
  bool hasFault;
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

// Generate sequential scooter ID
std::string makeId(int number) {
  std::ostringstream stream;
  stream << "SC" << std::setw(3) << std::setfill('0') << number;
  return stream.str();
}

// Generate scooters records
std::vector<ScooterRecord> generateScooters() {
  constexpr int recordCount = 220;

  // Scooter models
  const std::array<ScooterModel, 10> models{{
      {"NinebotG2", 120.0, 2100.0},
      {"NinebotF2", 80.0, 1800.0},
      {"NinebotMax", 300.0, 2600.0},
      {"Xiaomi4", 60.0, 1700.0},
      {"Xiaomi4Pro", 100.0, 2300.0},
      {"KugooKirinM4", 200.0, 2400.0},
      {"OkaiNeon", 50.0, 1600.0},
      {"AovoProES80", 40.0, 1400.0},
      {"UrbanGlide100", 70.0, 1900.0},
      {"InmotionS1", 150.0, 2500.0},
  }};

  // Fixed seed for stable generation
  std::mt19937 random = std::mt19937{20260910U};

  // Battery percent from almost empty to full
  std::uniform_int_distribution<int> batteryDistribution =
      std::uniform_int_distribution<int>{5, 100};

  // Base fault probability
  std::bernoulli_distribution faultDistribution =
      std::bernoulli_distribution{0.13};

  // Random scooter model index
  std::uniform_int_distribution<int> modelDistribution =
      std::uniform_int_distribution<int>{0,
                                         static_cast<int>(models.size()) - 1};

  // Mileage around service threshold
  std::normal_distribution<double> serviceMileageDistribution =
      std::normal_distribution<double>{1650.0, 260.0};

  std::vector<ScooterRecord> scooters;
  scooters.reserve(recordCount);

  // Make records
  for (int index = 1; index <= recordCount; index++) {
    // Pick model first, because mileage range depends on model
    const auto &model =
        models[static_cast<std::size_t>(modelDistribution(random))];

    // Usual mileage for selected model
    std::uniform_real_distribution<double> mileageDistribution =
        std::uniform_real_distribution<double>{model.minMileage,
                                               model.maxMileage};

    double mileage = mileageDistribution(random);

    // Add regular service-threshold cases
    if (index % 17 == 0) {
      mileage = serviceMileageDistribution(random);
    }

    // Normal distribution can go below zero
    if (mileage < 0.0) {
      mileage = 0.0;
    }

    // Fault is random with several guaranteed faulty records
    scooters.push_back({
        makeId(index),
        model.name,
        batteryDistribution(random),
        mileage,
        faultDistribution(random) || index % 29 == 0,
    });
  }

  // Anti-random guarantee some cases
  // Low battery
  scooters[2].batteryPercent = 18;

  // Mileage over service threshold
  scooters[3].mileage = 1642.1;

  // Explicit fault
  scooters[4].hasFault = true;

  // Battery exactly below charge threshold
  scooters[18].batteryPercent = 29;

  // Mileage exactly at service threshold
  scooters[41].mileage = 1500.0;

  // Full battery case
  scooters[76].batteryPercent = 100;

  // Very low battery case
  scooters[119].batteryPercent = 5;

  // Another explicit fault
  scooters[158].hasFault = true;

  return scooters;
}

// Save scooters to file
void writeScooters(const std::filesystem::path &filePath,
                   const std::vector<ScooterRecord> &scooters) {
  std::ofstream output{filePath};

  if (!output) {
    throw std::runtime_error{"Failed to open output file: " +
                             filePath.string()};
  }

  output << std::fixed << std::setprecision(1);

  for (const auto &scooter : scooters) {
    output << scooter.id << ' ' << scooter.model << ' '
           << scooter.batteryPercent << ' ' << scooter.mileage << ' '
           << (scooter.hasFault ? 1 : 0) << '\n';
  }
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_01_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} / "01-cpp-foundations";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));
    const auto outputFile = outputDirectory / "scooters.txt";

    writeScooters(outputFile, generateScooters());

    std::cout << "Generated " << outputFile << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
