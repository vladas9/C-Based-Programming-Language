#include<stdio.h>
struct Node {
    int type;
    int intValue;    //INTEGER nodes
    char* strValue;  //IDENTIFIER nodes
    struct Node* left;
    struct Node* right;
};
struct Node* parseExpression(FILE* input);
struct Node* parseTerm(FILE* input);
struct Node* parseFactor(FILE* input);

// Function to parse an expression
struct Node* parseExpression(FILE* input) {
    struct Node* node = parseTerm(input);
    while (match(input, TOKEN_OPERATOR, "+") || match(input, TOKEN_OPERATOR, "-")) { //need to modify according to our tokens
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->type = TOKEN_OPERATOR;
        newNode->strValue = getTokenValue(input);
        newNode->left = node;
        newNode->right = parseTerm(input);
        node = newNode;
    }
    return node;
}

// Function to parse a term
struct Node* parseTerm(FILE* input) {
    struct Node* node = parseFactor(input);
    while (match(input, TOKEN_OPERATOR, "*") || match(input, TOKEN_OPERATOR, "/")) {//need to modify according to our tokens
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->type = TOKEN_OPERATOR;
        newNode->strValue = getTokenValue(input);
        newNode->left = node;
        newNode->right = parseFactor(input);
        node = newNode;
    }
    return node;
}

// Function to parse a factor
struct Node* parseFactor(FILE* input) {
    if (match(input, TOKEN_INTEGER, "")) {
        struct Node* node = (struct Node*)malloc(sizeof(struct Node));
        node->type = TOKEN_INTEGER;
        node->intValue = atoi(getTokenValue(input));
        return node;
    } else if (match(input, TOKEN_IDENTIFIER, "")) {
        struct Node* node = (struct Node*)malloc(sizeof(struct Node));
        node->type = TOKEN_IDENTIFIER;
        node->strValue = getTokenValue(input);
        return node;
    } else if (match(input, TOKEN_LPAREN, "(")) {
        struct Node* node = parseExpression(input);
        if (!match(input, TOKEN_RPAREN, ")")) {
            // Handle mismatched parentheses
            // can also add error handling here
        }
        return node;
    } else {
        // Handle syntax errors or unexpected tokens
        // an add error handling here
    }
    return NULL;
}

int main() {
    FILE* input = fopen("source.txt", "r");
    if (input == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    // Initialize lexer here

    struct Node* syntaxTree = parseExpression(input);

    // Handle the syntax tree, e.g., evaluate or generate code

    //free the syntax tree after we're done with it

    fclose(input);
    return 0;
}