#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "include/parser.h"
#include "include/interpreter.h"

const char* NodeTypeToString(NodeType type) {
    switch (type) {
        case ROOT_NODE:
            return "ROOT_NODE";
        case DEC_INT_NODE:
            return "DEC_INT_NODE";
        case DEC_DOUBLE_NODE:
            return "DEC_DOUBLE_NODE";
        case ASS_NODE:
            return "ASS_NODE";
        case VAR_NODE:
            return "VAR_NODE";
        case EXPR_NODE:
            return "EXPR_NODE";
        case TERM_NODE:
            return "TERM_NODE";
        case FACTOR_NODE:
            return "FACTOR_NODE";
        case PLUS_NODE:
            return "PLUS_NODE";
        case MINUS_NODE:
            return "MINUS_NODE";
        case DIV_NODE:
            return "DIV_NODE";
        case MULTIPLY_NODE:
            return "MULTIPLY_NODE";
        case MOD_NODE:
            return "MOD_NODE";
        case GREATER_NODE:
            return "GREATER_NODE";
        case EQUAL_NODE:
            return "EQUAL_NODE";
        case LESS_NODE:
            return "LESS_NODE";
        case WHILE_NODE:
            return "WHILE_NODE";
        case IF_NODE:
            return "IF_NODE";
        case THEN_ELSE_NODE:
            return "THEN_ELSE_NODE";
        case THEN_NODE:
            return "THEN_NODE";
        case ELSE_NODE:
            return "ELSE_NODE";
        case BLOCK_NODE:
            return "BLOCK_NODE";
        case PRINT_NODE:
            return "PRINT_NODE";
        case LITERAL_INT_NODE:
            return "LITERAL_INT_NODE";
        case LITERAL_DOUBLE_NODE:
            return "LITERAL_DOUBLE_NODE";
        default:
            return "Unknown node";
    }
}

SymbolTableEntry *symbolTable = NULL;

int isWholeNumber(double value) {
    return value == floor(value);
}

SymbolTableEntry *findOrAddSymbolTableEntry(const char* variable, int addIfNotFound, NodeType type){
    SymbolTableEntry *entry = symbolTable;
    while (entry) {
        if (strcmp(entry->variable, variable) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    if (addIfNotFound) {
        SymbolTableEntry *newEntry = malloc(sizeof(SymbolTableEntry));
        if (!newEntry) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        newEntry->variable = strdup(variable);
        if(type == LITERAL_INT_NODE){
            newEntry->value.i_val = 0;
            newEntry->type = "int";
        }else if(type == LITERAL_DOUBLE_NODE){
            newEntry->value.d_val = 0;
            newEntry->type = "double";
        }
        newEntry->next = symbolTable;
        symbolTable = newEntry;
        return newEntry;
    }
    return NULL;
}

double interpret(Node *ast){
    SymbolTableEntry *entry;
    // if(ast->type == ASS_NODE){
    //     printf("%s\n", NodeTypeToString(ast->left->type));
    //     printf("%s\n", ast->strValue);
    // }
    double leftVal, rightVal;
    if(!ast){
        // printf("NO node\n");
        return;
    }
    // printf("%s\n", NodeTypeToString(ast->type));
    switch (ast->type)
    {
    case ROOT_NODE:
        // printf("Enter root\n");
        return interpret(ast->left);
    case DEC_INT_NODE:
    case DEC_DOUBLE_NODE:
        // printf("declared successful\n");
        entry = findOrAddSymbolTableEntry(ast->left->strValue, 0, ast->type);
        if(entry){
            printf("Variable %s already declared\n", ast->left->strValue);
            exit(EXIT_FAILURE);
        }else{
                if(ast->type == DEC_DOUBLE_NODE){
                    entry = findOrAddSymbolTableEntry(ast->left->strValue, 1, LITERAL_DOUBLE_NODE);
                }else{
                    entry = findOrAddSymbolTableEntry(ast->left->strValue, 1, LITERAL_INT_NODE);
                }
                interpret(ast->right);
                return;
        }
        break;
    case LITERAL_INT_NODE:
        return ast->Value.intValue;
    case LITERAL_DOUBLE_NODE:
        return ast->Value.doubleValue;
    case ASS_NODE:
        // printf("Start assignment\n");
        entry = findOrAddSymbolTableEntry(ast->strValue, 0, ast->type);
        if (!entry) {
            printf("ERROR: undeclared variable %s\n", ast->strValue);
            exit(EXIT_FAILURE);
        }

        // Evaluate the right-hand side of the assignment
        double leftVal = interpret(ast->left);
        if (strcmp(entry->type, "int") == 0) {
            if (isWholeNumber(leftVal)) {
                entry->value.i_val = (int)leftVal;
            } else {
                printf("Type mismatch: Cannot assign a non-integer value to an integer variable\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(entry->type, "double") == 0) {
            entry->value.d_val = leftVal;
        } else {
            printf("Unknown variable type\n");
            exit(EXIT_FAILURE);
        }
        interpret(ast->right);
        break;
    case VAR_NODE:
        entry = findOrAddSymbolTableEntry(ast->strValue, 0, ast->type);
        if(entry){
            if(entry->type == "int"){
                return entry->value.i_val;
            }else if(entry->type == "double"){
                return entry->value.d_val;
            }
        }else{
            printf("ERROR: undeclared variable\n");
            exit(EXIT_FAILURE);
        }
        break;
    case PLUS_NODE:
        // printf("ADD\n");
        return interpret(ast->left) + interpret(ast->right);
    case MINUS_NODE:
        // printf("MIN\n");
        return interpret(ast->left) - interpret(ast->right);
    case MULTIPLY_NODE:
        // printf("MULTI\n");
        return interpret(ast->left) * interpret(ast->right);
    case DIV_NODE:
        // printf("DIV\n");
        return interpret(ast->left) / interpret(ast->right);
    case GREATER_NODE:
        // printf("GREATER\n");
        return interpret(ast->left) > interpret(ast->right);
    case IF_NODE:
        // printf("IF\n");
        if((int)interpret(ast->left)){
            interpret(ast->right->left->left);
        }else{
            interpret(ast->right->right->left);
        }
        break;
    case WHILE_NODE:
        // printf("WHILE\n");
        while((int)interpret(ast->left)){
            interpret(ast->right->left);
        }
    case PRINT_NODE:
        leftVal= interpret(ast->left);
        if(isWholeNumber(leftVal)){
            printf("%i\n", (int)leftVal);   
        }else{
            printf("%f\n", leftVal);
        }
        interpret(ast->right);
        break;
    default:
        break;
    }
}


int interpreter(Node *ast) {
    interpret(ast);

    return 0;
}
