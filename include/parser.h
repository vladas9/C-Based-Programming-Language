#ifndef PARSER_H
#define PARSER_H

typedef enum{
    DEC_NODE,//declaration
    ASS_NODE,//assignment
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
    LITERAL_NODE,
}NodeType;


#endif