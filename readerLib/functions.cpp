#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <cstddef>
#include "../common.h"

#include "functions.h"

extern FILE* LOGFILEPTR;
const char* FullOpArray1[] = {"None", "ADD", "SUB", "None", "MUL", "DIV", "POW", "COS", "SIN", "LOG", "LN", "(", ")", "{", "}", ";", "EQ", ",", "&lt;", "&gt;", "IN", "OUT"};
const char   GraphFile[20] = "GraphFile.txt";

size_t fileSize (FILE* file)
{
    fseek(file, 0l, SEEK_END);
    size_t size = (size_t) ftell(file);
    fseek(file, 0l, SEEK_SET);

    return size;
}

int readFile(FILE* openedFile, char** dest)
{
    ASSERT(openedFile != nullptr);

    size_t numberOfChars = fileSize(openedFile);

//    printf("filesize = %lu\n", numberOfChars);

    *dest = (char*) calloc(numberOfChars + 1, sizeof(char));

    size_t charsReaded = fread((void*) *dest, sizeof(char), numberOfChars, openedFile); //
    if (charsReaded != numberOfChars)
        return readingError;

    return 0;
}

int textToStr(char* text, size_t* numberOfLines)
{
    ASSERT(text != nullptr);

    size_t i = 0;
    for (; text[i] != '\0'; i++)
    {
        if (text[i] == '\n')
        {
            *numberOfLines += 1;
            text[i] = '\0';
        }
    }

    text[i] = '\n';
//    fprintf(stderr, "number of lines = %lu\n", *numberOfLines);

    return 0;
}

int strToText(char* text, size_t* numberOfLines)
{
    ASSERT(text != nullptr);

    size_t i = 0;
    for (; text[i] != '\n'; i++)
    {
        if (text[i] == '\0')
        {
            *numberOfLines += 1;
            text[i] = '\n';
        }
    }

    text[i] = '\0';
//    fprintf(stderr, "number of lines = %lu\n", *numberOfLines);

    return 0;
}

int splitIntoLines(InputFile *inputFile)
{
    ASSERT(inputFile != nullptr);

    char *text           =  inputFile->text;
    size_t numberOfLines =  inputFile->numberOfLines;
    Line **Lines         = &inputFile->arrayOfLines;

    ASSERT(text != NULL);

    *Lines = (Line*) calloc(numberOfLines, sizeof(Line));
    Line* arrayOfLines = *Lines;
    ASSERT(arrayOfLines != NULL);

    size_t line = 0;

//    fprintf(stderr, "%s", ((inputFile->arrayOfLines)[line]).charArray);
//    fprintf(stderr, "%s\n", ((arrayOfLines)[line]).charArray);
    arrayOfLines[line].charArray = text;
//    fprintf(stderr, "%s", ((inputFile->arrayOfLines)[line]).charArray);

    line++;

    size_t i = 0;
    for (; text[i + 1] != '\n'; i++)
    {
        if (text[i] == '\0')
        {
//           fprintf(stderr, "line number is %lu\n", line);

           arrayOfLines[line - 1].length    = (size_t) (&(text)[i] - arrayOfLines[line - 1].charArray);
           arrayOfLines[line]    .charArray = &(text)[i + 1];

        // fprintf(stderr, "%s\n", ((arrayOfLines)[line]).charArray);
 //          fprintf(stderr, "length = %lu\n", arrayOfLines[line - 1].length);

           line++;
        }
    }

    arrayOfLines[line - 1].length = (size_t) (&text[i] - arrayOfLines[line - 1].charArray);

//    fprintf(stderr, "line number is %lu\n", line);

    return 0;
}

int readFileToLinesStruct(FILE* openedFile, InputFile* inputFile)
{
    ASSERT(openedFile != nullptr);

    readFile(openedFile, &(inputFile->text));
    char* text = inputFile->text;
    textToStr(inputFile->text, &(inputFile->numberOfLines));
    splitIntoLines(inputFile);
    strToText (inputFile->text, &(inputFile->numberOfLines));

    return 0;
}

void treeDump (Node* node, const char* str, ...)
{
    fprintf(LOGFILEPTR, "<hr>\n");

    va_list argPtr = {};
    va_start (argPtr, str);

    fprintf (LOGFILEPTR, "<h2>");
    vfprintf (LOGFILEPTR, str, argPtr);
    fprintf (LOGFILEPTR, "</h2>\n");

    makeGraph (node);
    static int picVersion = 0;
    fprintf (LOGFILEPTR, "<img src = \"src/pic%d.svg\"/>\n", picVersion++);

    return;
}

