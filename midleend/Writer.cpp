#include "../stack/config.h"
#include "../common.h"
#include "../stack/stack.h"
#include "../utils/include/ErrorHandlerLib.h"
#include "Writer.h"
#include <cassert>

static int numberOfLabels = 0;

void createTable (Stack_t* stk)
{
    NameTable* newTable = (NameTable*) calloc (1, sizeof (*(newTable)));
    assert (newTable != nullptr);

    Name* newData = (Name*) calloc (NUMOFNAMES, sizeof(*newData));
    assert (newData != nullptr);

    newTable->data = newData;

    stackPush (stk, newTable);
}

void destroyTable (Stack_t* stk)
{
    NameTable* oldTable = stackPop (stk);

    free (oldTable);
}

void parseSt (Node* node, Stack_t* stk, int isLocal, FILE* asmFile)
{

    fprintf (stderr, "CURNODE %d %d \n", node->type, node->opValue);

    int relativeShift = countVariables (node);

    if (isLocal)
    {
        createTable (stk);
        addRelativeShift (relativeShift, asmFile);

        printf ("CREATED new TABLE\n");
    }


    if (node->left)
    {
        fprintf (stderr, "LEFTNODE %d %d\n", node->left->type, node->left->opValue);

        switch (node->left->type)
        {
            case OP_t:
                parseExp (node->left, stk, asmFile);
                break;

            case Var_t:
                parseExp (node->left, stk, asmFile);
                break;

            case Num_t:
                parseExp (node->left, stk, asmFile);
                break;

            case Key_t:
                printf ("%s\n", node->left->Name);
                if (strcmp (node->left->Name, "VAR") == 0)
                    parseVar (node->left, stk, asmFile);

                else if (strcmp (node->left->Name, "IF") == 0)
                    parseIf (node->left, stk, asmFile);

                else if (strcmp (node->left->Name, "WHILE") == 0)
                    parseWhile (node->left, stk, asmFile);

                else if (strcmp (node->left->Name, "ST") == 0)
                    parseSt (node->left, stk, 1, asmFile);

                else if (strcmp (node->left->Name, "RET") == 0)
                {
                    parseSt (node->left, stk, 0, asmFile);
                }
                    

                break;

            case Func_t:
                parseFunc (node->left, stk, 1, asmFile);
                break;

            case BuiltIn_t:
                parseFunc (node->left, stk, 1, asmFile);
                break;

            case Unknown:
                fprintf (stderr, "Writer unknown %s\n", node->Name);
                assert (0);
                break;

            default:
                assert (0);
        }
    }

    if (node->right)
    {
        fprintf (stderr, "RIGHTNODE %d %d\n", node->right->type, node->right->opValue);

        switch (node->right->type)
        {
            case OP_t:
                parseExp (node->right, stk, asmFile);
                break;

            case Var_t:
                parseExp (node->left, stk, asmFile);
                break;

            case Num_t:
                parseExp (node->right, stk, asmFile);
                break;

            case Key_t:
                if (strcmp (node->right->Name, "VAR") == 0)
                    parseVar (node->right, stk, asmFile);

                else if (strcmp (node->right->Name, "IF") == 0)
                    parseIf (node->right, stk, asmFile);

                else if (strcmp (node->right->Name, "WHILE") == 0)
                    parseWhile (node->right, stk, asmFile);

                else if (strcmp (node->right->Name, "ST") == 0)
                    parseSt (node->right, stk, 0, asmFile);
                
                else if (strcmp (node->right->Name, "CALL") == 0)
                    parseSt (node->right, stk, 0, asmFile);

                break;

            case Func_t:
                parseFunc (node->right, stk, 0, asmFile);
                break;

            case Unknown:
                fprintf (stderr, "Writer unknown %s\n", node->Name);
                assert (0);
                break;

            default:
                assert (0);
        }
    } 

    if (isLocal)
    {
        addRelativeShift (-relativeShift, asmFile);
        destroyTable (stk);
    }

}

