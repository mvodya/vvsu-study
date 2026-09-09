#include <algorithm>
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

struct VehicleType {
  std::string name;
  std::string validZoneA;
  std::string validZoneB;
  bool isTruck;
};

struct ParkingSession {
  int enterMinute;
  int exitMinute;
  std::string plate;
  std::string type;
  std::string zone;
  int cargoWeight;
};

struct ParkingEvent {
  int minute;
  int sessionIndex;
  bool isEnter;
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

// Convert minutes from day start to HH:MM
std::string formatTime(int totalMinutes) {
  const int hours = totalMinutes / 60;
  const int minutes = totalMinutes % 60;

  std::ostringstream stream;
  stream << std::setw(2) << std::setfill('0') << hours << ':' << std::setw(2)
         << std::setfill('0') << minutes;

  return stream.str();
}

// Generate sequential plate number
std::string makePlate(int number) {
  const std::array<std::string, 8> prefixes{
      {"A", "B", "C", "M", "T", "K", "P", "H"}};
  const std::array<std::string, 8> suffixes{
      {"BC", "TY", "MM", "AA", "CC", "OP", "EK", "VH"}};

  std::ostringstream stream;
  stream << prefixes[static_cast<std::size_t>(number % prefixes.size())]
         << std::setw(3) << std::setfill('0') << (100 + number % 900)
         << suffixes[static_cast<std::size_t>(number % suffixes.size())];

  return stream.str();
}

// Pick valid or intentionally invalid zone
std::string pickZone(const VehicleType &type, int index, std::mt19937 &random) {
  const std::array<std::string, 3> allZones{{"A", "B", "C"}};

  // Guaranteed invalid parking sessions
  if (index % 11 == 0 || index == 5 || index == 17) {
    for (const auto &zone : allZones) {
      if (zone != type.validZoneA && zone != type.validZoneB) {
        return zone;
      }
    }
  }

  // Single-zone vehicle type
  if (type.validZoneB.empty()) {
    return type.validZoneA;
  }

  // Valid zone index for vehicle type with two zones
  std::uniform_int_distribution<int> zoneDistribution =
      std::uniform_int_distribution<int>{0, 1};

  return zoneDistribution(random) == 0 ? type.validZoneA : type.validZoneB;
}

// Generate parking sessions
std::vector<ParkingSession> generateParkingSessions() {
  constexpr int sessionCount = 90;

  // Vehicle types from lab table
  const std::array<VehicleType, 4> vehicleTypes{{
      {"CAR", "A", "B", false},
      {"MOTORCYCLE", "A", "B", false},
      {"MINIBUS", "B", "", false},
      {"TRUCK", "C", "", true},
  }};

  // Fixed seed for stable generation
  std::mt19937 random = std::mt19937{20260910U + 4U};

  // Random vehicle type index
  std::uniform_int_distribution<int> typeDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(vehicleTypes.size()) - 1};

  // Parking starts during work day
  std::uniform_int_distribution<int> enterTimeDistribution =
      std::uniform_int_distribution<int>{7 * 60, 19 * 60};

  // Parking duration from short visit to long stay
  std::uniform_int_distribution<int> durationDistribution =
      std::uniform_int_distribution<int>{20, 390};

  // Cargo weight for truck
  std::uniform_int_distribution<int> cargoDistribution =
      std::uniform_int_distribution<int>{2, 18};

  std::vector<ParkingSession> sessions;
  sessions.reserve(sessionCount);

  // Make records
  for (int index = 1; index <= sessionCount; index++) {
    // Pick vehicle type first, because zone rules depend on type
    const auto &vehicleType =
        vehicleTypes[static_cast<std::size_t>(typeDistribution(random))];

    const int enterMinute = enterTimeDistribution(random);
    int exitMinute = enterMinute + durationDistribution(random);

    // Keep exit time inside one day
    if (exitMinute >= 24 * 60) {
      exitMinute = 23 * 60 + 59;
    }

    sessions.push_back({
        enterMinute,
        exitMinute,
        makePlate(index),
        vehicleType.name,
        pickZone(vehicleType, index, random),
        vehicleType.isTruck ? cargoDistribution(random) : 0,
    });
  }

  // Anti-random guarantee some cases
  // Car in invalid cargo zone
  sessions[0].type = "CAR";
  sessions[0].zone = "C";
  sessions[0].cargoWeight = 0;

  // Minibus in invalid open zone
  sessions[1].type = "MINIBUS";
  sessions[1].zone = "A";
  sessions[1].cargoWeight = 0;

  // Truck in valid cargo zone
  sessions[2].type = "TRUCK";
  sessions[2].zone = "C";
  sessions[2].cargoWeight = 8;

  // Truck in invalid covered zone
  sessions[3].type = "TRUCK";
  sessions[3].zone = "B";
  sessions[3].cargoWeight = 5;

  // Motorcycle with less than one hour duration
  sessions[4].type = "MOTORCYCLE";
  sessions[4].zone = "A";
  sessions[4].exitMinute = sessions[4].enterMinute + 25;
  sessions[4].cargoWeight = 0;

  return sessions;
}

// Make chronological event log
std::vector<ParkingEvent>
makeParkingEvents(const std::vector<ParkingSession> &sessions) {
  std::vector<ParkingEvent> events;
  events.reserve(sessions.size() * 2);

  // Make enter and exit event for every session
  for (std::size_t index = 0; index < sessions.size(); index++) {
    const int sessionIndex = static_cast<int>(index);

    events.push_back({sessions[index].enterMinute, sessionIndex, true});
    events.push_back({sessions[index].exitMinute, sessionIndex, false});
  }

  // Sort events by time
  std::ranges::sort(events,
                    [](const ParkingEvent &left, const ParkingEvent &right) {
                      if (left.minute != right.minute) {
                        return left.minute < right.minute;
                      }

                      if (left.isEnter != right.isEnter) {
                        return left.isEnter && !right.isEnter;
                      }

                      return left.sessionIndex < right.sessionIndex;
                    });

  return events;
}

// Save parking log to file
void writeParkingLog(const std::filesystem::path &filePath,
                     const std::vector<ParkingSession> &sessions) {
  std::ofstream output{filePath};

  if (!output) {
    throw std::runtime_error{"Failed to open output file: " +
                             filePath.string()};
  }

  const auto events = makeParkingEvents(sessions);

  for (const auto &event : events) {
    const auto &session =
        sessions[static_cast<std::size_t>(event.sessionIndex)];

    if (!event.isEnter) {
      output << "EXIT " << formatTime(event.minute) << ' ' << session.plate
             << '\n';
      continue;
    }

    output << "ENTER " << formatTime(event.minute) << ' ' << session.plate
           << ' ' << session.type << ' ' << session.zone;

    if (session.type == "TRUCK") {
      output << ' ' << session.cargoWeight;
    }

    output << '\n';
  }
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_04_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} / "04-polymorphism";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));
    const auto outputFile = outputDirectory / "parking_log.txt";

    writeParkingLog(outputFile, generateParkingSessions());

    std::cout << "Generated " << outputFile << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
