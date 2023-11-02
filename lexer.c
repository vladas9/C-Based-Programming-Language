#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "include\lexer.h"

#define INITIAL_BUFFER_SIZE 64

char* read_lexeme(FILE* source) {
    int buffer_size = INITIAL_BUFFER_SIZE;
    int lexeme_length = 0;
    char* buffer = malloc(buffer_size);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    
    char ch;
    while ((ch = fgetc(source)) != EOF && (isalnum(ch) || ch == '.' || ch == '_')) {
        // Resize the buffer if necessary
        if (lexeme_length + 1 >= buffer_size) {
            buffer_size *= 2;  // Double the buffer size
            char* new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                free(buffer);
                fprintf(stderr, "Memory reallocation failed.\n");
                exit(EXIT_FAILURE);
            }
            buffer = new_buffer;
        }
        
        buffer[lexeme_length++] = ch;
    }
    
    if (ch != EOF) {
        ungetc(ch, source);  // Put back the last character read if not EOF
    }
    
    buffer[lexeme_length] = '\0'; // Null-terminate the string
    return buffer;
}

Token get_next_token(FILE *source){
    char ch;
    Token token;
    char buffer[256];
    // skip all whitespaces
    do{
        ch = fgetc(source);
    }while(isspace(ch));
    // check if end of file
    if (feof(source)) {
        token.type = TOKEN_EOF;
        return token;
    }
    switch (ch)
    {
    case '#':
        // Check if it,s a declaration
        ch = fgetc(source);
         if (ch == 'i') {
            token.type = TOKEN_INT_DECL;
            token.lexeme = "#i";
        } else if (ch == 'd') {
            token.type = TOKEN_DOUBLE_DECL;
            token.lexeme = "#d";
        } else {
            token.type = TOKEN_ERROR; //return an error if it's not a declaration
        }
        break;
    case '+':
        // Check for +=
        ch = fgetc(source);
        if (ch == '=') {
            token.type = TOKEN_PLUS_ASSIGN;
            token.lexeme = "+=";
        } else {
            ungetc(ch, source); // Put back the character that is not part of this token
            token.type = TOKEN_PLUS;
            token.lexeme = "+";
        }
        break;
    case '-':
        // Check for -=
        ch = fgetc(source);
        if (ch == '=') {
            token.type = TOKEN_MINUS_ASSIGN;
            token.lexeme = "-=";
        } else {
            ungetc(ch, source); // Put back the character that is not part of this token
            token.type = TOKEN_MINUS;
            token.lexeme = "-";
        }
        break;
    case '*':
        // Check for *=
        ch = fgetc(source);
        if (ch == '=') {
            token.type = TOKEN_MULTI_ASSIGN;
            token.lexeme = "*=";
        } else {
            ungetc(ch, source); // Put back the character that is not part of this token
            token.type = TOKEN_MULTI;
            token.lexeme = "*";
        }
        break;
    case '/':
        // Check for /=
        ch = fgetc(source);
        if (ch == '=') {
            token.type = TOKEN_DIVISION_ASSIGN;
            token.lexeme = "/=";
        } else {
            ungetc(ch, source); // Put back the character that is not part of this token
            token.type = TOKEN_DIVISION;
            token.lexeme = "/";
        }
        break;
    case '//':
        // Check for //=
        ch = fgetc(source);
        if (ch == '=') {
            token.type = TOKEN_DIV_ASSIGN;
            token.lexeme = "//=";
        } else {
            ungetc(ch, source); // Put back the character that is not part of this token
            token.type = TOKEN_DIV;
            token.lexeme = "//";
        }
        break;
    default:
        if (isdigit(ch) || ch == '.'){
            ungetc(ch, source);
                if (fscanf(source, "%255[0-9.]", buffer) == 1) {
                    if (strchr(buffer, '.') != NULL) {
                        token.type = TOKEN_DOUBLE_LITERAL;
                        token.value.d_val = atof(buffer);
                    } else {
                        token.type = TOKEN_INT_LITERAL;
                        token.value.i_val = atoi(buffer);
                    }
                    token.lexeme = strdup(buffer); // Make sure to free this later
                } else {
                    // Error handling for malformed number
                    token.type = TOKEN_ERROR;
                }
        }else{
            token.type = TOKEN_ERROR;
        }
        break;
    }

    return token;
}

int main(){
    FILE *source = fopen("example.txt", "r");
    if(!source){
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    Token token;
     do {
        token = get_next_token(source);
        // Process the token, e.g., print it or store it for the next phase
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
    
    return 0;
}