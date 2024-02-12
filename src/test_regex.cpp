#include <iostream>
#include <regex>
#include <string>

std::string toLowercase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lowerStr;
}

void testStartTagRegex() {
    std::regex pattern(
            R"(ai-gen\s*start\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^)]+?)\s*\))",
            std::regex_constants::icase
    );

    std::string test_strings[] = {
            "ai-gen start (copilot, 0, c)",
            "ai-gen start (copilot,1,c)",
            "     // ai-gen start (copilot,2,c)",
            "//ai-genstart(copilot,0,e)",
            "//AI-gen    start(copilot,1,e)",
            "//AI-gen STarT(copilot,2,e)",
            "ai-gen start(gpt,0,e)",
            "         ai-gen start(gpt,1,e)   ",
            "//         ai-gen start(gpt,2,e)   ",
            "ai-gen start (         copilot    , 0   ,       e  )",
            "ai-gen start (         copilot    , 1   ,       c  )",
            "         ai-gen start(         copilot    , 2   ,       c  )   ",
            "     // ai-gen start (         gpt    , 0   ,       e  )   ",
            "     // ai-gen start (         gpt    , 1   ,       c  )",
            "//         ai-gen start(         gpt    , 2   ,       c  )   "
    };

    for (const auto& test_str : test_strings) {
        std::smatch match;
        std::cout << "Test string: " << test_str << std::endl;
        if (std::regex_search(test_str, match, pattern) && match.size() > 3) {
            std::string generator = toLowercase(match[1].str());
            std::string intervention = toLowercase(match[2].str());
            std::string language = toLowercase(match[3].str());
            std::cout << "Generator: " << generator << std::endl;
            std::cout << "Intervention: " << intervention << std::endl;
            std::cout << "Language: " << language << std::endl;
            std::cout << "Match found: " << test_str << std::endl;
        } else {
            std::cout << "No match: " << test_str << std::endl;
        }
        std::cout << std::string(20, '=') << std::endl;
    }
}


void testPromptTagRegex() {
    std::regex pattern(R"(prompt\s*:\s*(.*))", std::regex_constants::icase);

    std::string test_strings[] = {
            "prompt: used copilot",
            "prompt: used    copilot",
            "prompt    : used copilot",
            "prompt:       USed copilot",
            "prompt:used copilot",
            "prompt: used copilot     ",
            "     prompt: used copilot     ",
            "// prompt: used copilot",
            "// prompt: used     coPilot",
            "// prompt    : used Copilot",
            "//prompt:used copilot",
            "// prompt:used copilot",
            "//prompt:    used copilot",
            "// prompt:    used copilot",
            "     // prompt: used copilot",
            "     // prompt: used copilot      ",
            "     // PromPt: used copilot      ",
            "prompt: https://www.google.com/",
            "prompt: https://www.google.com",
            "prompt    : https://www.google.com/",
            "prompt:       https://www.google.com/",
            "prompt:https://www.google.com/",
            "prompt: https://www.google.com/     ",
            "     prompt: https://www.google.com/     ",
            "// prompt: https://www.google.com/",
            "// prompt: https://www.google.com",
            "// prompt    : https://www.google.com/",
            "//prompt:https://www.google.com/",
            "// prompt:https://www.google.com/",
            "//prompt:    https://www.google.com/",
            "// prompt:    https://www.google.com/",
            "     // prompt: https://www.google.com/",
            "     // prompt: https://www.google.com/      ",
            "     // PromPt: https://www.google.com/      "
    };

    for (const auto& test_str : test_strings) {
        std::smatch match;
        std::cout << "Test string: " << test_str << std::endl;
        if (std::regex_search(test_str, match, pattern) && match.size() > 1) {
            std::string prompt = match[1].str();
            // Remove all the whitespace characters from both ends of the string
            prompt = std::regex_replace(prompt, std::regex("^\\s+|\\s+$"), "");
            std::regex urlRegex(R"(^https?:\/\/[^\s\/$.?#].[^\s]*$)", std::regex_constants::icase);
            if (!std::regex_match(prompt, urlRegex)) {
                prompt = toLowercase(prompt);
                // Replace more than one whitespace characters with one whitespace character only
                prompt = std::regex_replace(prompt, std::regex(R"(\s+)"), " ");
            }
            std::cout << "Prompt: " << prompt << std::endl;
            std::cout << "Match found: " << test_str << std::endl;
        } else {
            std::cout << "No match: " << test_str << std::endl;
        }
        std::cout << std::string(20, '=') << std::endl;
    }
}

void testEndTagRegex() {
    std::regex pattern(R"(ai-gen\s*end)", std::regex_constants::icase);

    std::string test_strings[] = {
            "ai-gen end",
            "ai-gen    end",
            "ai-gen end     ",
            "     ai-gen end",
            "     ai-gen end     ",
            "//ai-gen end",
            "// ai-gen end",
            "// ai-gen    end",
            "// ai-gen end     ",
            "    //ai-gen end",
            "    // ai-gen end",
            "    //     ai-gen end     "
    };

    for (const auto& test_str : test_strings) {
        std::smatch match;
        std::cout << "Test string: " << test_str << std::endl;
        if (std::regex_search(test_str, match, pattern) && !match.empty()) {
            std::cout << "Match found: " << test_str << std::endl;
        } else {
            std::cout << "No match: " << test_str << std::endl;
        }
        std::cout << std::string(20, '=') << std::endl;
    }
}

int main() {
    testStartTagRegex();
    std::cout << "\n\n" << std::endl;
    testPromptTagRegex();
    std::cout << "\n\n" << std::endl;
    testEndTagRegex();
    return 0;
}