int countVariables (Node* node)
{
    int num = 0;
    
    if (node->right)
        num += countVariables (node->right);
    if (node->left)
        num += countVariables (node->left);

    if (node->type == Key_t && strcmp ("VAR", node->Name) == 0)
        num += 1;

    return num;
}

void addRelativeShift (int shift, FILE* asmFile)
{
    fprintf (asmFile, "//relative shift\n\n");
    fprintf (asmFile, "    PUSH rax\n");
    fprintf (asmFile, "    PUSH %d\n", shift);
    fprintf (asmFile, "    ADD\n");
    fprintf (asmFile, "    POP rax\n");
    fprintf (asmFile, "//end of relative shift\n\n");
}

void parseIf (Node* node, Stack_t* stk, FILE* asmFile)
{
    printf ("PARSEIF\n");
    fprintf (asmFile, "//if\n\n");

    if (node->left)
        parseExp (node->left, stk, asmFile);
    else 
        assert (0);

    int curLabel = numberOfLabels;
    numberOfLabels += 1;
    int elseLabel = numberOfLabels;

    fprintf (asmFile, "    PUSH 0\n");
    fprintf (asmFile, "    JE if%d:\n", curLabel);

    if (node->right)
    {
        if (node->right->type == Key_t)
        {
            if (strcmp (node->right->Name, "ELSE") == 0)
            {
                if (node->right->left)
                {
                    parseSt(node->right->left, stk, 1, asmFile);
                    fprintf (asmFile, "JMP else%d: \n", elseLabel);
                    numberOfLabels += 1;
                }
                else
                {
                    printError ("Else dont have left son");
                    assert (0);
                }

                fprintf (asmFile, "if%d:\n", curLabel);

                if (node->right->right)
                {
                    parseSt(node->right->right, stk, 1, asmFile);
                    fprintf (asmFile, "else%d:\n", elseLabel);
                }
                else
                {
                    printError ("Else dont have right son");
                    assert (0);
                }

            }
            else
            {
                parseSt (node->right, stk, 1, asmFile); //Maybe mistakes
                fprintf (asmFile, "if%d:\n", curLabel);
            }
        }
        else
        {
            parseSt (node->right, stk, 1, asmFile); //Maybe mistakes
            fprintf (asmFile, "if%d:\n", curLabel);
        }
    }

    else 
    {
        printError ("IF HAS NO RIGHT Child");
        assert (0);
    }

    fprintf (asmFile, "//end of if\n\n");
}

void readInitParam (Node* node, Stack_t* stk, FILE* asmFile)
{
    if (node->type == Key_t)
    {
        if (node->left)
            parseVar (node->left, stk, asmFile);

        if (node->right)
            readInitParam (node->right, stk, asmFile);
    }
    else 
        assert (0);
}

void readCallParam (Node* node, Stack_t* stk, FILE* asmFile)
{
    if (node->type == Key_t)
    {
        if (node->right)
            readCallParam (node->right, stk, asmFile);

        if (node->left)
            parseExp (node->left, stk, asmFile);

    }
}

