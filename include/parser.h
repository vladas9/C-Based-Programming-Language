#ifndef PARSER_H
#define PARSER_H

typedef enum{
    DEC_INT_NODE,//declaration
    DEC_DOUBLE_NODE,
    ASS_NODE,//assignment
    VAR_NODE,
    EXPR_NODE,
    TERM_NODE,
    FACTOR_NODE,
    PLUS_NODE,
    MINUS_NODE,
    DIV_NODE,
    MULTIPLY_NODE,
    MOD_NODE,
    LOOP_NODE,
    COND_NODE,
    PRINT_NODE,
    LITERAL_INT_NODE,
    LITERAL_DOUBLE_NODE
}NodeType;

typedef struct Node{
    NodeType type; 
    union {
        int intValue;  
        double doubleValue;
    } Value;    //LITERAL nodes
    char* strValue;  //IDENTIFIER nodes
    struct Node* left;
    struct Node* right;
}Node;

#endif