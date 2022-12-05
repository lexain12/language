#include <stdio.h>
#include "common.h"
#include "Analyzer/analyzer.h"
#include "readerLib/functions.h"
#include <stdlib.h>

const size_t NUMOFNAMES = 100;

int main()
{
    InputFile inputFile = {"example.txt"}; 
    
    Utility utils = {}; 

    Node *tree = nullptr;

    FILE* DBFileptr = fopen ("example.txt", "r");
    readFileToLinesStruct (DBFileptr, &inputFile);

    utils.tokenArray = (Node**) calloc (inputFile.arrayOfLines[0].length, sizeof(*(utils.tokenArray)));
    utils.nameTable  = (Name*) calloc (NUMOFNAMES, sizeof(*(utils.nameTable)));
    getTokens (utils, inputFile.arrayOfLines[0].charArray);

    tree = getG (utils.tokenArray);
    treeDump (tree, "LOOK\n");

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
