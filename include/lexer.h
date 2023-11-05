#ifndef LEXER_H
#define LEXER_H

// All token typ
typedef enum {
    // Data type declarations
    TOKEN_INT_DECL,       // #i
    TOKEN_DOUBLE_DECL,    // #d

    // Literals
    TOKEN_INT_LITERAL,    // 123, 0, 456, etc.
    TOKEN_DOUBLE_LITERAL, // 123.456, 0.0, 0.789, etc.

    // Identifiers (variable names)
    TOKEN_IDENTIFIER,     // x, name, var123, _myVar, etc.

    // Arithmetic operators
    TOKEN_PLUS,           // +
    TOKEN_MINUS,          // -
    TOKEN_MULTI,          // *
    TOKEN_DIVISION,       // /
    TOKEN_DIV,            // //
    TOKEN_MOD,         // mod

    // Assignment operators
    TOKEN_ASSIGN,            // =
    TOKEN_PLUS_ASSIGN,       // +=
    TOKEN_MINUS_ASSIGN,      // -=
    TOKEN_MULTI_ASSIGN,      // *=
    TOKEN_DIVISION_ASSIGN,   // /=
    TOKEN_DIV_ASSIGN,        // //=

    // Comparison operators
    TOKEN_LESS,           // <
    TOKEN_GREATER,        // >
    TOKEN_EQUAL,          // ==

    // Parentheses and braces
    TOKEN_OPEN_PAREN,     // (
    TOKEN_CLOSE_PAREN,    // )
    TOKEN_OPEN_BRACE,     // {
    TOKEN_CLOSE_BRACE,    // }

    // Keywords
    TOKEN_PRINT,          // print()
    TOKEN_INPUT,          // input('message')

    // Condition and loop control
    TOKEN_CONDITION,      // condition
    TOKEN_THEN,           // ?
    TOKEN_ELSE,          // :

    // End of file and error
    TOKEN_NEW_LINE,
    TOKEN_EOF,            // End-of-file
    TOKEN_ERROR           // Error token
} TokenType;

typedef struct {
    TokenType type; // The type of the token
    char *lexeme;   // The string representation of the token from the source code
    union {
        int i_val;    // The value if it's an integer literal
        double d_val; // The value if it's a double literal
    } value;
    int line;       // The line number in the source code where this token was found
} Token;

Token get_next_token(FILE *source, int line);

#endif