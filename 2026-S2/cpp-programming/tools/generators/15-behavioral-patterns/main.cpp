#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

struct RoomFile {
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

// Make empty room with border walls
std::vector<std::string> makeEmptyRoom(int width, int height) {
  std::vector<std::string> room(
      static_cast<std::size_t>(height),
      std::string(static_cast<std::size_t>(width), '.'));

  for (int x = 0; x < width; x++) {
    room[0][static_cast<std::size_t>(x)] = '#';
    room[static_cast<std::size_t>(height - 1)][static_cast<std::size_t>(x)] =
        '#';
  }

  for (int y = 0; y < height; y++) {
    room[static_cast<std::size_t>(y)][0] = '#';
    room[static_cast<std::size_t>(y)][static_cast<std::size_t>(width - 1)] =
        '#';
  }

  return room;
}

// Add random dirt cells
void addDirt(std::vector<std::string> &room, int count, std::mt19937 &random) {
  const int height = static_cast<int>(room.size());
  const int width = static_cast<int>(room[0].size());

  // Random inner x coordinate
  std::uniform_int_distribution<int> xDistribution =
      std::uniform_int_distribution<int>{1, width - 2};

  // Random inner y coordinate
  std::uniform_int_distribution<int> yDistribution =
      std::uniform_int_distribution<int>{1, height - 2};

  int added = 0;

  while (added < count) {
    const int x = xDistribution(random);
    const int y = yDistribution(random);

    if (room[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] == '.') {
      room[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = '*';
      added++;
    }
  }
}

// Add random obstacles
void addObstacles(std::vector<std::string> &room, int count,
                  std::mt19937 &random) {
  const int height = static_cast<int>(room.size());
  const int width = static_cast<int>(room[0].size());

  // Random inner x coordinate
  std::uniform_int_distribution<int> xDistribution =
      std::uniform_int_distribution<int>{1, width - 2};

  // Random inner y coordinate
  std::uniform_int_distribution<int> yDistribution =
      std::uniform_int_distribution<int>{1, height - 2};

  int added = 0;

  while (added < count) {
    const int x = xDistribution(random);
    const int y = yDistribution(random);

    if (room[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] == '.') {
      room[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = '#';
      added++;
    }
  }
}

// Put robot into first available cell
void putRobot(std::vector<std::string> &room, int x, int y) {
  room[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] = 'R';
}

// Generate random rectangular room
RoomFile generateRandomRoom(const std::string &fileName, int width, int height,
                            int dirtCount, int obstacleCount,
                            unsigned int seedOffset) {
  // Fixed seed for stable generation
  std::mt19937 random = std::mt19937{20260910U + 15U + seedOffset};

  auto room = makeEmptyRoom(width, height);

  addObstacles(room, obstacleCount, random);
  addDirt(room, dirtCount, random);
  putRobot(room, width / 2, height / 2);

  return {fileName, room};
}

// Generate all room files
std::vector<RoomFile> generateRoomFiles() {
  std::vector<RoomFile> files;

  files.push_back({
      "room.txt",
      {
          "##########",
          "#..*.....#",
          "#...R....#",
          "#.....*..#",
          "##########",
      },
  });

  files.push_back(generateRandomRoom("room_small.txt", 12, 8, 10, 8, 1U));
  files.push_back(generateRandomRoom("room_medium.txt", 22, 14, 38, 42, 2U));
  files.push_back(generateRandomRoom("room_large.txt", 34, 20, 90, 130, 3U));

  files.push_back({
      "room_maze.txt",
      {
          "####################",
          "#R...#......#......#",
          "###.#.#.####.#.##*.#",
          "#...#.#....#.#....*#",
          "#.###.####.#.#######",
          "#.....#....#.......#",
          "#.#####.##########.#",
          "#.#...#......*...#.#",
          "#.#.#.######.###.#.#",
          "#...#......#.....#*#",
          "####################",
      },
  });

  files.push_back({
      "room_unreachable.txt",
      {
          "###############",
          "#R....#.......#",
          "#.....#..***..#",
          "#.....#..***..#",
          "#.....#.......#",
          "###############",
      },
  });

  files.push_back({
      "invalid_empty.txt",
      {},
  });

  files.push_back({
      "invalid_ragged.txt",
      {
          "########",
          "#..R...#",
          "#..*#",
          "########",
      },
  });

  files.push_back({
      "invalid_no_robot.txt",
      {
          "########",
          "#..*...#",
          "#......#",
          "########",
      },
  });

  files.push_back({
      "invalid_two_robots.txt",
      {
          "########",
          "#R.*...#",
          "#...R..#",
          "########",
      },
  });

  files.push_back({
      "invalid_symbol.txt",
      {
          "########",
          "#..R...#",
          "#..x*..#",
          "########",
      },
  });

  return files;
}

// Save room to file
void writeRoom(const std::filesystem::path &filePath,
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
  const auto roomsDirectory = outputDirectory / "rooms";
  const auto invalidRoomsDirectory = outputDirectory / "invalid_rooms";

  std::filesystem::create_directories(roomsDirectory);
  std::filesystem::create_directories(invalidRoomsDirectory);

  for (const auto &file : generateRoomFiles()) {
    const auto targetDirectory = file.fileName.starts_with("invalid_")
                                     ? invalidRoomsDirectory
                                     : roomsDirectory;
    writeRoom(targetDirectory / file.fileName, file.lines);
  }
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_15_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} / "15-behavioral-patterns";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));

    generateFiles(outputDirectory);

    std::cout << "Generated room files in " << outputDirectory << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
