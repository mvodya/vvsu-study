#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

struct Passenger {
  std::string firstName;
  std::string lastName;
  int seat;
  std::string destination;
};

struct Wagon {
  int number;
  std::string type;
  std::vector<Passenger> passengers;
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

// Generate random passenger name
Passenger makePassenger(int seat, const std::string &destination,
                        std::mt19937 &random) {
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
    return {maleFirstNames[static_cast<std::size_t>(
                maleFirstNameDistribution(random))],
            maleLastNames[static_cast<std::size_t>(
                maleLastNameDistribution(random))],
            seat, destination};
  }

  return {femaleFirstNames[static_cast<std::size_t>(
              femaleFirstNameDistribution(random))],
          femaleLastNames[static_cast<std::size_t>(
              femaleLastNameDistribution(random))],
          seat, destination};
}

// Generate train wagons
std::vector<Wagon> generateTrain() {
  const std::array<std::string, 5> wagonTypes{
      {"COUPE", "PLATZKART", "SV", "SEATING", "LUX"}};
  const std::array<std::string, 9> destinations{{
      "Vladivostok",
      "Khabarovsk",
      "Ussuriysk",
      "Spassk",
      "Dalnerechensk",
      "Bikin",
      "Vyazemsky",
      "Ruzhino",
      "Sibirtsevo",
  }};

  // Fixed seed for stable generation
  std::mt19937 random = std::mt19937{20260910U + 11U};

  // Random wagon type index
  std::uniform_int_distribution<int> wagonTypeDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(wagonTypes.size()) - 1};

  // Random destination index
  std::uniform_int_distribution<int> destinationDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(destinations.size()) - 1};

  // Random passenger count for wagon
  std::uniform_int_distribution<int> passengerCountDistribution =
      std::uniform_int_distribution<int>{12, 42};

  std::vector<Wagon> wagons;
  wagons.reserve(12);

  // Make wagons
  for (int wagonNumber = 1; wagonNumber <= 12; wagonNumber++) {
    Wagon wagon{
        wagonNumber,
        wagonTypes[static_cast<std::size_t>(wagonTypeDistribution(random))],
        {},
    };

    const int passengerCount = passengerCountDistribution(random);
    wagon.passengers.reserve(static_cast<std::size_t>(passengerCount));

    // Make passengers for wagon
    for (int seat = 1; seat <= passengerCount; seat++) {
      const std::string &destination = destinations[static_cast<std::size_t>(
          destinationDistribution(random))];
      wagon.passengers.push_back(makePassenger(seat, destination, random));
    }

    wagons.push_back(wagon);
  }

  // Anti-random guarantee some cases
  // Required Kim search result
  wagons[0].passengers[1] = {"Maria", "Kim", 13, "Khabarovsk"};

  // Vladivostok passengers
  wagons[0].passengers[0] = {"Alexey", "Orlov", 12, "Vladivostok"};
  wagons[1].passengers[1] = {"Anna", "Lee", 32, "Vladivostok"};
  wagons[4].passengers[5].destination = "Vladivostok";
  wagons[7].passengers[3].destination = "Vladivostok";

  // Seats greater than 30
  wagons[1].passengers[0] = {"Ivan", "Petrov", 31, "Ussuriysk"};
  wagons[2].passengers[10].seat = 36;
  wagons[8].passengers[8].seat = 41;

  return wagons;
}

// Save train to file
void writeTrain(const std::filesystem::path &filePath,
                const std::vector<Wagon> &wagons) {
  std::ofstream output{filePath};

  if (!output) {
    throw std::runtime_error{"Failed to open output file: " +
                             filePath.string()};
  }

  for (const auto &wagon : wagons) {
    output << "WAGON " << wagon.number << ' ' << wagon.type << '\n';

    for (const auto &passenger : wagon.passengers) {
      output << "P " << passenger.firstName << ' ' << passenger.lastName << ' '
             << passenger.seat << ' ' << passenger.destination << '\n';
    }
  }
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_11_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} / "11-iterators-ranges";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));
    const auto outputFile = outputDirectory / "train.txt";

    writeTrain(outputFile, generateTrain());

    std::cout << "Generated " << outputFile << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
