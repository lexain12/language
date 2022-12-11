#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "stack/config.h"
#include "stack/stack.h"
#include "Analyzer/analyzer.h"
#include "readerLib/functions.h"
#include "midleend/Writer.h"

const size_t NUMOFNAMES = 100;

int main()
{
    InputFile inputFile = {"example.txt"}; 
    FILE* asmFile = fopen ("asmMainFile.txt", "w");
    
    Utility utils = {}; 
    Stack_t stk   = {};
    stackCtor (&stk, 10); // fix this

    Node *tree = nullptr;

    FILE* DBFileptr = fopen ("example.txt", "r");
    readFileToLinesStruct (DBFileptr, &inputFile);

    utils.tokenArray = (Node**) calloc (fileSize(DBFileptr), sizeof(*(utils.tokenArray)));
    utils.nameTable.data  = (Name*) calloc (NUMOFNAMES, sizeof(*(utils.nameTable.data)));
    getTokens (utils, inputFile.arrayOfLines[0].charArray);

    tree = getG (&utils);
    treeDump (tree, "LOOK\n");

    printf ("start parsing\n");
    parseSt (tree, &stk, 1, asmFile);

//    for (int i = 0; utils.tokenArray[i]; i ++)
//    {
//        treeDump (utils.tokenArray[i], "HEY\n");
//    }

//    for (size_t index = 0; index < NUMOFNAMES && utils.nameTable[index].name; ++index)
//    {
//        fprintf (stderr, "%s\n", utils.nameTable[index].name);
//    }

//

}
