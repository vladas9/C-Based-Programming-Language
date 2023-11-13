#ifndef INTERPRETER_H
#define INTERPRETER_H

typedef struct SymbolTableEntry {
    char *variable;
    char *type;
    union 
    {
        double d_val;
        int i_val;
    }value;
    struct SymbolTableEntry *next;
} SymbolTableEntry;

int interpreter(Node *ast);

#endif