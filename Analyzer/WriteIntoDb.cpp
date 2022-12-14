#include <ctype.h>
#include <cassert>
#include <cstring>
#include "../common.h"
#include "analyzer.h"
#include "../utils/include/ErrorHandlerLib.h"

const char* ShortOpArray = "n+-*/^csll(){};=,";

void treePrint (const Node* node, FILE* DBFileptr)
{
    if (node == nullptr) 
    {
        printError ("Node has nullptr");
        return;
    }
    if (DBFileptr == nullptr)
    {
        printError ("DBFileptr has nullptr");
        return;
    }

    fprintf (DBFileptr, "{");

    switch (node->type)
    {
        case Unknown:
            printError ("Unknown case in treePrint");
            break;

        case OP_t:
            fprintf (DBFileptr, " %c ", ShortOpArray[node->opValue]);
            break;

        case Var_t:
            fprintf (DBFileptr, " %s ", node->var.varName);
            break;

        case Num_t:
            fprintf (DBFileptr, " %lg ", node->numValue);
            break;

        case Key_t:
            fprintf (DBFileptr, " %s ", node->Name);
            break;

        case Func_t:
            fprintf (DBFileptr, " %s ", node->Name);
            break;

        default:
            printError ("Default case while writing into DB");
            assert (0);
    }

    if (node->left)  
        treePrint (node->left, DBFileptr);
    else
        fprintf (DBFileptr, "{ NIL }");
            
    if (node->right) 
        treePrint (node->right, DBFileptr);
    else
        fprintf (DBFileptr, "{ NIL }");

    fprintf (DBFileptr, "}");

    return;
}

Node* treeParse (Node* node, FILE* DBFileptr, NameTable* nameTable, Type type)
{
    assert (node != nullptr);
    assert (DBFileptr != nullptr);
    assert (nameTable != nullptr);
    char* data = (char*) calloc (MAXDATASIZE, sizeof(*data));
    char bracket           = '\0';
    double num             = 0;
    Type leftType          = Unknown;
    Type rightType         = Unknown;

    bracket = (char) fgetc (DBFileptr);

    Node* curNode = nullptr; 

    if (bracket == '{')
    {
        skipFileSpace (DBFileptr);

        if (fscanf (DBFileptr, "%lg", &num))
        {
            curNode = createNum (num);
            printf ("I AM NUMBER %lg\n", num);
        }
        else if (fscanf (DBFileptr, "%s", data))
        {
            printf ("I am here %s, type %d \n", data, type);
            switch (type)
            {
                case Unknown:
                    if (findInTable (data, nameTable->data))
                    {
                        curNode = VAR();
                        curNode->var.varName = data;
                    }
                    else if (strcmp (data, "ST") == 0)
                    {
                        curNode = ST(nullptr, nullptr);
                    }
                    else if (strcmp (data, "FUNC") == 0)
                    {
                        leftType = Func_t;
                        curNode = FUNC(nullptr, nullptr);
                    }
                    else if (strcmp (data, "VAR") == 0)
                    {
                        leftType = Var_t;
                        curNode = VAR();
                        curNode->var.varName = data;
                    }
                    else if (strcmp (data, "ADD") == 0)
                        curNode = ADD(nullptr, nullptr);

                    else if (strcmp (data, "MUL") == 0)
                        curNode = MUL(nullptr, nullptr);

                    else if (strcmp (data, "SUB") == 0)
                        curNode = SUB(nullptr, nullptr);

                    else if (strcmp (data, "DIV") == 0)
                        curNode = DIV(nullptr, nullptr);

                    else if (strcmp (data, "PARAM") == 0)
                    {
                        curNode = WORD("PARAM");
                        curNode->type = Key_t;
                        leftType = Var_t;
                    }

                    else if (strcmp (data, "RET") == 0)
                    {
                        curNode = WORD("RET");
                        curNode->type = Key_t;
                    }

                    else if (strcmp (data, "CALL") == 0)
                    {
                        curNode = FUNC(nullptr, nullptr);
                        curNode->Name = "CALL";
                        leftType = Func_t;
                    }
                    else if (strcmp (data, "NIL") == 0)
                    {
                        curNode = nullptr;
                    }

                    else
                        printError ("UnknownName, %s", data);

                    break;

                case OP_t:
                    printError ("OP_t, that was not suppose to be");
                    break;

                case Var_t:
                    if (strcmp (data, "VAR") == 0)
                    {
                        leftType = Var_t;
                        curNode = VAR();
                        curNode->var.varName = data;
                    }
                    else 
                    {
                        tableAdd (data, nameTable->data);
                        curNode = VAR();
                        curNode->Name = data;
                        curNode->var.varName= data;
                    }
                    break;

                case Num_t:
                    printError ("Num_t, that was not suppose to be");
                    break;

                case Key_t:
                    curNode = WORD(data);
                    curNode->type = Key_t;
                    break;

                case Func_t:
                    curNode = FUNC(nullptr, nullptr);
                    curNode->Name = data;
                    break;

                default:
                    printError ("Default");
                    assert (0);
            }
        }
        else
        {
            printError ("Can't read from dbFile");
        }
    }

    skipFileSpace (DBFileptr);
    bracket = (char) fgetc (DBFileptr);


    if (bracket == '}')
    {
        printf ("First return\n");
        return curNode;
    }

    if (bracket == '{')
    {
        printf ("GOING LEFT, Type %d\n", leftType);
        ungetc (bracket, DBFileptr);
        curNode->left = treeParse (curNode, DBFileptr, nameTable, leftType);
    }

    skipFileSpace (DBFileptr);
    bracket = (char) fgetc (DBFileptr);


    if (bracket == '}')
    {
        printf ("second return\n");
        return curNode;
    }
    else if (bracket == '{')
    {
        ungetc (bracket, DBFileptr);
        curNode->right = treeParse (curNode, DBFileptr, nameTable, rightType);
    }

    skipFileSpace (DBFileptr);
    bracket = (char) fgetc (DBFileptr);


    if (bracket == '}')
    {
        printf ("Last return\n");
        return curNode;
    }

    assert (0);
    return curNode;
}

void skipFileSpace (FILE* file)
{
    if (file == nullptr)
    {
        printError ("file has nullptr");
    }
    char curChar = (char) fgetc (file);

    while (isspace(curChar)) 
    {
        curChar = (char) fgetc (file);
    }

    ungetc (curChar, file);
}