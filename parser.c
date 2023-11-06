#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include "include/cJSON.h" 
#include "include/cJSON.c" 
#include "include/lexer.h"
#include "include/parser.h"

struct Node{
    NodeType type;
    int intValue;    //INTEGER nodes
    char* strValue;  //IDENTIFIER nodes
    struct Node* left;
    struct Node* right;
};

void syntax_error(const char* expected, const Token found) {
    printf("Syntax error: Expected %s, found '%s' at line %d\n", expected, found.lexeme, found.line);
    exit(1);
}
TokenType mapJsonToEnum(const char* jsonType){
    if (strcmp(jsonType, "TOKEN_INT_DECL") == 0) return TOKEN_INT_DECL;
    if (strcmp(jsonType, "TOKEN_DOUBLE_DECL") == 0) return TOKEN_DOUBLE_DECL;
    if (strcmp(jsonType, "TOKEN_INT_LITERAL") == 0) return TOKEN_INT_LITERAL;
    if (strcmp(jsonType, "TOKEN_DOUBLE_LITERAL") == 0) return TOKEN_DOUBLE_LITERAL;
    if (strcmp(jsonType, "TOKEN_IDENTIFIER") == 0) return TOKEN_IDENTIFIER;
    if (strcmp(jsonType, "TOKEN_PLUS") == 0) return TOKEN_PLUS;
    if (strcmp(jsonType, "TOKEN_MINUS") == 0) return TOKEN_MINUS;
    if (strcmp(jsonType, "TOKEN_MULTI") == 0) return TOKEN_MULTI;
    if (strcmp(jsonType, "TOKEN_DIVISION") == 0) return TOKEN_DIVISION;
    if (strcmp(jsonType, "TOKEN_DIV") == 0) return TOKEN_DIV;
    if (strcmp(jsonType, "TOKEN_MOD") == 0) return TOKEN_MOD;
    if (strcmp(jsonType, "TOKEN_ASSIGN") == 0) return TOKEN_ASSIGN;
    if (strcmp(jsonType, "TOKEN_PLUS_ASSIGN") == 0) return TOKEN_PLUS_ASSIGN;
    if (strcmp(jsonType, "TOKEN_MINUS_ASSIGN") == 0) return TOKEN_MINUS_ASSIGN;
    if (strcmp(jsonType, "TOKEN_MULTI_ASSIGN") == 0) return TOKEN_MULTI_ASSIGN;
    if (strcmp(jsonType, "TOKEN_DIVISION_ASSIGN") == 0) return TOKEN_DIVISION_ASSIGN;
    if (strcmp(jsonType, "TOKEN_DIV_ASSIGN") == 0) return TOKEN_DIV_ASSIGN;
    if (strcmp(jsonType, "TOKEN_LESS") == 0) return TOKEN_LESS;
    if (strcmp(jsonType, "TOKEN_GREATER") == 0) return TOKEN_GREATER;
    if (strcmp(jsonType, "TOKEN_EQUAL") == 0) return TOKEN_EQUAL;
    if (strcmp(jsonType, "TOKEN_OPEN_PAREN") == 0) return TOKEN_OPEN_PAREN;
    if (strcmp(jsonType, "TOKEN_CLOSE_PAREN") == 0) return TOKEN_CLOSE_PAREN;
    if (strcmp(jsonType, "TOKEN_OPEN_BRACE") == 0) return TOKEN_OPEN_BRACE;
    if (strcmp(jsonType, "TOKEN_CLOSE_BRACE") == 0) return TOKEN_CLOSE_BRACE;
    if (strcmp(jsonType, "TOKEN_PRINT") == 0) return TOKEN_PRINT;
    if (strcmp(jsonType, "TOKEN_INPUT") == 0) return TOKEN_INPUT;
    if (strcmp(jsonType, "TOKEN_CONDITION") == 0) return TOKEN_CONDITION;
    if (strcmp(jsonType, "TOKEN_THEN") == 0) return TOKEN_THEN;
    if (strcmp(jsonType, "TOKEN_ELSE") == 0) return TOKEN_ELSE;
    if (strcmp(jsonType, "TOKEN_EOF") == 0) return TOKEN_EOF;
    if (strcmp(jsonType, "TOKEN_ERROR") == 0) return TOKEN_ERROR;
    if (strcmp(jsonType, "TOKEN_NEW_LINE") == 0) return TOKEN_NEW_LINE;
    return TOKEN_ERROR;
}
struct Node* parseDeclaration(cJSON* tokens, int *currentTokenIndex);//ready
struct Node* parseAssignment(cJSON* tokens, int *currentTokenIndex);//in process...
struct Node* parseExpression();
struct Node* parseTerm();
struct Node* parseFactor();
struct Node* parseLoop();
struct Node* parseCondition();
struct Node* parsePrint();
Token getCurrentToken(cJSON* tokens, int *currentTokenIndex){
    cJSON* token = cJSON_GetArrayItem(tokens, *currentTokenIndex);
    Token currentToken;
    const char* jsonType = cJSON_GetObjectItem(token, "type")->valuestring;
    currentToken.type = mapJsonToEnum(jsonType);
    currentToken.line = cJSON_GetObjectItem(token, "line")->valueint;
    if (cJSON_HasObjectItem(token, "value")) {
        cJSON* value = cJSON_GetObjectItem(token, "value");
        if (currentToken.type == TOKEN_INT_LITERAL) {
            currentToken.value.i_val = value->valueint;
        } else if (currentToken.type == TOKEN_DOUBLE_LITERAL) {
            currentToken.value.d_val = value->valuedouble;
        }
    } else if (cJSON_HasObjectItem(token, "lexeme")) {
        currentToken.lexeme = strdup(cJSON_GetObjectItem(token, "lexeme")->valuestring);
    }
    return currentToken;
}
Token getNextToken(cJSON* tokens,int *currentTokenIndex) {
    if (*currentTokenIndex < cJSON_GetArraySize(tokens)) {
        cJSON* token = cJSON_GetArrayItem(tokens, *currentTokenIndex);
        Token nextToken;
        const char* jsonType = cJSON_GetObjectItem(token, "type")->valuestring;
        nextToken.type = mapJsonToEnum(jsonType);
        nextToken.line = cJSON_GetObjectItem(token, "line")->valueint;

        if (cJSON_HasObjectItem(token, "value")) {
            cJSON* value = cJSON_GetObjectItem(token, "value");
            if (nextToken.type == TOKEN_INT_LITERAL) {
                nextToken.value.i_val = value->valueint;
            } else if (nextToken.type == TOKEN_DOUBLE_LITERAL) {
                nextToken.value.d_val = value->valuedouble;
            }
        } else if (cJSON_HasObjectItem(token, "lexeme")) {
            nextToken.lexeme = strdup(cJSON_GetObjectItem(token, "lexeme")->valuestring);
        }

        *currentTokenIndex=*currentTokenIndex+1;
        return nextToken;
    } else {
        // Return a special token to indicate the end of input
        Token endToken;
        //endToken.type = END_OF_INPUT;
        return endToken;
    }
}

