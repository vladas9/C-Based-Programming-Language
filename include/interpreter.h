#ifndef INTERPRETER_H
#define INTERPRETER_H

typedef struct SymbolTableEntry {
    char *variable;
    union 
    {
        double d_val;
        int i_val;
    }value;
    struct SymbolTableEntry *next;
} SymbolTableEntry;


#endif