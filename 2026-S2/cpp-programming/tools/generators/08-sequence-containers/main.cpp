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

struct Passenger {
  std::string booking;
  std::string name;
  std::string serviceClass;
  std::string preference;
  std::string requestedSeat;
  std::string groupId;
};

struct PassengerFile {
  std::string fileName;
  int passengerCount;
  unsigned int seedOffset;
  bool manyGroups;
  bool manyExactSeats;
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

// Generate sequential booking number
std::string makeBooking(int number) {
  std::ostringstream stream;
  stream << "BR" << std::setw(5) << std::setfill('0') << number;
  return stream.str();
}

// Generate requested seat like 12C
std::string makeSeat(int row, char letter) {
  std::ostringstream stream;
  stream << row << letter;
  return stream.str();
}

// Generate random passenger name
std::string makeName(std::mt19937 &random) {
  const std::array<std::string, 12> maleFirstNames{{
      "Ivan",
      "Oleg",
      "Alexey",
      "Sergey",
      "Pavel",
      "Nikita",
      "Denis",
      "Roman",
      "Kirill",
      "Mikhail",
      "Stepan",
      "Timur",
  }};

  const std::array<std::string, 12> femaleFirstNames{{
      "Anna",
      "Maria",
      "Daria",
      "Elena",
      "Irina",
      "Olga",
      "Sofia",
      "Alina",
      "Yulia",
      "Vera",
      "Polina",
      "Kira",
  }};

  const std::array<std::string, 12> maleLastNames{{
      "Petrov",
      "Kim",
      "Lee",
      "Orlov",
      "Sokolov",
      "Volkov",
      "Ivanov",
      "Park",
      "Popov",
      "Kang",
      "Miller",
      "Wilson",
  }};

  const std::array<std::string, 12> femaleLastNames{{
      "Petrova",
      "Kim",
      "Lee",
      "Orlova",
      "Sokolova",
      "Volkova",
      "Ivanova",
      "Park",
      "Popova",
      "Kang",
      "Miller",
      "Wilson",
  }};

  // Random gender
  std::bernoulli_distribution maleChance = std::bernoulli_distribution{0.5};

  // Random male name index
  std::uniform_int_distribution<int> maleFirstNameDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(maleFirstNames.size()) - 1};

  // Random female name index
  std::uniform_int_distribution<int> femaleFirstNameDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(femaleFirstNames.size()) - 1};

  // Random male surname index
  std::uniform_int_distribution<int> maleLastNameDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(maleLastNames.size()) - 1};

  // Random female surname index
  std::uniform_int_distribution<int> femaleLastNameDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(femaleLastNames.size()) - 1};

  if (maleChance(random)) {
    return maleFirstNames[static_cast<std::size_t>(
               maleFirstNameDistribution(random))] +
           " " +
           maleLastNames[static_cast<std::size_t>(
               maleLastNameDistribution(random))];
  }

  return femaleFirstNames[static_cast<std::size_t>(
             femaleFirstNameDistribution(random))] +
         " " +
         femaleLastNames[static_cast<std::size_t>(
             femaleLastNameDistribution(random))];
}

