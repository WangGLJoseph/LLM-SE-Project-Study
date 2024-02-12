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

std::string toLowercase(const std::string &str);

int countFilesInDirectory(const std::string &path);

void processDirectory(const std::string &repoName,
                      const std::string &directoryPath,
                      std::ofstream &outCodeFile,
                      std::ofstream &outDatasetFile,
                      const std::string &outChatPath,
                      int &totalGeneratedSnippets,
                      int &totalGeneratedLines);

void processFile(const std::string &repoName,
                 const std::string &directoryPath,
                 const std::string &filePath,
                 std::ofstream &outCodeFile,
                 std::ofstream &outDatasetFile,
                 const std::string &outChatPath,
                 int &totalGeneratedSnippets,
                 int &totalGeneratedLines);

std::string trimResult(const std::string &result);

void saveResult(const std::string &repoName,
                const std::string &directoryPath,
                const std::string &filePath,
                std::ofstream &outCodeFile,
                std::ofstream &outDatasetFile,
                std::string &component,
                std::string &generator,
                std::string &intervention,
                std::string &language,
                std::string &prompt,
                std::string &chatId,
                std::string &outChatIdPath,
                std::string &result);

int main(int argc, char **argv) {
    // Check if the correct number of command-line arguments (input path and output file) is provided
    if (argc != 5) {
        std::cout << "Wrong input parameters, input should consist of an input repository, "
                     "an output code txt file, an output chat directory and an output dataset file"
                  << "\n";
        return 1; // Return an error code to indicate failure
    }

    // Get the input path from the first argument
    std::string inputPath = argv[1];

    // Get the repository name from the input path
    int lastSlashPos = inputPath.find_last_of("/");
    std::string repoName;
    if (lastSlashPos != std::string::npos) {
        repoName = inputPath.substr(lastSlashPos + 1);
    }

    // Get the output txt file path from the second argument
    std::string outputCodeTxtPath = argv[2];

    // Get the output chat file path from the third argument
    std::string outputChatPath = argv[3];

    // Get the output dataset file path from the fourth argument
    std::string outputDatasetPath = argv[4];

    // Check if the output code txt file has a .txt extension
    // If not, display an error message and return
    if (outputCodeTxtPath.substr(outputCodeTxtPath.size() - 3) != "txt") {
        std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                  << "Wrong output code txt file format, output file should be a txt file"
                  << "\n";
        return 1; // Return an error code to indicate failure
    }

    // Check if the output dataset file has a .csv extension
    // If not, display an error message and return
    if (outputDatasetPath.substr(outputDatasetPath.size() - 3) != "csv") {
        std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                  << "Wrong output dataset file format, output file should be a csv file"
                  << "\n";
        return 1; // Return an error code to indicate failure
    }

    // Open the output code file for appending (to aggregate results from multiple files)
    std::ofstream outCodeFile(outputCodeTxtPath, std::ios::app);
    if (!outCodeFile) {
        std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                  << "Unable to open output code txt file: "
                  << outputCodeTxtPath
                  << "\n";
        return 1; // Return an error code to indicate failure
    }

    // Check if the output dataset file exists
    bool outDatasetFileExists = fs::exists(outputDatasetPath);

    // Open the output dataset file for appending (to aggregate results from multiple files)
    std::ofstream outDatasetFile(outputDatasetPath, std::ios::app);
    if (!outDatasetFile) {
        std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                  << "Unable to open output dataset file: "
                  << outputDatasetPath
                  << "\n";
        return 1; // Return an error code to indicate failure
    }

    if (!outDatasetFileExists) {
        outDatasetFile << "RepoName, Component, FilePath, Generator, Intervention, Language, Prompt, ChatId\n";
    }

    // Tracking total snippets and lines
    int totalGeneratedSnippets = 0;
    int totalGeneratedLines = 0;

    // Check if the input path is a directory and process it recursively
    if (fs::is_directory(inputPath)) {
        processDirectory(repoName,
                         inputPath,
                         outCodeFile,
                         outDatasetFile,
                         outputChatPath,
                         totalGeneratedSnippets,
                         totalGeneratedLines);
    } else {
        std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                  << "Wrong input path format, input path should be a directory"
                  << "\n";
        return 1; // Return an error code to indicate failure
    }

    // Close the output file after all files have been processed
    outCodeFile << "Directory:" << inputPath << "\n";
    outCodeFile << "Total Snippets: " << totalGeneratedSnippets << "\n";
    outCodeFile << "Total lines: " << totalGeneratedLines << "\n";
    outCodeFile << "SP Snippets: " << spSnippets << "\n";
    outCodeFile << "SP lines: " << spLineCount << "\n";
    outCodeFile << "PKB Snippets: " << pkbSnippets << "\n";
    outCodeFile << "PKB lines: " << pkbLineCount << "\n";
    outCodeFile << "QPS Snippets: " << qpsSnippets << "\n";
    outCodeFile << "QPS lines: " << qpsLineCount << "\n";
    outCodeFile.close();
    outDatasetFile.close();

    return 0;
}

