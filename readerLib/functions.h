#define ASSERT(condition) if(!(condition))          \
{                                                    \
    printf("Error in condition:%s line:%d\n", #condition, __LINE__); \
}
#include <cstddef>

const int sortedArray   = 0;
const int reversedArray = 1;
const int origArray     = 2;

enum Status
{
    inputError   = 1,
    readingError = 2,
    printError   = 3,
};

struct Line
{
    size_t length   = 0;
    char* charArray = nullptr;
};

struct InputFile   
{
    char*  fileName      = nullptr;
    size_t numberOfLines = 0;
    Line*  arrayOfLines  = nullptr;
    char*  text          = nullptr;
    size_t sizeOfFile    = 0;
};

int isBigger(const char* line1, const char* line2);
  
int countLines(FILE *intputFile);

void sortOnegin(InputFile* inputFile);

int readFileToLinesStruct(FILE* openedFile, InputFile* inputFile);

int splitIntoLines(char* text, size_t numberOfLines, Line* arrayOfLines);

void swapLines(char* line1, char* line2);

int readLines(char **arrayOfLines, FILE *inputFile, size_t numberOfLines);

Status printArrayInFile(FILE* fileToPrint, InputFile* inputFile);

Status printOrig(FILE* fileToPrint, char* buf);

void textToStr(char* text, int numberOfLines);

void readFile(FILE* openedFile, char* dest);

void addArrayInFile(const char* outputFile,InputFile* inputFile);

int checkArg(int numberOfArguments, const char* argumentsOfCmd[], InputFile* inputFile, char** outputFilePtr);

size_t fileSize (FILE* file);
int readFile(FILE* openedFile, char** dest);
int textToStr(char* text, size_t* numberOfLines);
int splitIntoLines(InputFile *inputFile);
int strToText(char* text, size_t* numberOfLines);

void makeGraph (Node* node);
void treeDump (Node* node, const char* str, ...);
void treeGraph (const Node* node, FILE* GraphFilePtr);

