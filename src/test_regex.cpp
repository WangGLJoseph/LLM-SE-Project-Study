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
    const std::regex pattern(
            R"(ai-gen\s*start\s*\(\s*([^,]+?)\s*,\s*([^,]+?)\s*,\s*([^)]+?)\s*\))",
            std::regex_constants::icase
    );

    const std::string test_strings[] = {
            "// ai-gen start (copilot, 0, c)",
            "////ai-gen start (copilot,1,c)",
            "     // ai-gen start (copilot,2,c)",
            "///ai-genstart(copilot,0,e)",
            "//////AI-gen    start(copilot,1,e)",
            "//AI-gen STarT(copilot,2,e)",
            "//// ai-gen start(gpt,0,e)",
            "//         ai-gen start(gpt,1,e)   ",
            "//         ai-gen start(gpt,2,e)   ",
            "//ai-gen start (         copilot    , 0   ,       e  )",
            "// ai-gen start (         copilot    , 1   ,       c  )",
            "         ai-gen start(         copilot    , 2   ,       c  )   ",
            "     // ai-gen start (         gpt    , 0   ,       e  )   ",
            "     // ai-gen start (         gpt    , 1   ,       c  )",
            "//         ai-gen start(         gpt    , 2   ,       c  )   ",
            "//         ai-gen start(         gpt    , 0   ,       c  )))))))",
            "//         ai-gen start(         gpt    , 1   ,       c  )  test ",
            "// ai-gen start(         gpt    , 2  ,       e)!@#$%^&*() test ",
            "// ai-gen start(         copilot    , 0   ,    e  )    !@#$%^&*() test"
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
            std::cout << "Match found" << std::endl;
        } else {
            std::cout << "No match" << std::endl;
        }
        std::cout << std::string(20, '=') << std::endl;
    }
}

void testPromptTagRegex() {
    const std::regex pattern(R"(prompt\s*:\s*(.*))", std::regex_constants::icase);

    const std::string test_strings[] = {
            "////prompt: used copilot",
            "//// prompt: used    copilot",
            "///prompt    : used copilot",
            "// prompt:       USed copilot",
            "/// prompt:used copilot",
            "// prompt: used copilot     ",
            "////     prompt: used copilot     ",
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
            "//prompt: https://www.google.com/",
            "////prompt: https://www.google.com",
            "/// prompt    : https://www.google.com/",
            "//// prompt:       https://www.google.com/",
            "//prompt:https://www.google.com/",
            "// prompt: https://www.google.com/     ",
            "///     prompt: https://www.google.com/     ",
            "// prompt: https://www.google.com/",
            "// prompt: https://www.google.com",
            "// prompt    : https://www.google.com/",
            "//prompt:https://www.google.com/",
            "// prompt:https://www.google.com/",
            "//prompt:    https://www.google.com/",
            "// prompt:    https://www.google.com/",
            "     // prompt: https://www.google.com/",
            "     /// prompt: https://www.google.com/      ",
            "     //// PromPt: https://www.google.com/      ",
            "     // prompt: https://www.google.com/ //",
            "     // prompt: https://www.google.com/ !@#$%^&*() test ",
            "     /// prompt: https://www.google.com/ test"
    };

    for (const auto& test_str : test_strings) {
        std::smatch match;
        std::cout << "Test string: " << test_str << std::endl;
        if (std::regex_search(test_str, match, pattern) && match.size() > 1) {
            std::string prompt = match[1].str();
            // Remove all the whitespace characters from both ends of the string
            prompt = std::regex_replace(prompt, std::regex("^\\s+|\\s+$"), "");
            const std::regex urlRegex(R"(https?:\/\/[^\s\/$.?#].[^\s]*)", std::regex_constants::icase);
            std::smatch urlMatch;
            if (std::regex_search(prompt, urlMatch, urlRegex) && !urlMatch.empty()) {
                prompt = urlMatch[0].str();
            } else {
                prompt = toLowercase(prompt);
                // Replace more than one whitespace characters with one whitespace character only
                prompt = std::regex_replace(prompt, std::regex(R"(\s+)"), " ");
            }
            std::cout << "Prompt: " << prompt << std::endl;
            std::cout << "Match found" << std::endl;
        } else {
            std::cout << "No match" << std::endl;
        }
        std::cout << std::string(20, '=') << std::endl;
    }
}

void testEndTagRegex() {
    const std::regex pattern(R"(ai-gen\s*end)", std::regex_constants::icase);

    const std::string test_strings[] = {
            "//ai-gen end",
            "///  ai-gen    end",
            "////ai-gen end     ",
            "//     ai-gen end",
            "////     ai-gen end     ",
            "//ai-gen end",
            "// ai-gen end",
            "// ai-gen    end",
            "// ai-gen end     ",
            "    //ai-gen end",
            "    // ai-gen end",
            "    //     ai-gen end     ",
            "    //ai-gen end!@#$%^&*() test ",
            "    // ai-gen end test",
            "    //     ai-gen end  !@#$%^&*() test    "
    };

    for (const auto& test_str : test_strings) {
        std::smatch match;
        std::cout << "Test string: " << test_str << std::endl;
        if (std::regex_search(test_str, match, pattern) && !match.empty()) {
            // Replace more than one whitespace characters with one whitespace character only
            std::cout << std::regex_replace(toLowercase(match[0].str()), std::regex(R"(\s+)"), " ") << std::endl;
            std::cout << "Match found" << std::endl;
        } else {
            std::cout << "No match" << std::endl;
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