void parseFunc (Node* node, Stack_t* stk, int isLocal, FILE* asmFile)
{
    if (node->type == BuiltIn_t)
    {
        fprintf (asmFile, "//BUILTIN\n");
        if (node->opValue != OP_IN)
        {
            if (node->left)
                readCallParam (node->left, stk, asmFile);
            
            fprintf (asmFile, "CALL %s:\n", FullOpArray [node->opValue]);
        }
        else
        {
            
            fprintf (asmFile, "CALL %s:\n", FullOpArray [node->opValue]);
            fprintf (asmFile, "    POP [rax+%d]\n", -findInStack (node->left->left->var.varName, stk));
        }

        fprintf (asmFile, "//end of FUNC\n\n");

        
    }
    else if (strcmp (node->Name, "CALL") == 0)
    {
        fprintf (asmFile, "//FUNC\n\n");
        
        if (node->left)
        {
            if (node->left->left)
                readCallParam (node->left->left, stk, asmFile);


            fprintf (asmFile, "    CALL %s:\n", node->left->Name);

            fprintf (asmFile, "//end of FUNC\n\n");
        }

        else 
        {
            printError ("leftNode of func node doesn't exist");
            return;
        }

    }
    else if (strcmp (node->Name, "FUNC") == 0)
    {
        if (node->right)
        {
            if (!(node->right->left))
            {
                fprintf (asmFile, "//HERE WAS declaration of %s\n", node->left->Name);
                return;
            }
        }
        fprintf (asmFile, "//FUNC\n\n");
        int relativeShift = countVariables (node);

        if (node->left)
        {
            fprintf (asmFile, "%s:\n", node->left->Name);

            if (isLocal)
            {
                if (stk->size > 0)
                    addRelativeShift (relativeShift, asmFile);

                createTable (stk);
                printf ("CREATED new TABLE\n");
            }

            if (node->left->left)
                readInitParam (node->left->left, stk, asmFile);
        }
        else 
            assert (0);

        if (node->right)
            parseSt (node->right, stk, 0, asmFile);

        if (isLocal)
        {
            destroyTable (stk);
            if (stk->size > 0)
                addRelativeShift (-relativeShift, asmFile);
        }
        fprintf (asmFile, "    RET\n");

        fprintf (asmFile, "//end of FUNC\n\n");
    }


}

void parseWhile (Node* node, Stack_t* stk, FILE* asmFile)
{
    printf ("PARSEWHILE\n");
    fprintf (asmFile, "//while\n");
    int curLabel = numberOfLabels;
    numberOfLabels += 1;
    int curLabel2 = numberOfLabels;
    numberOfLabels += 1;

    fprintf (asmFile, "while%d:\n", curLabel2);
    if (node->left)
        parseExp (node->left, stk, asmFile);
    else 
        assert (0);


    fprintf (asmFile, "    PUSH 0\n");
    fprintf (asmFile, "    JE while%d:\n", curLabel);

    if (node->right)
    {
        parseSt (node->right, stk, 1, asmFile); //Maybe mistakes
    }
    else 
        assert (0);

    fprintf (asmFile, "    JMP while%d:\n", curLabel2);

    fprintf (asmFile, "while%d:\n", curLabel);

    fprintf (asmFile, "//End of While\n\n");
}

void parseEqual (Node* node, Stack_t* stk, FILE* asmFile)
{
    if (node->left)
    {
        if (node->left->type != Var_t)
            assert (0);

        if (node->right)
        {
            parseExp (node->right, stk, asmFile);
        }
        else 
            assert (0);

        fprintf (stderr, "Name %s\n", node->var.varName);
        int pos = findInStack (node->left->var.varName, stk); 
        fprintf (asmFile, "    POP [rax+%d]\n", -pos);
    }
    else 
        assert (0);
}

