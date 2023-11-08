#include "include\parser.h"
#include <stdio.h>
#include <stdlib.h>

double evaluateExpression(Node *expr){
    switch (expr->type)
    {
    case LITERAL_NODE:
        return expr->Value.intValue;
    case PLUS_NODE:
        return evaluateExpression(expr->left) + evaluateExpression(expr->right);
    case DIV_NODE:
        return evaluateExpression(expr->left)/evaluateExpression(expr->right);
    default:
        break;
    }
}

int main(){
    Node *node;
    node = malloc(sizeof(Node)); // Allocate memory for the PLUS_NODE
    node->type = PLUS_NODE;
    
    node->left = malloc(sizeof(Node)); // Allocate memory for the left child
    node->left->type = LITERAL_NODE;
    node->left->Value.intValue = 4;

    node->right = malloc(sizeof(Node)); // Allocate memory for the right child (DIV_NODE)
    node->right->type = DIV_NODE;

    node->right->left = malloc(sizeof(Node)); // Allocate memory for the left child of DIV_NODE
    node->right->left->type = LITERAL_NODE;
    node->right->left->Value.intValue = 16;

    node->right->right = malloc(sizeof(Node)); // Allocate memory for the right child of DIV_NODE
    node->right->right->type = LITERAL_NODE;
    node->right->right->Value.intValue = 8;
    int r = evaluateExpression(node);
    printf("%i", r);
}