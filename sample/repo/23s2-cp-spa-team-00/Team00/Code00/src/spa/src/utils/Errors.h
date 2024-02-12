#pragma once

#include <iostream>
#include <string>

class Exception : public std::exception {
    std::string msg;

 public:
    explicit Exception(const std::string& err) noexcept
        : msg("Exception: " + err) {}
    Exception(const std::string& err, const size_t& pos) noexcept
        : msg("Exception [" + std::to_string(pos) + "]: " + err) {}
    Exception(const std::string& err, const std::string& value) noexcept
        : msg("Exception [" + value + "]: " + err) {}
    Exception(const Exception& other) noexcept = default;
    Exception& operator=(const Exception& other) noexcept = default;

    const char* what() const noexcept override { return msg.c_str(); }
};

// ai-gen start (copilot, 2, e)
// prompt: used copilot
class FileError final : public Exception {
 public:
    explicit FileError(const std::string& err) noexcept
        : Exception("File error: " + err) {}
    FileError(const std::string& err, const size_t& pos) noexcept
        : Exception("File error: " + err, pos) {}
    FileError(const std::string& err, const std::string& value) noexcept
        : Exception("File error: " + err, value) {}
};
// ai-gen end

// ai-gen start (gpt, 0, e)
// prompt: https://platform.openai.com/playground/p/NOluedVgW5yWh05DioM75jSz?model=gpt-4&mode=chat
class SyntaxError final : public Exception {
 public:
    explicit SyntaxError(const std::string& err) noexcept
        : Exception("Syntax error: " + err) {}
    SyntaxError(const std::string& err, const size_t& pos) noexcept
        : Exception("Syntax error: " + err, pos) {}
    SyntaxError(const std::string& err, const std::string& value) noexcept
        : Exception("Syntax error: " + err, value) {}
};
// ai-gen end

// ai-gen start (copilot, 1, c)
// prompt: used copilot

class SemanticError final : public Exception {
 public:
    explicit SemanticError(const std::string& err) noexcept
        : Exception("Semantic error: " + err) {}
    SemanticError(const std::string& err, const size_t& pos) noexcept
        : Exception("Semantic error: " + err, pos) {}
    SemanticError(const std::string& err, const std::string& value) noexcept
        : Exception("Semantic error: " + err, value) {}
};

// ai-gen stop

class SourceParsingError final : public Exception {
 public:
    explicit SourceParsingError(const std::string& err) noexcept
        : Exception("Source parsing error: " + err) {}
    SourceParsingError(const std::string& err, const size_t& pos) noexcept
        : Exception("Source parsing error: " + err, pos) {}
    SourceParsingError(const std::string& err,
                       const std::string& value) noexcept
        : Exception("Source parsing error: " + err, value) {}
};

// ai-gen start(copilot, 1, e)
// prompt: used copilot
class SystemError final : public Exception {
 public:
    explicit SystemError(const std::string& err) noexcept
        : Exception("System error: " + err) {}
    SystemError(const std::string& err, const size_t& pos) noexcept
        : Exception("System error: " + err, pos) {}
    SystemError(const std::string& err, const std::string& value) noexcept
        : Exception("System error: " + err, value) {}
};

// ai-gen end
