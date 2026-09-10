#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Forecast {
  std::string city;
  double temperatureC;
  double windMs;
  int humidity;
};

struct ServiceFile {
  std::string fileName;
  std::vector<std::string> lines;
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

// Convert Celsius to Fahrenheit
double toFahrenheit(double temperatureC) {
  return temperatureC * 9.0 / 5.0 + 32.0;
}

// Convert meters per second to miles per hour
double toMph(double windMs) {
  return windMs * 2.2369362921;
}

// Convert Celsius to Kelvin
double toKelvin(double temperatureC) {
  return temperatureC + 273.15;
}

// Convert meters per second to kilometers per hour
double toKmh(double windMs) {
  return windMs * 3.6;
}

// Format floating point value
std::string formatValue(double value) {
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(1) << value;
  return stream.str();
}

// Generate Service A format
ServiceFile makeServiceAFile(const std::string &fileName,
                             const Forecast &forecast) {
  return {
      fileName,
      {
          "city=" + forecast.city,
          "temperature_c=" + formatValue(forecast.temperatureC),
          "wind_ms=" + formatValue(forecast.windMs),
          "humidity=" + std::to_string(forecast.humidity),
      },
  };
}

// Generate Service B format
ServiceFile makeServiceBFile(const std::string &fileName,
                             const Forecast &forecast) {
  return {
      fileName,
      {
          forecast.city + ";" + formatValue(toFahrenheit(forecast.temperatureC)) +
              ";" + formatValue(toMph(forecast.windMs)) + ";" +
              std::to_string(forecast.humidity),
      },
  };
}

// Generate Service C format
ServiceFile makeServiceCFile(const std::string &fileName,
                             const Forecast &forecast) {
  return {
      fileName,
      {
          "name: " + forecast.city,
          "temp_kelvin: " + formatValue(toKelvin(forecast.temperatureC)),
          "wind_kmh: " + formatValue(toKmh(forecast.windMs)),
          "humidity_percent: " + std::to_string(forecast.humidity),
      },
  };
}

// Generate all weather service files
std::vector<ServiceFile> generateServiceFiles() {
  const std::array<Forecast, 5> forecasts{{
      {"Vladivostok", 18.5, 6.2, 72},
      {"Khabarovsk", 21.3, 3.8, 58},
      {"Ussuriysk", 19.7, 4.5, 68},
      {"Nakhodka", 17.9, 7.1, 76},
      {"Artem", 20.1, 5.0, 64},
  }};

  std::vector<ServiceFile> files;

  // Base files from lab example
  files.push_back(makeServiceAFile("service_a.txt", forecasts[0]));
  files.push_back(makeServiceBFile("service_b.txt", forecasts[0]));
  files.push_back(makeServiceCFile("service_c.txt", forecasts[0]));

  // More valid files for several cities
  for (std::size_t index = 1; index < forecasts.size(); index++) {
    files.push_back(makeServiceAFile("service_a_" + forecasts[index].city + ".txt",
                                     forecasts[index]));
    files.push_back(makeServiceBFile("service_b_" + forecasts[index].city + ".txt",
                                     forecasts[index]));
    files.push_back(makeServiceCFile("service_c_" + forecasts[index].city + ".txt",
                                     forecasts[index]));
  }

  // Unknown format
  files.push_back({
      "invalid_unknown_format.txt",
      {
          "Vladivostok temperature 18.5 wind 6.2 humidity 72",
      },
  });

  // Missing required field
  files.push_back({
      "invalid_missing_field.txt",
      {
          "city=Vladivostok",
          "temperature_c=18.5",
          "humidity=72",
      },
  });

  // Broken numeric value
  files.push_back({
      "invalid_number.txt",
      {
          "name: Vladivostok",
          "temp_kelvin: warm",
          "wind_kmh: 22.3",
          "humidity_percent: 72",
      },
  });

  // Value outside reasonable range
  files.push_back({
      "invalid_range.txt",
      {
          "city=Vladivostok",
          "temperature_c=118.5",
          "wind_ms=6.2",
          "humidity=72",
      },
  });

  // Damaged Service B line
  files.push_back({
      "invalid_damaged_line.txt",
      {
          "Vladivostok;65.3;13.9",
      },
  });

  return files;
}

// Save service file
void writeServiceFile(const std::filesystem::path &filePath,
                      const std::vector<std::string> &lines) {
  std::ofstream output{filePath};

  if (!output) {
    throw std::runtime_error{"Failed to open output file: " +
                             filePath.string()};
  }

  for (const auto &line : lines) {
    output << line << '\n';
  }
}

// Generate all files
void generateFiles(const std::filesystem::path &outputDirectory) {
  const auto servicesDirectory = outputDirectory / "services";
  const auto invalidDirectory = outputDirectory / "invalid";

  std::filesystem::create_directories(servicesDirectory);
  std::filesystem::create_directories(invalidDirectory);

  for (const auto &file : generateServiceFiles()) {
    const auto targetDirectory = file.fileName.starts_with("invalid_")
                                     ? invalidDirectory
                                     : servicesDirectory;
    writeServiceFile(targetDirectory / file.fileName, file.lines);
  }
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_16_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} / "16-structural-patterns";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));

    generateFiles(outputDirectory);

    std::cout << "Generated weather service files in " << outputDirectory
              << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
