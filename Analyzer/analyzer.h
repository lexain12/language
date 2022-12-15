#pragma once

#pragma GCC diagnostic ignored "-Wwrite-strings"

extern const size_t MAXDATASIZE;

//tokenizer
void getBOpToken (Node*** tokenArray, char** string);
void skipSpaces (char** string);
void skipFileSpace (FILE* file);
void getOpOrWordToken (Node*** tokenArray, char** string, Utility utils);
void getTokens (Utility utils, char* string);
Node* keyWordCheck (char* data);

//common
Node* createNode (Type type, OP opValue, double numValue, char* varName, char* Name, Node* left, Node* right);
void nodeDtor (Node* node);
Node* nodeCtor ();
int strEqual(const char *l, const char *r);

//lexer
Node* getFunction (Utility* utils);
Node* getStatement (Utility* utils);
Node* getN (Utility* utils);
Node* getP (Utility* utils);
Node* getG (Utility* utils);
Node* getE (Utility* utils);
Node* getT (Utility* utils);
Node* getPW (Node*** tokenArray);
Node* getUnOP (Node*** tokenArray);
Node* getVar (Utility* utils);
Node* getCondition (Utility* utils);

//Table Of Names

#define createNum(NUM) createNode(Num_t, UnknownOp, NUM, nullptr, nullptr, nullptr, nullptr)
#define SUB(left, right) createNode(OP_t, OP_SUB, 0, nullptr, nullptr, left, right)
#define ADD(left, right) createNode(OP_t, OP_ADD, 0, nullptr, nullptr, left, right)
#define MUL(left, right) createNode(OP_t, OP_MUL, 0, nullptr, nullptr, left, right)
#define DIV(left, right) createNode(OP_t, OP_DIV, 0, nullptr, nullptr, left, right)
#define POW(left, right) createNode(OP_t, OP_POW, 0, nullptr, nullptr, left, right)
#define ABV(left, right) createNode(OP_t, OP_ABV, 0, nullptr, nullptr, left, right)
#define BLW(left, right) createNode(OP_t, OP_BLW, 0, nullptr, nullptr, left, right)
#define COS(right) createNode(      OP_t, OP_COS, 0, nullptr, nullptr, nullptr, right)
#define SIN(right) createNode(      OP_t, OP_SIN, 0, nullptr, nullptr, nullptr, right)
#define LOG(left, right) createNode(OP_t, OP_LOG, 0, nullptr, nullptr, left, right)
#define LN(right) createNode(       OP_t, OP_LN,  0, nullptr, nullptr, nullptr, right)
#define LBR()     createNode(       OP_t, OP_LBR, 0, nullptr, nullptr, nullptr, nullptr)
#define RBR()     createNode(       OP_t, OP_RBR, 0, nullptr, nullptr, nullptr, nullptr)
#define FLB()     createNode(       OP_t, OP_FLB, 0, nullptr, nullptr, nullptr, nullptr)
#define FRB()     createNode(       OP_t, OP_FRB, 0, nullptr, nullptr, nullptr, nullptr)
#define ELSE(left, right) createNode(Key_t, UnknownOp, 0, nullptr, "ELSE", left, right)
#define WORD(name) createNode(Unknown, UnknownOp, 0, nullptr, name, nullptr, nullptr)
#define TER()     createNode(       OP_t, OP_TER, 0, nullptr, nullptr, nullptr, nullptr)
#define COM()     createNode(       OP_t, OP_COM, 0, nullptr, nullptr, nullptr, nullptr)
#define VAR()     createNode(      Var_t, UnknownOp, 0, nullptr, nullptr, nullptr, nullptr)
#define EQ(left, right) createNode(OP_t, OP_EQ,   0, nullptr, nullptr, left, right)
#define FUNC(left, right) createNode(Func_t, UnknownOp, 0, nullptr, "FUNC", left, right)
#define ST(left, right) createNode(Key_t, UnknownOp, 0, nullptr, "ST", left, right)
#define PARAM(left, right) createNode(Key_t, UnknownOp, 0, nullptr, "PARAM", left, right)

void printFuncPrint (FILE* asmFile);
void printFuncIn (FILE* asmFile);
void printFuncSqrt (FILE* asmFile);
void printFuncAbs (FILE* asmFile);
