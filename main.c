#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"


int main(){
    Token token = lexer();
    if(token.type == TOKEN_ERROR){
        printf("LEXER ERROR: %s %s on line %i", token.lexeme,token.error_message,token.line);
    }
}