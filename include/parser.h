#ifndef PARSER_H
#define PARSER_H

typedef enum{
    ROOT_NODE,
    
    DEC_INT_NODE,       //declaration
    DEC_DOUBLE_NODE,
    ASS_NODE,           //assignment
    VAR_NODE,           
    EXPR_NODE,
    TERM_NODE,
    FACTOR_NODE,
    PLUS_NODE,
    MINUS_NODE,
    DIV_NODE,
    MULTIPLY_NODE,
    MOD_NODE,          
    GREATER_NODE,       //node for >
    EQUAL_NODE,         //node for ==
    LESS_NODE,         //node for <
    WHILE_NODE,         //while
    IF_NODE,            //if
    THEN_ELSE_NODE,     //then-else
    THEN_NODE,
    ELSE_NODE,
    BLOCK_NODE,         //Block
    PRINT_NODE,
    LITERAL_INT_NODE,
    LITERAL_DOUBLE_NODE,
}NodeType;

typedef struct Node Node;
struct Node{
    NodeType type;
    union {
        int intValue;  
        double doubleValue;
    } Value;    //LITERAL nodes
    char* strValue;  //IDENTIFIER nodes
    Node* left;
    Node* right;
};

#endif