struct Node* parseDeclaration(cJSON* tokens, int *currentTokenIndex) {
    struct Node* declarationNode = (struct Node*)malloc(sizeof(struct Node));
    declarationNode->type = DEC_NODE;

    Token token = getNextToken(tokens, currentTokenIndex);
    if (token.type == TOKEN_INT_DECL || token.type == TOKEN_DOUBLE_DECL) {
        declarationNode->strValue = strdup(token.lexeme);
    } else {
        syntax_error("data type", token);
    }
    token = getNextToken(tokens, currentTokenIndex);
    if (token.type == TOKEN_IDENTIFIER) {
        declarationNode->left = (struct Node*)malloc(sizeof(struct Node));
        declarationNode->left->type = TOKEN_IDENTIFIER;
        declarationNode->left->strValue = strdup(token.lexeme);
    } else {
        syntax_error("identifier", token);
    }
    // Expect an assignment or end of line declaration
    token = getNextToken(tokens, currentTokenIndex);
    switch (token.type){
    case TOKEN_ASSIGN://parseAssignment(tokens, currentTokenIndex);break; uncomment only after impementing
    case TOKEN_NEW_LINE:break;
    default:syntax_error("'new line'", token);break;
    }
    return declarationNode;
}
struct Node* parseAssignment(cJSON* tokens, int *currentTokenIndex) {
    Token token = getNextToken(tokens, currentTokenIndex);
    struct Node* assignmentNode = (struct Node*)malloc(sizeof(struct Node));
    assignmentNode->type = ASS_NODE;

    if (token.type == TOKEN_IDENTIFIER) {
        assignmentNode->strValue = strdup(token.lexeme);
    } else {
        syntax_error("identifier", token);
    }

    token = getNextToken();
    if (token.type != TOKEN_ASSIGN) {
        syntax_error("'='", token);
    }

    // Parse the right-hand side (expression or value)
    assignmentNode->left = parseExpression(); // You'll need to implement parseExpression()

    // Expect nline
    token = getNextToken();
    if (token.type != TOKEN_NEW_LINE) {
        syntax_error("new line", token);
    }

    return assignmentNode;
}


int main() {
    cJSON* root;
    cJSON* tokens;
    int currentTokenIndex=0;
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
    int i=0;
    tokens = cJSON_GetObjectItem(root, "tokens");
    while (currentTokenIndex < cJSON_GetArraySize(tokens))
    {
        switch(getCurrentToken(tokens, &currentTokenIndex).type){
            case TOKEN_INT_DECL: 
            case TOKEN_DOUBLE_DECL:
                parseDeclaration(tokens, &currentTokenIndex);
                break;
            case TOKEN_ERROR:
                printf("Invalid syntax: line %i", getCurrentToken(tokens, &currentTokenIndex).line);
                break;
            default:
            printf("uknown sequence of tokens");
            exit(1);
        }
        
        
    }
    
    cJSON_Delete(root);
    free(jsonBuffer);
    //printf("hello");
    return 0;
}