std::string toLowercase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lowerStr;
}

int countFilesInDirectory(const std::string &dirPath) {
    try {
        return std::count_if(fs::directory_iterator(dirPath),
                             fs::directory_iterator{},
                             [](const auto& entry) {
                                 return fs::is_regular_file(entry);
                             });
    } catch (const fs::filesystem_error& e) {
        std::cout << "Unable to count files in directory: " << dirPath << "\n";
        return 0;
    }
}

void processDirectory(const std::string &repoName,
                      const std::string &directoryPath,
                      std::ofstream &outCodeFile,
                      std::ofstream &outDatasetFile,
                      const std::string &outChatPath,
                      int &totalGeneratedSnippets,
                      int &totalGeneratedLines) {
    for (const auto &entry : fs::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            const std::string filePath = entry.path().string();
            // Check if the file has a valid extension (.cpp, .h, or .hpp)
            std::string extension = fs::path(filePath).extension().string();
            if (extension == ".cpp" || extension == ".h" || extension == ".hpp") {
                processFile(repoName,
                            directoryPath,
                            filePath,
                            outCodeFile,
                            outDatasetFile,
                            outChatPath,
                            totalGeneratedSnippets,
                            totalGeneratedLines);
            }
        }
    }
}

void processFile(const std::string &repoName,
                 const std::string &directoryPath,
                 const std::string &filePath,
                 std::ofstream &outCodeFile,
                 std::ofstream &outDatasetFile,
                 const std::string &outChatPath,
                 int &totalGeneratedSnippets,
                 int &totalGeneratedLines) {

    bool inSp = false;
    bool inPkb = false;
    bool inQps = false;
    std::string component;

    // Convert file path to lower case for case-insensitive comparison
    std::string lowerFilePath = toLowercase(filePath);

    // Check if the file path contains 'sp', 'pkb', or 'qps' in any case and update counts and snippets
    // Need to manually check if the file path does not contain 'sp', 'pkb', or 'qps'
    if (lowerFilePath.find("/sp/") != std::string::npos) {
        inSp = true;
        component = "SP";
    } else if (lowerFilePath.find("/pkb/") != std::string::npos) {
        inPkb = true;
        component = "PKB";
    } else if (lowerFilePath.find("/qps/") != std::string::npos) {
        inQps = true;
        component = "QPS";
    }

    // Open the input file for reading
    std::ifstream inFile(filePath);
    if (!inFile) {
        std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                  << "Unable to open file: " << filePath << "\n";
        return; // Skip this file and continue with the next one
    }

    // Initialize variables for each file
    std::string line;
    std::string result;
    std::string generator;
    std::string intervention;
    std::string language;
    std::string prompt;
    std::string chatId;
    std::string outChatIdPath;
    // 'aiGenFound' is true when 'ai-gen start' tag is found
    // 'aiGenFound' is false when 'ai-gen end' tag is found
    bool aiGenFound = false;

    // Use a regular expression to match lines with the specified format
    // 'ai-gen start': Match the string 'ai-gen start'
    // '\s*': Match any number of whitespace characters, including spaces, tabs, and line breaks
    // '\(': Match the character '(' literally
    // '([^,]+?)': Match and capture any sequence of characters before a comma, up until the next comma
    // '([^)]+?)': Match and capture any sequence of characters until before a closing parenthesis
    // '\)': Match the character ')' literally
    std::regex startTagRegex(
            R"(ai-gen\s*start\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^)]+?)\s*\))",
            std::regex_constants::icase
    );
    // '(.*)': Match and capture any sequence of characters
    std::regex promptTagRegex(R"(prompt\s*:\s*(.*))", std::regex_constants::icase);
    std::regex endTagRegex(R"(ai-gen\s*end)", std::regex_constants::icase);

    while (std::getline(inFile, line)) {
        std::smatch match;
        // Previous 'ai-gen end' is not found
        if (aiGenFound && std::regex_search(line, match, startTagRegex) && match.size() > 3) {
            aiGenFound = false;

            result = trimResult(result);
            // Write the extracted result
            if (!result.empty()) {
                saveResult(repoName, directoryPath, filePath, outCodeFile, outDatasetFile, component, generator,
                           intervention, language, prompt, chatId, outChatIdPath, result);
            }

            std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                      << "Tag error, 'ai-gen end' tag is not found in file: " << filePath
                      << "\n";
        }

        // 'ai-gen start' is found
        if (!aiGenFound && std::regex_search(line, match, startTagRegex) && match.size() > 3) {
            aiGenFound = true;

            // Extract the information inside the parentheses
            generator = toLowercase(match[1].str());
            intervention = toLowercase(match[2].str());
            language = toLowercase(match[3].str());

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

        // 'prompt' is found
        if (aiGenFound && std::regex_search(line, match, promptTagRegex) && match.size() > 1) {
            prompt = match[1].str();
            // Remove all the whitespace characters from both ends of the string
            prompt = std::regex_replace(prompt, std::regex("^\\s+|\\s+$"), "");
            // Match the string starting with http:// or https://
            std::regex urlRegex(R"(^https?:\/\/[^\s\/$.?#].[^\s]*$)", std::regex_constants::icase);
            if (std::regex_match(prompt, urlRegex)) {
                std::smatch chatIdMatch;
                // Extract the chat id inside the URL
                std::regex chatIdRegex(R"(/p/([^/?]+))");
                if (std::regex_search(prompt, chatIdMatch, chatIdRegex) && chatIdMatch.size() > 1) {
                    chatId = chatIdMatch[1].str();
                    // Create chat id directory
                    outChatIdPath = outChatPath + "/" + chatId;
                    if (!fs::exists(outChatIdPath)) {
                        try {
                            if (!fs::create_directories(outChatIdPath)) {
                                std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                                          << "Unable to create chat id directory: " << chatId
                                          << "\n";
                            }
                        } catch (const fs::filesystem_error& e) {
                            std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                                      << "Unable to create chat id directory: " << chatId
                                      << "\n";
                        }
                    }
                } else {
                    std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                              << "Chat id error, chat id is not found in URL: " << prompt
                              << "\n";
                }
            } else {
                prompt = toLowercase(prompt);
                // Replace more than one whitespace characters with one whitespace character only
                prompt = std::regex_replace(prompt, std::regex(R"(\s+)"), " ");
            }

            continue;
        }

        // 'ai-gen end' is found
        if (aiGenFound && std::regex_search(line, match, endTagRegex) && !match.empty()) {
            aiGenFound = false;

            result = trimResult(result);
            // Write the extracted result
            if (!result.empty()) {
                saveResult(repoName, directoryPath, filePath, outCodeFile, outDatasetFile, component, generator,
                           intervention, language, prompt, chatId, outChatIdPath, result);
            }

            continue;
        }

        if (aiGenFound) {
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

    // Previous 'ai-gen end' is not found
    if (aiGenFound) {
        aiGenFound = false;

        result = trimResult(result);
        // Write the extracted result
        if (!result.empty()) {
            saveResult(repoName, directoryPath, filePath, outCodeFile, outDatasetFile, component, generator,
                       intervention, language, prompt, chatId, outChatIdPath, result);
        }

        std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                  << "Tag error, 'ai-gen end' tag is not found in file: " << filePath
                  << "\n";
    }

    // Close the input file
    inFile.close();
}

std::string trimResult(const std::string &result) {
    std::string trimmedResult = result;
    trimmedResult.erase(trimmedResult.begin(), find_if(trimmedResult.begin(), trimmedResult.end(),
                                                       [](char ch) { return !isspace(ch); }));
    trimmedResult.erase(find_if(trimmedResult.rbegin(), trimmedResult.rend(),
                                [](char ch) { return !isspace(ch); }).base(), trimmedResult.end());
    return trimmedResult;
}

void saveResult(const std::string &repoName,
                const std::string &directoryPath,
                const std::string &filePath,
                std::ofstream &outCodeFile,
                std::ofstream &outDatasetFile,
                std::string &component,
                std::string &generator,
                std::string &intervention,
                std::string &language,
                std::string &prompt,
                std::string &chatId,
                std::string &outChatIdPath,
                std::string &result) {
    if (!outChatIdPath.empty()) {
        int snippetId = countFilesInDirectory(outChatIdPath) + 1;
        std::string outSnippetPath = outChatIdPath + "/" + std::to_string(snippetId) + ".txt";
        // Open the output snippet file for appending
        std::ofstream outChatFile(outSnippetPath, std::ios::app);
        if (outChatFile) {
            outChatFile << result << "\n";
            outChatFile.close();
        } else {
            std::cout << (!repoName.empty() ? repoName + ": " : repoName)
                      << "Unable to open output snippet file: "
                      << outSnippetPath
                      << "\n";
        }
    }
    std::string trimmedFilePath = filePath;
    trimmedFilePath.erase(0, (directoryPath + "/").length());
    outCodeFile << "RepoName: " << repoName << "\n";
    outCodeFile << "File: " << trimmedFilePath << "\n";
    outCodeFile << "Generator: " << generator << "\n"; // Output 'generator'
    outCodeFile << "Intervention: " << intervention << "\n"; // Output 'intervention'
    outCodeFile << "Language: " << language << "\n"; // Output 'language'
    outCodeFile << "Prompt: " << prompt << "\n"; // Output 'prompt'
    outCodeFile << "ChatId: " << chatId << "\n"; // Output 'chatId'
    outCodeFile << "Code:\n";
    outCodeFile << result << "\n";
    outCodeFile << std::string(20, '-') + "\n"; // Separator between results
    outDatasetFile << repoName << ","
                   << component << ","
                   << trimmedFilePath << ","
                   << generator << ","
                   << intervention << ","
                   << language << ","
                   << prompt << ","
                   << chatId << "\n";
    // Reset variables
    generator = "";
    intervention = "";
    language = "";
    prompt = "";
    chatId = "";
    outChatIdPath = "";
    result = "";
}
