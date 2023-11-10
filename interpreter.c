#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/parser.h"

typedef struct SymbolTableEntry {
    char *variable;
    union 
    {
        double d_val;
        int i_val;
    }value;
    struct SymbolTableEntry *next;
} SymbolTableEntry;

SymbolTableEntry *symbolTable = NULL;

double interpret(Node *node);

void freeSymbolTable(SymbolTableEntry *table) {
    while (table) {
        SymbolTableEntry *temp = table;
        table = table->next;
        free(temp->variable);
        free(temp);
    }
}

SymbolTableEntry *findOrAddSymbolTableEntry(char *variable, int addIfNotFound, NodeType type) {
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
        }else if(type == LITERAL_DOUBLE_NODE){
            newEntry->value.d_val = 0;
        }
        newEntry->next = symbolTable;
        symbolTable = newEntry;
        return newEntry;
    }
    return NULL;
}

double evaluateBinaryOperation(double leftVal, double rightVal, NodeType type) {
    switch (type) {
        case PLUS_NODE: return leftVal + rightVal;
        case MINUS_NODE: return leftVal - rightVal;
        case MULTIPLY_NODE: return leftVal * rightVal;
        case DIV_NODE:
            if (rightVal == 0) {
                fprintf(stderr, "Division by zero error\n");
                exit(EXIT_FAILURE);
            }
            return leftVal / rightVal;
        case MOD_NODE:
            if (rightVal == 0) {
                fprintf(stderr, "Modulo by zero error\n");
                exit(EXIT_FAILURE);
            }
            return fmod(leftVal, rightVal);
        default:
            fprintf(stderr, "Invalid binary operation\n");
            exit(EXIT_FAILURE);
    }
}

double interpret(Node *node) {
    if (!node) {
        fprintf(stderr, "Null node encountered\n");
        exit(EXIT_FAILURE);
    }

    SymbolTableEntry *entry;
    double leftVal, rightVal;

    switch (node->type) {
        case LITERAL_INT_NODE:
            return node->Value.intValue;
        case LITERAL_DOUBLE_NODE:
            return node->Value.doubleValue;
        case VAR_NODE:
            entry = findOrAddSymbolTableEntry(node->strValue, 0, node->type);
            if (!entry) {
                fprintf(stderr, "Undefined variable: %s\n", node->strValue);
                exit(EXIT_FAILURE);
            }
            if(node->type == LITERAL_INT_NODE){
                return entry->value.i_val;
            }else if(node->type == LITERAL_DOUBLE_NODE){
                return entry->value.d_val;
            }
        case DEC_INT_NODE:
        case DEC_DOUBLE_NODE:
            if (entry) {
                fprintf(stderr, "Variable %s already declared\n", node->strValue);
                exit(EXIT_FAILURE);
            } else {
                // The variable was not found, so we add it to the symbol table with a default value
                if(node->type == DEC_DOUBLE_NODE){
                    entry = findOrAddSymbolTableEntry(node->strValue, 1, LITERAL_DOUBLE_NODE);
                }else{
                    entry = findOrAddSymbolTableEntry(node->strValue, 1, LITERAL_INT_NODE);
                }
                if(node->type == LITERAL_INT_NODE){
                    entry->value.i_val = 0;
                }else if(node->type == LITERAL_DOUBLE_NODE){
                    entry->value.d_val = 0;
                }
                return 0; // The declaration itself does not produce a value
            }
        case ASS_NODE:
            rightVal = interpret(node->right);
            entry = findOrAddSymbolTableEntry(node->left->strValue, 1, node->left->type);
            if(node->type == LITERAL_INT_NODE){
                entry->value.i_val = rightVal;
            }else if(node->type == LITERAL_DOUBLE_NODE){
                entry->value.d_val = rightVal;
            }
            return rightVal;

        case EXPR_NODE:
            // An expression node will typically be a wrapper around an arithmetic operation or a variable
            return interpret(node->left); // Assuming that the left side is where the expression is

        case TERM_NODE:
            // Same as EXPR_NODE, usually these are nested inside EXPR_NODE for handling different precedence levels
            return interpret(node->left);

        case FACTOR_NODE:
            // FACTOR_NODE is for numeric literals or parenthesized expressions
            return interpret(node->left);

        case PLUS_NODE:
        case MINUS_NODE:
        case DIV_NODE:
        case MULTIPLY_NODE:
        case MOD_NODE:
            leftVal = interpret(node->left);
            rightVal = interpret(node->right);
            return evaluateBinaryOperation(leftVal, rightVal, node->type);

        case PRINT_NODE:
            leftVal = interpret(node->left);
            printf("%f\n", leftVal);
            return leftVal;

        case COND_NODE:
            leftVal = interpret(node->left); // This would be the condition expression
            if (leftVal) {
                return interpret(node->right->left); // If-block
            } else {
                return interpret(node->right->right); // Else-block
            }

        case LOOP_NODE:
            leftVal = interpret(node->left);
            while(leftVal) {
                interpret(node->right->left);
                leftVal = interpret(node->left);
            }
            break;


        default:
            fprintf(stderr, "Unknown or unhandled node type: %d\n", node->type);
            exit(EXIT_FAILURE);
    }
}

int main() {
    // Assuming 'ast' is the root node of the AST obtained from the parser
    Node *ast = NULL;

    // Populate 'ast' with the actual AST from the parser
    // ...

    double result = interpret(ast);
    printf("Result: %f\n", result);

    // Clean up the symbol table
    freeSymbolTable(symbolTable);

    return 0;
}
