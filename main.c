#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/interpreter.h"

int main(){
    Token token = lexer();
    if(token.type == TOKEN_ERROR){
        printf("LEXER ERROR: %s %s on line %i", token.lexeme,token.error_message,token.line);
    }
    printf("Start parser\n");
    Node *ast =parser();
    printf("\nStart interpreter\n\n\n");
    interpreter(ast);
}