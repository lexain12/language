#pragma once
#include <cstddef>
#include <stdio.h>
const size_t MAXCMDSIZE    = 100;
extern const size_t NUMOFNAMES;

enum OP
{
    UnknownOp = 0,
    OP_ADD    = 1,
    OP_SUB    = 2,
    OP_MUL    = 4,
    OP_DIV    = 5,
    OP_POW    = 6,
    OP_COS    = 7,
    OP_SIN    = 8,
    OP_LOG    = 9,
    OP_LN     = 10,
    OP_LBR    = 11,
    OP_RBR    = 12,
    OP_FRB    = 13,
    OP_FLB    = 14,
    OP_TER    = 15,
    OP_EQ     = 16,
    OP_COM    = 17,
};

extern const char* ShortOpArray;

enum Type
{
    Unknown = 0,
    OP_t    = 1,
    Var_t   = 2,
    Num_t   = 3,
    Key_t   = 4,
    Func_t  = 5,
};

enum VarType 
{
    global = 1,
    local  = 2,
};

struct Var 
{
    char* varName;
    VarType varType;
};

enum NameType 
{
    KEY_TYPE  = 1,
    VAR_TYPE  = 2,
    FUNC_TYPE = 3,
};

struct Node 
{
    Type   type;
    OP     opValue;
    double numValue;
    char*  Name;
    Var    var;
    Node*  left;
    Node*  right;
};

struct Name
{
    char* name;
    NameType type;
    int position;
};

struct NameTable
{
    int numOfVars;
    Name* data;
};

struct Utility
{
    Node** tokenArray;
    NameTable nameTable;
};

//Table Of Names
Name* tableAdd (char* name, Name* table);
Name* findInTable (char* name, Name* table);
void includeStdLib (Name* data, FILE* asmFile);
void initAsmFile (FILE* asmFile);

void treePrint (const Node* node, FILE* DBFileptr);
Node* treeParse (Node* node, FILE* DBFileptr, NameTable* nameTable, Type type);