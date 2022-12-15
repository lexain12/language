void parseSt    (Node* node, Stack_t* stk, int isLocal, FILE* asmFile);
void parseFunc  (Node* node, Stack_t* stk, int isLocal, FILE* asmFile);
void parseWhile (Node* node, Stack_t* stk, FILE* asmFile);
void parseIf    (Node* node, Stack_t* stk, FILE* asmFile);
void parseEqual (Node* node, Stack_t* stk, FILE* asmFile);
void parseVar   (Node* node, Stack_t* stk, FILE* asmFile);
void parseExp   (Node* node, Stack_t* stk, FILE* asmFile);

void createTable (Stack_t* stk);
void destroyTable (Stack_t* stk);
int findInStack (char* name, Stack_t* stk);
void addRelativeShift (int shift, FILE* asmFile);
void tableDump (NameTable* nameTable);
int countVariables (Node* node);

void readInitParam (Node* node, Stack_t* stk, FILE* asmFile);
void readCallParam (Node* node, Stack_t* stk, FILE* asmFile);