#define dumpprint(...) fprintf(GraphFilePtr, __VA_ARGS__);
void treeGraph (const Node* node, FILE* GraphFilePtr)
{
    assert (node         != nullptr);
    assert (GraphFilePtr != nullptr);

    if (node->left)
    {
        //dumpprint ("    nd%p [fillcolor=\"#54e3c2\", label=\"{ %d | ",
        dumpprint ("    nd%p [label=\"{ %d | ",
                    node->left, node->left->type);

        switch (node->left->type)
        {
            case OP_t:
                dumpprint ("%s }\", fillcolor=\"#ffc0c0\" ", FullOpArray1[node->left->opValue - 1]);
                break;

            case Num_t:
                dumpprint ("%lg}\", fillcolor=\"#c0ffc0\" ", node->left->numValue);
                break;

            case Var_t:
                dumpprint ("%s}\", fillcolor=\"#c0c0ff\" ", node->left->var.varName);
                break;

            case Unknown:
                dumpprint ("%s}\", fillcolor=\"#ffffc0\"", node->left->Name);
                break;

            case Key_t:
                dumpprint ("%s}\", fillcolor=\"#ffc0ff\"", node->left->Name);
                break;

            case Func_t:
                dumpprint ("%s}\", fillcolor=\"#c0ffff\"", node->left->Name);
                break;

            case BuiltIn_t:
                dumpprint ("%s}\", fillcolor=\"#c0c0c0c\"", FullOpArray1[node->left->opValue - 1]);
                break;

            default:
                fprintf (stderr, "%d\n", node->left->type);
                assert (0);
        }
        dumpprint (" ];\n");

        dumpprint ("    nd%p -> nd%p;\n", node, node->left);

        treeGraph (node->left, GraphFilePtr);
    }

    if (node->right)
    {
        dumpprint ("    nd%p [fillcolor=\"#54e3c2\", label=\" { %d |",
                    node->right, node->right->type);

        switch (node->right->type)
        {
            case OP_t:
                dumpprint ("\'%s\' }", FullOpArray1[node->right->opValue - 1]);
                break;

            case Num_t:
                dumpprint ("%lg }", node->right->numValue);
                break;

            case Var_t:
                dumpprint ("%s }", node->right->var.varName);
                break;

            case Unknown:
                dumpprint ("%s }", node->right->Name);
                break;

            case Key_t:
                dumpprint ("%s}", node->right->Name);
                break;

            case Func_t:
                dumpprint ("%s}", node->right->Name);
                break;

            case BuiltIn_t:
                dumpprint ("%s }", FullOpArray1[node->left->opValue - 1]);
                break;

            default:
                fprintf (stderr, "%d\n", node->right->type);
                assert (0);

        }
        dumpprint (" \"];\n");


        dumpprint ("    nd%p -> nd%p;\n", node, node->right);

        treeGraph (node->right, GraphFilePtr);
    }

    return;
}

void makeGraph (Node* node)
{
    FILE* GraphFilePtr = fopen(GraphFile, "w");
    assert (node != nullptr);
    assert (GraphFilePtr != nullptr);

    dumpprint ("digraph MyGraph {\n")
    dumpprint ("    node [color=black, shape=record, style=\"rounded, filled\"];\n")
    dumpprint ("    rankdir=TB;\n")
    dumpprint ("    edge[constraint=true];\n")

    dumpprint ("    nd%p [fillcolor=\"#54e3c2\", label=\"%d\"];\n",
                node, node->type);
        dumpprint ("    nd%p [fillcolor=\"#54e3c2\", label=\"{ %d | ",
                    node, node->type);

        switch (node->type)
        {
            case OP_t:
                dumpprint ("%s}", FullOpArray1[node->opValue - 1]);
                break;

            case Num_t:
                dumpprint ("%lg }", node->numValue);
                break;

            case Var_t:
                dumpprint ("%s }", node->var.varName);
                break;

            case Unknown:
                dumpprint ("%s }", node->Name);
                break;

            case Key_t:
                dumpprint ("%s }", node->Name);
                break;

            case Func_t:
                dumpprint ("%s }", node->Name);
                break;

            case BuiltIn_t:
                dumpprint ("%s}", FullOpArray1[node->left->opValue - 1]);
                break;

            default:
                assert (0);
        }
        dumpprint ("\"];\n");

   treeGraph (node, GraphFilePtr);

    dumpprint ("}\n")

    fclose(GraphFilePtr);
    static int picVersion = 0;

    char buf[MAXCMDSIZE] = "";
    sprintf(buf, "dot -Tsvg -Gcharset=latin1 GraphFile.txt > src/pic%d.svg", picVersion);
    picVersion++;

    system (buf);
}

//Status printArrayInFile(FILE* fileToPrint, InputFile* inputFile)
//{
//    if (!fileToPrint)
//        return printError;
//    // fseek(fileToPrint, 0l, SEEK_SET);
//
//    for (size_t i = 0; i < inputFile->numberOfLines; ++i)
//        fprintf(fileToPrint, "%s\n", ((inputFile->arrayOfLines)[i]).charArray);
//
//    return noErrors;
//}
//
//Status printOrig(FILE* fileToPrint, char* buf)
//{
//    size_t i = 0;
//    for (;buf[i] != '\n'; i++)
//    {
//        if (buf[i] == '\0')
//        {
//            buf[i] = '\n';
//        }
//    }
//
//    buf[i] = '\0';
//    fprintf(fileToPrint, "%s", buf);
//    return noErrors;
//}
//
