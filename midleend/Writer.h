void parseSt    (Node* node, Stack_t* stk, int isLocal, FILE* asmFile);
void parseIf    (Node* node, Stack_t* stk, int isLocal, FILE* asmFile);
void parseWhile (Node* node, Stack_t* stk, int isLocal, FILE* asmFile);
void parseFunc  (Node* node, Stack_t* stk, int isLocal, FILE* asmFile);
void parseVar   (Node* node, Stack_t* stk, FILE* asmFile);
void parseExp   (Node* node, Stack_t* stk, FILE* asmFile);

void createTable (Stack_t* stk);
void destroyTable (Stack_t* stk);
int findInStack (char* name, Stack_t* stk);
void addRelativeShift (Stack_t* stk, int shift, FILE* asmFile);

