#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

struct LogRecord {
  std::string ip;
  std::string method;
  std::string path;
  int status;
  int bytes;
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

// Generate IP from documentation ranges
std::string makeIp(int number) {
  const std::array<std::string, 3> networks{
      {"192.0.2.", "198.51.100.", "203.0.113."}};

  const auto &network =
      networks[static_cast<std::size_t>(number % networks.size())];
  const int host = 1 + number % 254;

  return network + std::to_string(host);
}

// Generate access log records
std::vector<LogRecord> generateLogRecords() {
  constexpr int recordCount = 8000;

  const std::array<std::string, 5> methods{
      {"GET", "POST", "PUT", "DELETE", "PATCH"}};

  const std::array<std::string, 24> paths{{
      "/",
      "/catalog",
      "/catalog/search",
      "/catalog/item",
      "/login",
      "/logout",
      "/profile",
      "/cart",
      "/cart/add",
      "/cart/remove",
      "/checkout",
      "/orders",
      "/orders/view",
      "/api/products",
      "/api/cart",
      "/api/login",
      "/api/orders",
      "/assets/app.js",
      "/assets/app.css",
      "/images/logo.png",
      "/admin",
      "/admin/users",
      "/health",
      "/metrics",
  }};

  const std::array<int, 9> statuses{
      {200, 201, 204, 301, 400, 401, 403, 404, 500}};

  const std::array<std::string, 10> hotClients{{
      "203.0.113.15",
      "198.51.100.4",
      "192.0.2.77",
      "203.0.113.42",
      "198.51.100.88",
      "192.0.2.130",
      "203.0.113.101",
      "198.51.100.210",
      "192.0.2.19",
      "203.0.113.200",
  }};

  const std::array<std::string, 8> hotPaths{{
      "/catalog",
      "/login",
      "/cart",
      "/api/products",
      "/checkout",
      "/assets/app.js",
      "/profile",
      "/orders",
  }};

  // Fixed seed for stable generation
  std::mt19937 random = std::mt19937{20260910U + 9U};

  // Random method index
  std::uniform_int_distribution<int> methodDistribution =
      std::uniform_int_distribution<int>{0,
                                         static_cast<int>(methods.size()) - 1};

  // Random path index
  std::uniform_int_distribution<int> pathDistribution =
      std::uniform_int_distribution<int>{0, static_cast<int>(paths.size()) - 1};

  // Random status index
  std::uniform_int_distribution<int> statusDistribution =
      std::uniform_int_distribution<int>{0,
                                         static_cast<int>(statuses.size()) - 1};

  // Random generated IP index
  std::uniform_int_distribution<int> ipDistribution =
      std::uniform_int_distribution<int>{0, 1600};

  // Random hot client index
  std::uniform_int_distribution<int> hotClientDistribution =
      std::uniform_int_distribution<int>{
          0, static_cast<int>(hotClients.size()) - 1};

  // Random hot path index
  std::uniform_int_distribution<int> hotPathDistribution =
      std::uniform_int_distribution<int>{0,
                                         static_cast<int>(hotPaths.size()) - 1};

  // Most requests are successful
  std::bernoulli_distribution successChance = std::bernoulli_distribution{0.78};

  // Hot clients appear often
  std::bernoulli_distribution hotClientChance =
      std::bernoulli_distribution{0.42};

  // Hot paths appear often
  std::bernoulli_distribution hotPathChance = std::bernoulli_distribution{0.55};

  // Response size in bytes
  std::uniform_int_distribution<int> bytesDistribution =
      std::uniform_int_distribution<int>{120, 180000};

  std::vector<LogRecord> records;
  records.reserve(recordCount);

  // Make records
  for (int index = 0; index < recordCount; index++) {
    const std::string ip = hotClientChance(random)
                               ? hotClients[static_cast<std::size_t>(
                                     hotClientDistribution(random))]
                               : makeIp(ipDistribution(random));

    const std::string path =
        hotPathChance(random)
            ? hotPaths[static_cast<std::size_t>(hotPathDistribution(random))]
            : paths[static_cast<std::size_t>(pathDistribution(random))];

    const std::string method =
        methods[static_cast<std::size_t>(methodDistribution(random))];

    const int status =
        successChance(random)
            ? 200
            : statuses[static_cast<std::size_t>(statusDistribution(random))];

    records.push_back({ip, method, path, status, bytesDistribution(random)});
  }

  // Anti-random guarantee some cases
  // Frequent catalog access from one client
  for (int index = 0; index < 120; index++) {
    records[static_cast<std::size_t>(index)] = {"203.0.113.15", "GET",
                                                "/catalog", 200, 13420};
  }

  // Frequent failed login attempts
  for (int index = 120; index < 180; index++) {
    records[static_cast<std::size_t>(index)] = {"198.51.100.4", "POST",
                                                "/login", 401, 821};
  }

  // Server errors for profile page
  for (int index = 180; index < 220; index++) {
    records[static_cast<std::size_t>(index)] = {"203.0.113.42", "GET",
                                                "/profile", 500, 256};
  }

  // Rare DELETE method
  records[220] = {"192.0.2.77", "DELETE", "/cart/remove", 204, 180};

  // Rare PATCH method
  records[221] = {"192.0.2.78", "PATCH", "/profile", 200, 640};

  // Shuffle records to look like a usual mixed log
  std::ranges::shuffle(records, random);

  return records;
}

// Save access log to file
void writeAccessLog(const std::filesystem::path &filePath,
                    const std::vector<LogRecord> &records) {
  std::ofstream output{filePath};

  if (!output) {
    throw std::runtime_error{"Failed to open output file: " +
                             filePath.string()};
  }

  for (const auto &record : records) {
    output << record.ip << ' ' << record.method << ' ' << record.path << ' '
           << record.status << ' ' << record.bytes << '\n';
  }
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_09_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} /
         "09-associative-containers";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));
    const auto outputFile = outputDirectory / "access.log";

    writeAccessLog(outputFile, generateLogRecords());

    std::cout << "Generated " << outputFile << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
