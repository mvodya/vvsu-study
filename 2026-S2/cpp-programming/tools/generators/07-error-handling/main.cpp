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

struct Transaction {
  std::string id;
  std::string date;
  std::string type;
  double amount;
  std::string description;
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

// Generate sequential transaction ID
std::string makeId(int number) {
  std::ostringstream stream;
  stream << "TX" << std::setw(4) << std::setfill('0') << number;
  return stream.str();
}

// Generate September date
std::string makeDate(int day) {
  std::ostringstream stream;
  stream << "2026-09-" << std::setw(2) << std::setfill('0') << day;
  return stream.str();
}

// Pick amount sign by operation type
double makeAmount(const std::string &type, std::mt19937 &random) {
  // Usual expense amount
  std::uniform_real_distribution<double> expenseDistribution =
      std::uniform_real_distribution<double>{45.0, 9500.0};

  // Usual income amount
  std::uniform_real_distribution<double> incomeDistribution =
      std::uniform_real_distribution<double>{12000.0, 120000.0};

  if (type == "INCOME") {
    return incomeDistribution(random);
  }

  if (type == "TRANSFER") {
    std::bernoulli_distribution signDistribution =
        std::bernoulli_distribution{0.35};
    return signDistribution(random) ? incomeDistribution(random) * 0.4
                                    : -expenseDistribution(random);
  }

  return -expenseDistribution(random);
}

// Format amount with two decimal digits
std::string formatAmount(double amount) {
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(2) << amount;
  return stream.str();
}

// Generate valid transactions
std::vector<Transaction> generateValidTransactions(int firstId, int count) {
  const std::array<std::string, 4> types{{"INCOME", "PAYMENT", "TRANSFER",
                                          "FEE"}};
  const std::array<std::string, 14> descriptions{{
      "Salary",      "Coffee",       "Taxi",        "Groceries",
      "Book",        "Mobile",       "Internet",    "Restaurant",
      "Transfer",    "Pharmacy",     "Fuel",        "Utilities",
      "Subscription", "Cashback",
  }};

  // Fixed seed for stable generation
  std::mt19937 random = std::mt19937{20260910U + 7U +
                                     static_cast<unsigned int>(firstId)};

  // Random operation type index
  std::uniform_int_distribution<int> typeDistribution =
      std::uniform_int_distribution<int>{0, static_cast<int>(types.size()) - 1};

  // Random day in September
  std::uniform_int_distribution<int> dayDistribution =
      std::uniform_int_distribution<int>{1, 30};

  // Random description index
  std::uniform_int_distribution<int> descriptionDistribution =
      std::uniform_int_distribution<int>{0,
                                         static_cast<int>(descriptions.size()) -
                                             1};

  std::vector<Transaction> transactions;
  transactions.reserve(static_cast<std::size_t>(count));

  // Make records
  for (int index = 0; index < count; index++) {
    const std::string &type =
        types[static_cast<std::size_t>(typeDistribution(random))];

    transactions.push_back({
        makeId(firstId + index),
        makeDate(dayDistribution(random)),
        type,
        makeAmount(type, random),
        descriptions[static_cast<std::size_t>(descriptionDistribution(random))],
    });
  }

  // Anti-random guarantee some cases
  // Empty optional description
  transactions[3].description = "";

  // Positive income
  transactions[7].type = "INCOME";
  transactions[7].amount = 75000.0;
  transactions[7].description = "Salary";

  // Negative fee
  transactions[11].type = "FEE";
  transactions[11].amount = -99.0;
  transactions[11].description = "";

  return transactions;
}

// Save transactions to file
void writeTransactions(const std::filesystem::path &filePath,
                       const std::vector<Transaction> &transactions) {
  std::ofstream output{filePath};

  if (!output) {
    throw std::runtime_error{"Failed to open output file: " +
                             filePath.string()};
  }

  for (const auto &transaction : transactions) {
    output << transaction.id << ';' << transaction.date << ';'
           << transaction.type << ';' << formatAmount(transaction.amount) << ';'
           << transaction.description << '\n';
  }
}

// Save prepared lines to file
void writeLines(const std::filesystem::path &filePath,
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

// Generate valid statement file
void generateValidStatement(const std::filesystem::path &outputDirectory) {
  writeTransactions(outputDirectory / "valid_statement.txt",
                    generateValidTransactions(1001, 3000));
}

// Generate mixed statement file
void generateMixedStatement(const std::filesystem::path &outputDirectory) {
  std::vector<std::string> lines;

  for (const auto &transaction : generateValidTransactions(2001, 2500)) {
    lines.push_back(transaction.id + ";" + transaction.date + ";" +
                    transaction.type + ";" + formatAmount(transaction.amount) +
                    ";" + transaction.description);
  }

  // Empty line
  lines.insert(lines.begin() + 3, "");

  // Wrong field count
  lines.insert(lines.begin() + 80, "TX4701;2026-09-03;PAYMENT;-150.00");

  // Empty ID
  lines.insert(lines.begin() + 140, ";2026-09-04;PAYMENT;-250.00;Office");

  // Duplicate ID
  lines.insert(lines.begin() + 220, "TX2005;2026-09-05;TRANSFER;-1000.00;Transfer");

  // Invalid date
  lines.insert(lines.begin() + 360, "TX4702;2026-13-40;PAYMENT;-450.00;Groceries");

  // Unknown operation type
  lines.insert(lines.begin() + 520, "TX4703;2026-09-07;REFUND;300.00;Cashback");

  // Invalid amount format
  lines.insert(lines.begin() + 750, "TX4704;2026-09-08;PAYMENT;abc;Restaurant");

  // Amount breaks operation rule
  lines.insert(lines.begin() + 1100, "TX4705;2026-09-09;PAYMENT;120.00;Mobile");

  // Another empty line
  lines.insert(lines.begin() + 1600, "");

  // Another duplicate ID
  lines.insert(lines.begin() + 2100, "TX2400;2026-09-18;FEE;-45.00;Subscription");

  writeLines(outputDirectory / "mixed_statement.txt", lines);
}

// Generate edge cases statement file
void generateEdgeCasesStatement(const std::filesystem::path &outputDirectory) {
  const std::vector<std::string> lines{
      "TX3001;2026-09-01;INCOME;0.01;Small income",
      "TX3002;2026-09-01;PAYMENT;-0.01;Small payment",
      "TX3003;2026-09-02;TRANSFER;0.01;Small positive transfer",
      "TX3004;2026-09-02;TRANSFER;-0.01;Small negative transfer",
      "TX3005;2026-09-03;FEE;-0.01;Small fee",
      "TX3006;2026-09-04;INCOME;250000.00;Large income",
      "TX3007;2026-09-05;PAYMENT;-99999.99;Large payment",
      "TX3008;2026-09-06;FEE;-99.00;",
      "TX3009;2026-09-07;TRANSFER;0.00;Zero transfer is invalid",
      "TX3010;2026-09-08;INCOME;-100.00;Negative income is invalid",
      "TX3011;2026-09-09;FEE;20.00;Positive fee is invalid",
      "TX3012;2026-09-31;PAYMENT;-500.00;Invalid day",
      "TX3013;2026/09/10;PAYMENT;-500.00;Invalid date separator",
      "TX3014;2026-09-10;PAYMENT;-500,00;Invalid decimal separator",
      "TX3015;2026-09-10;UNKNOWN;-500.00;Unknown type",
      "TX3001;2026-09-11;PAYMENT;-50.00;Duplicate id",
  };

  writeLines(outputDirectory / "edge_cases_statement.txt", lines);
}

// Generate all statement files
void generateStatementFiles(const std::filesystem::path &outputDirectory) {
  generateValidStatement(outputDirectory);
  generateMixedStatement(outputDirectory);
  generateEdgeCasesStatement(outputDirectory);
}

// Get path from CLI args
std::filesystem::path outputDirectoryFromArguments(int argc, char *argv[]) {
  if (argc > 2) {
    throw std::runtime_error{"Usage: generate_lab_07_data [output-directory]"};
  }

  if (argc == 2) {
    return argv[1];
  }

  return std::filesystem::path{CPP_COURSE_LABS_DIR} / "07-error-handling";
}

int main(int argc, char *argv[]) {
  try {
    const auto outputDirectory =
        requireOutputDirectory(outputDirectoryFromArguments(argc, argv));

    generateStatementFiles(outputDirectory);

    std::cout << "Generated statement files in " << outputDirectory << '\n';
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << '\n';
    return 1;
  }

  return 0;
}
