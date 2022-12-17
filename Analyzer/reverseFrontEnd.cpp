#include <cstring>
#include <cassert>
#include "../common.h"
#include "analyzer.h"
#include "../utils/include/ErrorHandlerLib.h"

const char StandartFileName[] = "DBFile.txt";
const char OutFileName[]      = "translated.txt";
const char* WordForOP[]       = {"None", "tape", "jab", "None", "drug", "uppercut", "POW", "COS", "SIN", "LOG", "LN", "(", ")", "{", "}", ";", "EQ", ",", "<", ">", "IN", "OUT"};

void translate (Node* tree, const char* outFileName);
void printTreeInLang (Node* node, FILE* fileptr);
void printExpInLang (Node* node, FILE* fileptr);

int main ()
{
    Node* tree = {};
    tree = getTreeFromStandart (StandartFileName);

    translate (tree, OutFileName);
}

void translate (Node* tree, const char* outFileName)
{
    FILE* fileptr = fopen (outFileName, "w");
    if (fileptr == nullptr)
    {
        printError ("Cant open the file %s\n", outFileName);
        assert (0);
    }

    printTreeInLang (tree, fileptr);
}

void printTreeInLang (Node* node, FILE* fileptr)
{
    switch (node->type)
    {
        case Var_t:
            fprintf (fileptr, " %s ", node->var.varName);
            break;

        case Func_t:
            if (strcmp (node->Name, "FUNC") == 0)
            {
                if (node->left)  
                {
                    fprintf (fileptr, "new technique %s (", node->left->Name);
                    
                    Node* curNode = node->left; // Name of func

                    if (curNode->left)
                    {
                        curNode = curNode->left;  // param
                        
                        if (curNode->left)
                        {
                            printTreeInLang (curNode->left, fileptr);
                        }
                        else 
                            printError ("Param has no left son");

                        while (curNode->right)
                        {
                            curNode = curNode->right;

                            if (curNode->left)
                            {
                                fprintf (fileptr, ",");
                                printTreeInLang (curNode->left, fileptr);
                            }
                            else 
                                printError ("Param has no left son");
                            }
                    }

                    fprintf (fileptr, ") \n{\n");
                }
                else 
                    printError ("Function has no left son, that impossible");

                if (node->right)
                    printTreeInLang (node->right, fileptr);

                fprintf (fileptr, "};\n");
            }
            break;

        case Key_t:
            if (strcmp (node->Name, "ST") == 0)
            {
                if (node->left)
                    printTreeInLang (node->left, fileptr);
                else 
                    printError ("ST dont have left son");

                if (node->right)
                    printTreeInLang (node->right, fileptr);
            }
            else if (strcmp (node->Name, "VAR") == 0)
            {
                fprintf (fileptr, "var");

                if (node->left)
                    printTreeInLang (node->left, fileptr);
                else 
                    printError ("VAR dont have left son");

                if (node->right)
                {
                    printExpInLang (node->right, fileptr);
                    fprintf (fileptr, " hp;\n");
                }
            }
            else if (strcmp (node->Name, "IF") == 0)
            {
                fprintf (fileptr, "right (");
                if (node->left)
                    printExpInLang (node->left, fileptr);
                else 
                    printError ("If has no condition");
                fprintf (fileptr, ")\n{\n");

                if (node->right)
                {
                    printTreeInLang (node->right, fileptr);
                }
            }
            else if (strcmp (node->Name, "ELSE") == 0)
            {
                if (node->left)
                {
                    printTreeInLang (node->left, fileptr);
                }
                fprintf (fileptr, "}\nleft \n{\n");
                if (node->right)
                {
                    printTreeInLang (node->right, fileptr);
                }
                fprintf (fileptr, "}\n");
            }
            else if (strcmp (node->Name, "WHILE") == 0)
            {
                fprintf (fileptr, "Ccombo!! (");
                if (node->left)
                {
                    printTreeInLang (node->left, fileptr);
                }
                else
                    printError ("No condition in while");

                fprintf (fileptr, ")\n{\n");

                if (node->right)
                {
                    printTreeInLang (node->right, fileptr);
                }
                fprintf (fileptr, "}\n");
            }
            else if (strcmp (node->Name, "RET") == 0)
            {
                fprintf (fileptr, "K.O. ");
                if (node->left)
                    printExpInLang (node->left, fileptr);

                fprintf (fileptr, ";\n");
            }
            break;

        case OP_t:
            printExpInLang (node, fileptr);
            fprintf (fileptr, ";\n");
            break;

        case BuiltIn_t:
            if (node->left)
            {
                if (node->left->left)
                {
                    fprintf (fileptr, "%s (", WordForOP [node->opValue]);
                    fprintf (fileptr, "%s)", node->left->left->var.varName);
                }
            }
            fprintf (fileptr, ";\n");
            break;
    }
}

void printExpInLang (Node* node, FILE* fileptr)
{

    switch (node->type)
    {
        case OP_t:
            fprintf (stderr, "Node type %d", node->opValue);
            if (node->left)
                printExpInLang (node->left, fileptr);

            if (node->opValue > OP_DIV && node->opValue < OP_TER)
                printError ("This cmd dont support ");
            else if (node->opValue == OP_EQ)
            {
                if (node->right)
                    printExpInLang (node->right, fileptr);

                fprintf (fileptr," hp");
                return;
            
            }
            else
                fprintf (fileptr, " %s ", WordForOP[node->opValue]);

            if (node->right)
                printExpInLang (node->right, fileptr);

            break;

        case Num_t:
            if (node->left)
                printExpInLang (node->left, fileptr);

            fprintf (fileptr, "%lg", node->numValue);

            if (node->right)
                printExpInLang (node->right, fileptr);

            break;

        case BuiltIn_t:
            if (node->left)
            {
                if (node->left->left)
                {
                    fprintf (fileptr, "%s (", WordForOP [node->opValue]);
                    fprintf (fileptr, "%s)", node->left->left->var.varName);
                }
            }
            break;

        case Func_t:
            if (strcmp (node->Name, "CALL") == 0)
            {
                if (node->left)
                {
                    Node* curNode = node->left;
                    fprintf (fileptr, " %s (", curNode->Name); // name of func
                    if (curNode->left)
                    {
                        curNode = curNode->left; // param
                        printExpInLang (curNode->left, fileptr);

                        while (curNode->right)
                        {
                            curNode = curNode->right;
                            printExpInLang (curNode->left, fileptr);
                        }
                    }
                    fprintf (fileptr, ")");

                }
                else
                {
                    printError ("CALL has no left son, that impossible");
                    assert (0);
                }
            }
            else 
                printError ("Wrong Expression, nodeName %s", node->Name);
                
            break;

        case Var_t:
            if (node->left)
                printExpInLang (node->left, fileptr);

           fprintf (fileptr, " %s ", node->var.varName);

            if (node->right)
                printExpInLang (node->right, fileptr);

           break;

        case Key_t:
           printError ("This thing doesnt supposed to be here, %s", node->Name);
           break;

        default:
           printError ("Smth went wrong in exp, node->type %d", node->type);

    }

}

