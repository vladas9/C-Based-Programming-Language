#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include "include/cJSON.h" 
#include "include/cJSON.c" 
#include "include/lexer.h"
struct Node {
    int type;
    int intValue;    //INTEGER nodes
    char* strValue;  //IDENTIFIER nodes
    struct Node* left;
    struct Node* right;
};
void syntax_error(const char* expected, const Token* found) {
    printf("Syntax error: Expected %s, found '%s' at line %d\n", expected, found->lexeme, found->line);
    exit(1);
}
struct Node* parseDeclaration();
struct Node* parseAssignment();
struct Node* parseExpression(tokens);
struct Node* parseTerm(tokens);
struct Node* parseFactor(tokens);
struct Node* parseLoop(tokens);
struct Node* parseCondition(tokens);
struct Node* parsePrint(tokens);

struct Node* parseDeclaration() {
    struct Node* declarationNode = (struct Node*)malloc(sizeof(struct Node));
    declarationNode->type = DECLARATION;

    // Parse the data type
    Token* token = getNextToken();
    if (token->type == TOKEN_INT_DECL || token->type == TOKEN_DOUBLE_DECL) {
        declarationNode->strValue = strdup(token->lexeme);
    } else {
        syntax_error("data type", token);
    }

    // Parse identifier
    token = getNextToken();
    if (token->type == TOKEN_IDENTIFIER) {
        declarationNode->left = (struct Node*)malloc(sizeof(struct Node));
        declarationNode->left->type = TOKEN_IDENTIFIER;
        declarationNode->left->strValue = strdup(token->lexeme);
    } else {
        syntax_error("identifier", token);
    }

    // Expect an assignment or end of line declaration
    token = getNextToken();
    if (token->type != TOKEN_NEW_LINE) {// need to add for implicit assignment
        syntax_error("'new line'", token);
    }

    return declarationNode;
}

struct Node* parseAssignment() {
    struct Node* assignmentNode = (struct Node*)malloc(sizeof(struct Node));
    assignmentNode->type = TOKEN_ASSIGN;

    // Parse the left-hand side (variable or identifier)
    // You should have a function for getting the next token from the lexer.
    Token* token = getNextToken();
    if (token->type == IDENTIFIER) {
        assignmentNode->strValue = strdup(token->lexeme);
    } else {
        syntax_error("identifier", token);
    }

    // Expect an '='
    token = getNextToken();
    if (token->type != EQUAL) {
        syntax_error("'='", token);
    }

    // Parse the right-hand side (expression or value)
    assignmentNode->left = parseExpression(); 

    return assignmentNode;
}


int main() {
    cJSON* root;
    cJSON* tokens;
    FILE* file = fopen("./tokens/tokens.json", "r");
    if (file == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* jsonBuffer = (char*)malloc(fileSize + 1);
    fread(jsonBuffer, 1, fileSize, file);
    fclose(file);
    jsonBuffer[fileSize] = '\0';
    
    root = cJSON_Parse(jsonBuffer);
    if (!root) {
        fprintf(stderr, "JSON is fucked\n");
        return 1;
    }
    
    tokens = cJSON_GetObjectItem(root, "tokens");

    /*something unnecesary
    for (int i = 0; i < cJSON_GetArraySize(tokens); i++) {
        cJSON* token = cJSON_GetArrayItem(tokens, i);
        char* type = cJSON_GetObjectItem(token, "type")->valuestring;
        //const int* value = cJSON_GetObjectItem(token, "value")->valuedouble;
        int line = cJSON_GetObjectItem(token, "line")->valueint;
        double value;
        if (cJSON_HasObjectItem(token, "value")) {
            if(strcmp(type, "TOKEN_INT_LITERAL")==0) value = cJSON_GetObjectItem(token, "value")->valueint;
            else value = cJSON_GetObjectItem(token, "value")->valuedouble;
            printf("Token type: %s, value: %f, line: %d\n", type, value, line);
        } else {
            const char* lexeme = cJSON_GetObjectItem(token, "lexeme")->valuestring;
            //printf("Token type: %s, lexeme: %s, line: %d\n", type, lexeme, line);
        }
        
    }*/

    
    cJSON_Delete(root);
    free(jsonBuffer);
    //printf("hello");
    return 0;
}