// Generate requested seat from one of lab schemes
std::string makeRequestedSeat(std::mt19937 &random) {
  const std::array<char, 10> allSeatLetters{
      {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K'}};

  // Random row for all three aircraft
  std::uniform_int_distribution<int> rowDistribution =
      std::uniform_int_distribution<int>{1, 45};

  // Random seat letter from widest scheme
  std::uniform_int_distribution<int> seatDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(allSeatLetters.size()) - 1};

  return makeSeat(
      rowDistribution(random),
      allSeatLetters[static_cast<std::size_t>(seatDistribution(random))]);
}

// Generate passenger list
std::vector<Passenger> generatePassengers(const PassengerFile &file) {
  const std::array<std::string, 5> preferences{
      {"WINDOW", "AISLE", "MIDDLE", "ANY", "TOGETHER"}};

  // Fixed seed for stable generation
  std::mt19937 random = std::mt19937{20260910U + 8U + file.seedOffset};

  // Random preference index
  std::uniform_int_distribution<int> preferenceDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(preferences.size()) - 1};

  // Business class is less frequent
  std::bernoulli_distribution businessClassChance =
      std::bernoulli_distribution{0.18};

  // Some passengers request exact seats
  const double requestedSeatProbability = file.manyExactSeats ? 0.48 : 0.20;
  std::bernoulli_distribution requestedSeatChance =
      std::bernoulli_distribution{requestedSeatProbability};

  std::vector<Passenger> passengers;
  passengers.reserve(static_cast<std::size_t>(file.passengerCount));

  // Make records
  for (int index = 1; index <= file.passengerCount; index++) {
    const std::string serviceClass =
        businessClassChance(random) ? "BUSINESS" : "ECONOMY";

    std::string preference =
        preferences[static_cast<std::size_t>(preferenceDistribution(random))];
    std::string requestedSeat;
    std::string groupId;

    // Exact seat request
    if (requestedSeatChance(random) && preference != "TOGETHER") {
      requestedSeat = makeRequestedSeat(random);
    }

    const int groupPeriod = file.manyGroups ? 9 : 37;

    // Groups of 2 or 3 passengers
    if (index % groupPeriod == 0) {
      preference = "TOGETHER";
      requestedSeat = "";
      groupId = "G" + std::to_string(index / groupPeriod);
    } else if (index % groupPeriod == 1 && index > 1) {
      preference = "TOGETHER";
      requestedSeat = "";
      groupId = "G" + std::to_string((index - 1) / groupPeriod);
    } else if (index % (groupPeriod * 2) == 2 && index > 2) {
      preference = "TOGETHER";
      requestedSeat = "";
      groupId = "G" + std::to_string((index - 2) / groupPeriod);
    }

    passengers.push_back({
        makeBooking(10000 + static_cast<int>(file.seedOffset) * 1000 + index),
        makeName(random),
        serviceClass,
        preference,
        requestedSeat,
        groupId,
    });
  }

  if (passengers.size() < 12) {
    return passengers;
  }

  // Anti-random guarantee some cases
  // Specific A321 economy window request
  passengers[0].preference = "WINDOW";
  passengers[0].requestedSeat = "12A";
  passengers[0].serviceClass = "ECONOMY";

  // Specific business aisle request
  passengers[1].preference = "AISLE";
  passengers[1].requestedSeat = "2C";
  passengers[1].serviceClass = "BUSINESS";

  // Group of two passengers
  passengers[2].preference = "TOGETHER";
  passengers[2].requestedSeat = "";
  passengers[2].groupId = "G100";
  passengers[3].preference = "TOGETHER";
  passengers[3].requestedSeat = "";
  passengers[3].groupId = "G100";

  // Group of three passengers
  passengers[4].preference = "TOGETHER";
  passengers[4].requestedSeat = "";
  passengers[4].groupId = "G101";
  passengers[5].preference = "TOGETHER";
  passengers[5].requestedSeat = "";
  passengers[5].groupId = "G101";
  passengers[6].preference = "TOGETHER";
  passengers[6].requestedSeat = "";
  passengers[6].groupId = "G101";

  // Seat that does not exist on A321
  passengers[7].requestedSeat = "40K";
  passengers[7].preference = "WINDOW";
  passengers[7].serviceClass = "ECONOMY";

  // Business row with economy class mismatch
  passengers[8].requestedSeat = "3A";
  passengers[8].preference = "WINDOW";
  passengers[8].serviceClass = "ECONOMY";

  // Economy row with business class mismatch
  passengers[9].requestedSeat = "18C";
  passengers[9].preference = "AISLE";
  passengers[9].serviceClass = "BUSINESS";

  // Same requested seat for conflict
  passengers[10].requestedSeat = "12A";
  passengers[10].preference = "WINDOW";
  passengers[10].serviceClass = "ECONOMY";

  return passengers;
}

// Save passengers to CSV file
void writePassengers(const std::filesystem::path &filePath,
                     const std::vector<Passenger> &passengers) {
  std::ofstream output{filePath};

  if (!output) {
    throw std::runtime_error{"Failed to open output file: " +
                             filePath.string()};
  }

  output << "booking,name,service_class,preference,requested_seat,group_id\n";

  for (const auto &passenger : passengers) {
    output << passenger.booking << ',' << passenger.name << ','
           << passenger.serviceClass << ',' << passenger.preference << ','
           << passenger.requestedSeat << ',' << passenger.groupId << '\n';
  }
}

// Generate all passenger files
void generatePassengerFiles(const std::filesystem::path &outputDirectory) {
  const std::array<PassengerFile, 6> files{{
      {"passengers_small.txt", 36, 1U, false, false},
      {"passengers_medium.txt", 180, 2U, false, false},
      {"passengers.txt", 260, 3U, false, true},
      {"passengers_large.txt", 520, 4U, true, true},
      {"passengers_overbooked.txt", 620, 5U, true, true},
      {"passengers_groups.txt", 160, 6U, true, false},
  }};

  for (const auto &file : files) {
    writePassengers(outputDirectory / file.fileName, generatePassengers(file));
  }
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_08_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} / "08-sequence-containers";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));

    generatePassengerFiles(outputDirectory);

    std::cout << "Generated passenger files in " << outputDirectory << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
