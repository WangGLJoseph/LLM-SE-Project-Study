#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

#define UNUSED(x) (void)(x);

enum class StatementType { READ, PRINT, ASSIGN, CALL, WHILE, IF, NONE };

enum class EntityType { PROCEDURE, STATEMENT, VARIABLE, CONSTANT, NONE };

enum class AbstractionType {
    FOLLOWS,
    FOLLOWS_STAR,
    PARENT,
    PARENT_STAR,
    USES,
    MODIFIES,
    CALLS,
    CALLS_STAR,
    NEXT,
    NEXT_STAR,
    AFFECTS,
    AFFECTS_STAR
};

enum class TokenType {
    AND,               /* && */
    ASTERISK,          /* * */
    COMMA,             /* , */
    DOT,               /* . */
    DOUBLE_EQUAL,      /* == */
    DOUBLE_QUOTES,     /* " */
    EQUAL,             /* = */
    EXCLAMATION,       /* ! */
    GTE,               /* >= */
    HYPHEN,            /* - */
    LEFT_CHEVRON,      /* < */
    LEFT_CURLY,        /* { */
    LEFT_PARENTHESIS,  /* ( */
    LTE,               /* <= */
    NEQ,               /* != */
    OR,                /* || */
    PERCENT,           /* % */
    PLUS,              /* + */
    RIGHT_CHEVRON,     /* > */
    RIGHT_CURLY,       /* } */
    RIGHT_PARENTHESIS, /* ) */
    SEMICOLON,         /* ; */
    SLASH,             /* / */
    UNDERSCORE,        /* _ */
    END_OF_FILE,       /* \0 */
    INVALID,
    NUMBER,
    STRING
};

// ai-gen start(gpt, 0, e)
// prompt: https://platform.openai.com/playground/p/pi7zoOh99k14rCpENsFTiPEQ?model=gpt-4&mode=chat
struct CharHash {
    size_t operator()(char c) const { return std::hash<char>()(c); }
};

struct StringHash {
    size_t operator()(const std::string& s) const {
        return std::hash<std::string>()(s);
    }
};

struct StatementTypeHash {
    size_t operator()(const StatementType& k) const {
        return std::hash<int>()(static_cast<int>(k));
    }
};

struct EntityTypeHash {
    size_t operator()(const EntityType& k) const {
        return std::hash<int>()(static_cast<int>(k));
    }
};

struct AbstractionTypeHash {
    size_t operator()(const AbstractionType& k) const {
        return std::hash<int>()(static_cast<int>(k));
    }
};

struct TokenTypeHash {
    size_t operator()(const TokenType& k) const {
        return std::hash<int>()(static_cast<int>(k));
    }
};
// ai-gen end

// ai-gen start (copilot, 2, e)
// prompt: used copilot

inline std::unordered_map<const StatementType, const std::string,
                          StatementTypeHash>
    statement_type_to_string = {
        {StatementType::READ, "read"},     {StatementType::PRINT, "print"},
        {StatementType::ASSIGN, "assign"}, {StatementType::CALL, "call"},
        {StatementType::WHILE, "while"},   {StatementType::IF, "if"},
        {StatementType::NONE, "none"}};

inline std::unordered_map<const EntityType, const std::string, EntityTypeHash>
    entity_type_to_string = {{EntityType::PROCEDURE, "procedure"},
                             {EntityType::STATEMENT, "stmt"},
                             {EntityType::VARIABLE, "variable"},
                             {EntityType::CONSTANT, "constant"},
                             {EntityType::NONE, "none"}};

inline std::unordered_map<const AbstractionType, const std::string,
                          AbstractionTypeHash>
    abstraction_type_to_string = {{AbstractionType::FOLLOWS, "Follows"},
                                  {AbstractionType::FOLLOWS_STAR, "Follows*"},
                                  {AbstractionType::PARENT, "Parent"},
                                  {AbstractionType::PARENT_STAR, "Parent*"},
                                  {AbstractionType::USES, "Uses"},
                                  {AbstractionType::MODIFIES, "Modifies"},
                                  {AbstractionType::CALLS, "Calls"},
                                  {AbstractionType::CALLS_STAR, "Calls*"},
                                  {AbstractionType::NEXT, "Next"},
                                  {AbstractionType::NEXT_STAR, "Next*"},
                                  {AbstractionType::AFFECTS, "Affects"},
                                  {AbstractionType::AFFECTS_STAR, "Affects*"}};

inline std::unordered_map<const TokenType, const std::string, TokenTypeHash>
    token_type_to_string = {{TokenType::AND, "And"},
                            {TokenType::ASTERISK, "Asterisk"},
                            {TokenType::COMMA, "Comma"},
                            {TokenType::DOT, "Dot"},
                            {TokenType::DOUBLE_EQUAL, "DoubleEqual"},
                            {TokenType::DOUBLE_QUOTES, "DoubleQuotes"},
                            {TokenType::END_OF_FILE, "EndOfFile"},
                            {TokenType::EQUAL, "Equal"},
                            {TokenType::EXCLAMATION, "Exclamation"},
                            {TokenType::GTE, "GreaterThanEqual"},
                            {TokenType::HYPHEN, "Hyphen"},
                            {TokenType::INVALID, "Invalid"},
                            {TokenType::LEFT_CHEVRON, "LeftChevron"},
                            {TokenType::LEFT_CURLY, "LeftCurly"},
                            {TokenType::LEFT_PARENTHESIS, "LeftParenthesis"},
                            {TokenType::LTE, "LessThanEqual"},
                            {TokenType::NEQ, "NotEqual"},
                            {TokenType::NUMBER, "Number"},
                            {TokenType::OR, "Or"},
                            {TokenType::PERCENT, "Percent"},
                            {TokenType::PLUS, "Plus"},
                            {TokenType::RIGHT_CHEVRON, "RightChevron"},
                            {TokenType::RIGHT_CURLY, "RightCurly"},
                            {TokenType::RIGHT_PARENTHESIS, "RightParenthesis"},
                            {TokenType::SEMICOLON, "Semicolon"},
                            {TokenType::SLASH, "Slash"},
                            {TokenType::STRING, "String"},
                            {TokenType::UNDERSCORE, "Underscore"}};

// ai-gen end

inline std::ostream& operator<<(std::ostream& os,
                                const StatementType& statement_type) {
    os << statement_type_to_string[statement_type];
    return os;
}

// ai-gen start (copilot, 2, c)
// prompt:

inline std::ostream& operator<<(std::ostream& os,
                                const EntityType& entity_type) {
    os << entity_type_to_string[entity_type];
    return os;
}

// ai-gen end

// ai-gen start (gpt, 0, e)
// prompt: https://platform.openai.com/playground/p/soFN7wfISkrxYtflFA8nfCHH?model=gpt-4&mode=chat
inline std::ostream& operator<<(std::ostream& os,
                                const AbstractionType& abstraction_type) {
    os << abstraction_type_to_string[abstraction_type];
    return os;
}
// ai-gen end

// ai-gen start (gpt, 0, e)
// prompt: https://platform.openai.com/playground/p/soFN7wfISkrxYtflFA8nfCHH?model=gpt-4&mode=chat

inline std::ostream& operator<<(std::ostream& os, const TokenType& token_type) {
    os << token_type_to_string[token_type];
    return os;
}