void parseExp (Node* node, Stack_t* stk, FILE* asmFile)
{
    printf("PARSEEXP\n");
    int pos = -1;

    if (node->opValue == OP_EQ)
    {
        parseEqual (node, stk, asmFile);
        return;
    }
    else if (node->type == Key_t)
        return;

    if (node->left)
        parseExp (node->left, stk, asmFile);

    if (node->right)
        parseExp (node->right, stk, asmFile);

    switch (node->type)
    {
        case OP_t:
            switch (node->opValue)
            {
                case OP_ADD:
                    fprintf (asmFile, "    ADD\n");
                    break;

                case OP_SUB:
                    fprintf (asmFile, "    SUB\n");
                    break;

                case OP_MUL:
                    fprintf (asmFile, "    MUL\n");
                    break;

                case OP_DIV:
                    fprintf (asmFile, "    DIV\n");
                    break;

                case OP_ABV:
                    fprintf (asmFile, "    IS_GT\n");
                    break;

                case OP_BLW:
                    fprintf (asmFile, "    IS_BT\n");
                    break;

                case OP_POW:
                    fprintf (asmFile, "    POW\n");
                    break;

                case OP_COS:
                    fprintf (asmFile, "    COS\n");
                    break;

                case OP_SIN:
                    fprintf (asmFile, "    SIN\n");
                    break;

                case OP_LOG:
                    fprintf (asmFile, "    LOG\n");
                    break;
                    
                case OP_LN:
                    fprintf (asmFile, "    LN\n");
                    break;
                    
                case OP_EQ:
                    fprintf (asmFile, "    EQ\n");
                    break;

                case OP_LBR:
                    printError ("BAD thing to write in to ASM");
                    assert (0);
                    break;

                case OP_RBR:
                    printError ("BAD thing to write in to ASM");
                    assert (0);
                    break;

                case OP_FRB:
                    printError ("BAD thing to write in to ASM");
                    assert (0);
                    break;

                case OP_FLB:
                    printError ("BAD thing to write in to ASM");
                    assert (0);
                    break;

                case OP_TER:
                    printError ("BAD thing to write in to ASM");
                    assert (0);
                    break;

                case OP_COM:
                    printError ("BAD thing to write in to ASM");
                    assert (0);
                    break;

                case UnknownOp:
                    printError ("Unknown op, dont know what to write in file");
                    break;

                default:
                    assert (0);
            }

            break;

        case Num_t:
            fprintf (asmFile, "    PUSH %lg\n", node->numValue);
            break;

        case Var_t:
            fprintf (stderr, "Name %s\n", node->var.varName);
            pos = findInStack (node->var.varName, stk); 
            fprintf (asmFile, "    PUSH [rax+%d] // parseExp\n", -pos);
            break;

        case Func_t:
            parseFunc (node, stk, 1, asmFile);
            break;

        case Key_t:
            printError ("Key_t, curNode type %d, opValue %d\n, name %s function exp", node->type, node->opValue, node->Name);
            break;

        case Unknown:
            printError ("Unknown type of node, WTF?\n");
            break;

        default: 
            printError ("default, curNode type %d, opValue %d\n, function exp", node->type, node->opValue);


    }

    fprintf (stderr, "end of parse ex\n");
}

void tableDump (NameTable* nameTable)
{
    for (int index = 0; index < nameTable->numOfVars; ++index)
    {
        fprintf (stderr, "Name %s pos %d \n", nameTable->data[index].name, nameTable->data[index].position);
    }
}

int findInStack (char* name, Stack_t* stk)
{
    int pos = 0;

    fprintf (stderr, "LOOKING FOR %s\n", name);

    for (size_t index = stk->size - 1; (int) index >= 0 ; index--)
    {
        fprintf (stderr, "TABLE %lu\n", index);
        fprintf (stderr, "POS %d\n", pos);
        tableDump (stk->data[index]);
        if (findInTable (name, stk->data[index]->data))
        {
            pos += findInTable (name, stk->data[index]->data)->position;
            return pos;
        }
        else 
        {
            pos += stk->data[index]->numOfVars; 
        }

    }

    return -1;
}

void parseVar (Node* node, Stack_t* stk, FILE* asmFile)
{
    printf("PARSEVAR\n");

    int pos = 0;

    if (node->left)
    {
        NameTable* curTable = stackLast (stk);
        assert (curTable != nullptr);

        fprintf (stderr, "name in parse var %s\n", node->left->var.varName);
        Name* name = tableAdd (node->left->var.varName, curTable->data);
        if (name == nullptr)
        {
            printError ("Name already in table");
            return;
        }
        curTable->numOfVars += 1;
        name->position = curTable->numOfVars - 1;

        pos = name->position;
    }
    else 
        assert (0);

    if (node->right)
    {
        parseExp (node->right, stk, asmFile);
    }

    fprintf (asmFile, "    POP [rax+%d]\n", -pos);

    fprintf (stderr, "end of PARSEVAR\n");
}

