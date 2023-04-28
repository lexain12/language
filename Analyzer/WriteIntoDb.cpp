#pragma GCC diagnostic ignored "-Wswitch-enum"

#include <ctype.h>
#include <cassert>
#include <cstring>
#include "../common.h"
#include "analyzer.h"
#include "../utils/include/ErrorHandlerLib.h"

const char* FullOpArray[] = {"None", "ADD", "SUB", "None", "MUL", "DIV", "POW", "COS", "SIN", "LOG", "LN", "(", ")", "{", "}", ";", "EQ", ",", "IS_BT", "IS_GT", "IN", "OUT"};
const char* ShortOpArray = "n+-n*/^csll(){};=,<>";
const size_t NUMOFNAMES = 100;

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

    fprintf (DBFileptr, " { ");

    switch (node->type)
    {
        case Unknown:
            printError ("Unknown case in treePrint");
            break;

        case OP_t:
            fprintf (DBFileptr, " %s ", FullOpArray[node->opValue]);
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

        case BuiltIn_t:
            fprintf (DBFileptr, " %s ", FullOpArray[node->opValue]);
            break;

        default:
            printError ("Default case while writing into DB");
    }

    if ((node->left == nullptr) && (node->right == nullptr))
    {
        fprintf (DBFileptr, " } ");
        return;
    }

    if (node->left)
        treePrint (node->left, DBFileptr);
    else
        fprintf (DBFileptr, " { FULL } ");

    if (node->right)
        treePrint (node->right, DBFileptr);
    else
        fprintf (DBFileptr, " { FULL } ");

    fprintf (DBFileptr, " } ");

    return;
}

Node* treeParse (Node* node, FILE* DBFileptr, NameTable* nameTable, Type type)
{
    assert (DBFileptr != nullptr);
    assert (nameTable != nullptr);
    char* data = (char*) calloc (MAXDATASIZE, sizeof(*data));
    char bracket           = '\0';
    double num             = 0;
    Type leftType          = Unknown;
    Type rightType         = Unknown;

    skipFileSpace (DBFileptr);
    bracket = (char) fgetc (DBFileptr);
    printf ("%c\n", bracket);

    Node* curNode = nullptr;

    if (bracket == '{')
    {
        skipFileSpace (DBFileptr);

        char curChar = (char) fgetc (DBFileptr);                // This is fucking linux. %lg fscanf takes 'i' and 'n' (2 symbols), ignoring cases and dont return them
        fprintf (stderr, "ALLERT !!!! MF|%c|\n", curChar);
        ungetc (curChar, DBFileptr);


        if (!isalpha(curChar) && fscanf (DBFileptr, "%lf", &num))
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
                        curNode = WORD("VAR");
                        curNode->type = Key_t;
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

                    else if (strcmp (data, "EQ") == 0)
                        curNode = EQ(nullptr, nullptr);

                    else if (strcmp (data, "IS_GT") == 0)
                        curNode = ABV(nullptr, nullptr);

                    else if (strcmp (data, "IS_BT") == 0)
                        curNode = BLW(nullptr, nullptr);

                    else if (strcmp (data, "PARAM") == 0)
                    {
                        curNode = WORD("PARAM");
                        curNode->type = Key_t;
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

                    else if (strcmp (data, "FULL") == 0)
                    {
                        curNode = nullptr;
                    }

                    else if (strcmp (data, "IF") == 0)
                    {
                        curNode = WORD("IF");
                        curNode->type = Key_t;
                    }
                    else if (strcmp (data, "WHILE") == 0)
                    {
                        curNode = WORD("WHILE");
                        curNode->type = Key_t;
                    }

                    else if (strcmp (data, "ELSE") == 0)
                    {
                        curNode = WORD("ELSE");
                        curNode->type = Key_t;
                    }
                    else if (strcmp (data, "OUT") == 0)
                    {
                        curNode = BUILTIN (OP_OUT);
                    }

                    else if (strcmp (data, "IN") == 0)
                    {
                        curNode = BUILTIN (OP_IN);
                    }

                    else if (strcmp (data, "SQRT") == 0)
                    {
                        curNode = WORD("sqrt");
                        curNode->type = Func_t;
                    }

                    else
                    {
                        assert(0);
                        printError ("UnknownName, %s", data);
                    }

                    break;

                case OP_t:
                    printError ("OP_t, that was not suppose to be");
                    break;

                case Var_t:
                    if (strcmp (data, "VAR") == 0)
                    {
                        leftType = Var_t;
                        curNode = WORD("VAR");
                        curNode->type = Key_t;
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
    fprintf (stderr, "second bracket %c \n", bracket);


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


    printError ("problem place near %d\n", ftell (DBFileptr));
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
        fprintf (stderr, "current char |%c|\n", curChar);
        curChar = (char) fgetc (file);
    }

    fprintf (stderr, "current char would be ungetted |%c|\n", curChar);
    ungetc (curChar, file);
}

Node* makeTreeStandartIn (Node* node)
{
    if (node->left)
    {
        if (node->left->right)
        {
            if (node->left->right->type == Func_t)
            {
                fprintf (stderr, "Was here1\n");
                if (strcmp (node->left->right->Name, "CALL") == 0)
                {
                    fprintf (stderr, "Was here2\n");
                    if (strcmp (node->left->right->left->Name, "in") == 0)
                    {
                        node->left = IN(node->left->left);
                    }
                }
            }
        }
        if (node->left->left)
        {
            if (node->left->left->type == Func_t)
            {
                if (strcmp (node->left->left->Name, "print") == 0)
                {
                    fprintf (stderr, "Was here\n");
                    node->left = node->left->left;
                    node->left->Name = "OUT";
                }
            }
        }
    }
    if (node->left)
        node->left = makeTreeStandartIn (node->left);

    if (node->right)
        node->right = makeTreeStandartIn (node->right);

    return node;
}

Node* makeTreeStandartOut (Node* node)
{
    if (node->left)
        node->left = makeTreeStandartOut (node->left);

    if (node->right)
        node->right = makeTreeStandartOut (node->right);

    if (node->left)
        if (node->left->type == Func_t)
        {
            if (strcmp (node->left->Name, "print") == 0 || strcmp (node->left->Name, "sqrt") == 0)
            {
                node->left = FUNC (node->left, nullptr);
                node->left->Name = "CALL";
            }
        }

    return node;
}

Node* getTreeFromStandart (const char* FileName)
{
    printf ("Getting tree from standart\n");
    FILE* langFile = fopen (FileName, "r");
    if (langFile == nullptr)
    {
        printError ("Can't open file %s", FileName);
        assert (0);
    }

    NameTable tableForParse = {};
    tableForParse.data = (Name*) calloc (NUMOFNAMES, sizeof(*(tableForParse.data)));
    Node* tree = treeParse (nullptr, langFile, &tableForParse, Unknown);
    return tree;
}
