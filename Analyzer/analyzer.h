extern const size_t MAXDATASIZE;

//tokenizer
void getBOpToken (Node*** tokenArray, char** string);
void skipSpaces (char** string);
void getOpOrWordToken (Node*** tokenArray, char** string, Utility utils);
void getTokens (Utility utils, char* string);
char* findInTable (char* name, Name* table);
int tableAdd (char* name, Name* table);

//common
Node* createNode (Type type, OP opValue, double numValue, char* varName, char* Name, Node* left, Node* right);
void nodeDtor (Node* node);
Node* nodeCtor ();
int strEqual(const char *l, const char *r);

//lexer
Node* getN (Node*** tokenArray);
Node* getP (Node*** tokenArray);
Node* getG (Node*** tokenArray);
Node* getE (Node*** tokenArray);
Node* getT (Node*** tokenArray);
Node* getPW (Node*** tokenArray);
Node* getUnOP (Node*** tokenArray);
Node* getVar (Node*** tokenArray);

#define createNum(NUM) createNode(Num_t, UnknownOp, NUM, nullptr, nullptr, nullptr, nullptr)
#define SUB(left, right) createNode(OP_t, OP_SUB, 0, nullptr, nullptr, left, right)
#define ADD(left, right) createNode(OP_t, OP_ADD, 0, nullptr, nullptr, left, right)
#define MUL(left, right) createNode(OP_t, OP_MUL, 0, nullptr, nullptr, left, right)
#define DIV(left, right) createNode(OP_t, OP_DIV, 0, nullptr, nullptr, left, right)
#define POW(left, right) createNode(OP_t, OP_POW, 0, nullptr, nullptr, left, right)
#define COS(right) createNode(      OP_t, OP_COS, 0, nullptr, nullptr, nullptr, right)
#define SIN(right) createNode(      OP_t, OP_SIN, 0, nullptr, nullptr, nullptr, right)
#define LOG(left, right) createNode(OP_t, OP_LOG, 0, nullptr, nullptr, left, right)
#define LN(right) createNode(       OP_t, OP_LN,  0, nullptr, nullptr, nullptr, right)
#define LBR()     createNode(       OP_t, OP_LBR, 0, nullptr, nullptr, nullptr, nullptr)
#define RBR()     createNode(       OP_t, OP_RBR, 0, nullptr, nullptr, nullptr, nullptr)
#define FLB()     createNode(       OP_t, OP_FLB, 0, nullptr, nullptr, nullptr, nullptr)
#define FRB()     createNode(       OP_t, OP_FRB, 0, nullptr, nullptr, nullptr, nullptr)
#define WORD(name) createNode(Unknown, UnknownOp, 0, nullptr, name, nullptr, nullptr)
#define TER()     createNode(       OP_t, OP_TER, 0, nullptr, nullptr, nullptr, nullptr)

