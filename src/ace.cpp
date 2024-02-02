#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

using namespace std;
namespace fs = std::__fs::filesystem;

// Global variables
int spSnippets;
int spLineCount = 0;
int pkbSnippets;
int pkbLineCount = 0;
int qpsSnippets;
int qpsLineCount = 0;

void processFile(const std::string &filePath,
                 std::ofstream &outFile,
                 int &totalGeneratedSnippets,
                 int &totalGeneratedLines);
void processDirectory(const std::string &directoryPath,
                      std::ofstream &outFile,
                      int &totalGeneratedSnippets, int &totalGeneratedLines);

int main(int argc, char **argv) {
  // Check if the correct number of command-line arguments (input path and output file) is provided.
  if (argc != 3) {
    std::cout << "Wrong input parameters, input should consist of "
                 "the path of a .cpp/.h file or a directory and the output txt file"
              << "\n";
    return 1; // Return an error code to indicate failure.
  }

  // Get the input path from the first argument.
  std::string inputPath = argv[1];

  // Get the output file path from the second argument.
  std::string outputTxtPath = argv[2];

  // Check if the output file has a .txt extension. If not, display an error message and return.
  if (outputTxtPath.substr(outputTxtPath.size() - 3) != "txt") {
    std::cout << "Wrong output file format, output file should be a txt file"
              << "\n";
    return 1; // Return an error code to indicate failure.
  }

  // Clear the output file by opening it in truncation mode.
  std::ofstream clearOutputFile(outputTxtPath, std::ios::trunc);
  clearOutputFile.close();

  // Open the output file for appending (to aggregate results from multiple files).
  std::ofstream outFile(outputTxtPath, std::ios::app);
  if (!outFile) {
    std::cout << "Unable to open output file: " << outputTxtPath << "\n";
    return 1; // Return an error code to indicate failure.
  }

  // Tracking total snippets and lines
  int totalGeneratedSnippets = 0;
  int totalGeneratedLines = 0;

  // Check if the input path is a directory and process it recursively.
  if (fs::is_directory(inputPath)) {
    processDirectory(inputPath,
                     outFile,
                     totalGeneratedSnippets,
                     totalGeneratedLines);
  } else {
    // If it's not a directory, assume it's a single file and process it.
    processFile(inputPath,
                outFile,
                totalGeneratedSnippets,
                totalGeneratedLines);
  }

  // Close the output file after all files have been processed.
  outFile << "Directory:" << inputPath << "\n";
  outFile << "Total Snippets: " << totalGeneratedSnippets << "\n";
  outFile << "Total lines: " << totalGeneratedLines << "\n";
  outFile << "SP Snippets: " << spSnippets << "\n";
  outFile << "SP lines: " << spLineCount << "\n";
  outFile << "PKB Snippets: " << pkbSnippets << "\n";
  outFile << "PKB lines: " << pkbLineCount << "\n";
  outFile << "QPS Snippets: " << qpsSnippets << "\n";
  outFile << "QPS lines: " << qpsLineCount << "\n";

  outFile.close();

  return 0;
}

void processDirectory(const std::string &directoryPath,
                      std::ofstream &outFile,
                      int &totalGeneratedSnippets,
                      int &totalGeneratedLines) {
  for (const auto &entry : fs::recursive_directory_iterator(directoryPath)) {
    if (entry.is_regular_file()) {
      const std::string filePath = entry.path().string();
      // Check if the file has a valid extension (.cpp, .h, or .hpp).
      std::string extension = fs::path(filePath).extension().string();
      if (extension == ".cpp" || extension == ".h" || extension == ".hpp") {
        processFile(filePath,
                    outFile,
                    totalGeneratedSnippets,
                    totalGeneratedLines);
      }
    }
  }
}

void processFile(const std::string &filePath,
                 std::ofstream &outFile,
                 int &totalGeneratedSnippets,
                 int &totalGeneratedLines) {

  bool inSp = false;
  bool inPkb = false;
  bool inQps = false;

  // Convert file path to lower case for case-insensitive comparison
  std::string lowerFilePath = filePath;
  std::transform(lowerFilePath.begin(),
                 lowerFilePath.end(),
                 lowerFilePath.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  // Check if the file path contains 'sp', 'pkb', or 'qps' in any case and update counts and snippets
  if (lowerFilePath.find("/sp/") != std::string::npos) {
    inSp = true;
  } else if (lowerFilePath.find("/pkb/") != std::string::npos) {
    inPkb = true;
  } else if (lowerFilePath.find("/qps/") != std::string::npos) {
    inQps = true;
  }

  // Open the input file for reading.
  std::ifstream inFile(filePath);
  if (!inFile) {
    std::cout << "Unable to open file: " << filePath << "\n";
    return; // Skip this file and continue with the next one.
  }

  // Initialize variables for each file.
  std::string line;
  std::string result;
  std::string generator;
  std::string intervention;
  bool aiGenStartFound = false;
  bool aiGenEndFound = false; // Track if "ai-gen end" tag is found.

  // Use a regular expression to match lines with the specified format.
  std::regex startTagRegex("ai-gen start \\(([^,]+),([^)]+)\\)");

  while (std::getline(inFile, line)) {
    std::smatch match;
    if (std::regex_search(line, match, startTagRegex)) {
      // Extract the information inside the parentheses.
      generator = match[1];
      intervention = match[2];
      generator = std::regex_replace(generator, std::regex("^\\s+|\\s+$"), "");
      intervention =
          std::regex_replace(intervention, std::regex("^\\s+|\\s+$"), "");

      aiGenStartFound = true;
      totalGeneratedSnippets++;

      if (inSp) {
        spSnippets++;
      } else if (inPkb) {
        pkbSnippets++;
      } else if (inQps) {
        qpsSnippets++;
      }

      continue;
    }

    if (line.find("ai-gen end") != std::string::npos) {
      aiGenEndFound = true; // "ai-gen end" tag is found.
      aiGenStartFound = false;
      // Write the extracted code (if any) along with fileName, 'generator', and 'intervention'.
      if (!result.empty()) {
        outFile << "File: " << filePath << "\n";
        outFile << "Generator: " << generator << "\n"; // Output 'generator'.
        outFile << "Intervention: " << intervention
                << "\n"; // Output 'intervention'.
        outFile << "Code:\n";
        outFile << result + "\n";
        outFile << "-----------------\n"; // Separator between results.
        result = "";
      }
    }

    if (aiGenStartFound) {
      result += line + "\n";
      totalGeneratedLines++;
      if (inSp) {
        spLineCount++;
      } else if (inPkb) {
        pkbLineCount++;
      } else if (inQps) {
        qpsLineCount++;
      }
    }
  }

  // Close the input file.
  inFile.close();

  // Check if "ai-gen end" tag was not found.
  if (aiGenStartFound && !aiGenEndFound) {
    std::cout << "Error: 'ai-gen end' tag not found in file: " << filePath
              << "\n";
  }
}
