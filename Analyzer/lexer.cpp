#include <stdio.h>
#include <math.h>
#include <cassert>
#include <cstring>
#include <ctype.h>
#include "../common.h"
#include "analyzer.h"
#include "../utils/include/ErrorHandlerLib.h"

Node* getN (Utility* utils)
{

    double val = 0;
    int isMinus = 0;
 
    if ((**(utils->tokenArray)).opValue == OP_SUB)
    {
         isMinus = 1;
         utils->tokenArray += 1;
    }

    if ((**(utils->tokenArray)).type != Num_t)
    {
        if (isMinus) utils->tokenArray -= 1;

        return nullptr;
    }

    val = (**(utils->tokenArray)).numValue;
    utils->tokenArray += 1;


     if (isMinus) 
        return createNum (-val);
     else
        return createNum (val);
}

Node* getT (Utility* utils)
{

    Node* val = getP (utils);

    while ((**(utils->tokenArray)).opValue == OP_DIV || (**(utils->tokenArray)).opValue == OP_MUL)
    {
        int op = (**(utils->tokenArray)).opValue;

        utils->tokenArray += 1;

        Node* val2 = getP (utils);

        if (op == OP_MUL)
            val = MUL (val, val2);            
        else
            val = DIV (val, val2);

    }

    return val;
}

Node* getE (Utility* utils)
{

    assert (utils->tokenArray != nullptr);

    Node* val = getT (utils);

    while ((**(utils->tokenArray)).opValue == OP_ADD || (**(utils->tokenArray)).opValue == OP_SUB)
    {
        int op = (**(utils->tokenArray)).opValue;

        utils->tokenArray += 1;

        Node* val2 = getT (utils);

        if (op == OP_ADD)
            val = ADD (val, val2);
        else 
            val = SUB (val, val2);

    }

    return val;
}

Node* getP (Utility* utils)
{ 

    assert (utils->tokenArray != nullptr);

    Node* val = nullptr;

    if ((**(utils->tokenArray)).opValue == OP_LBR)
    {
        utils->tokenArray += 1;
        val = getE (utils);
        assert ((**(utils->tokenArray)).opValue == OP_RBR);
        utils->tokenArray += 1;
    }
    else if ((val = getVar (utils))) {}

    else if ((val = getN (utils))) {}

    else if ((val = getFunction (utils))) {}

    return val;
}

Node* getVar (Utility* utils)
{

    assert (utils->tokenArray != nullptr);
    Node* val = nullptr;

    if ((**(utils->tokenArray)).type == Unknown) 
    {
        if ((**(utils->tokenArray + 1)).opValue != OP_LBR)
        {

            val = *(utils->tokenArray);
            val->type = Var_t;
            val->var.varName = val->Name;

            utils->tokenArray += 1;
        }
        else 
            return nullptr;
    }

    return val;
}

Node* getCondition (Utility* utils)
{
    assert (utils->tokenArray != nullptr);

    Node* val = getE (utils);

    if ((**(utils->tokenArray)).opValue == OP_ABV ||
        (**(utils->tokenArray)).opValue == OP_BLW)
    {
        int op = (**(utils->tokenArray)).opValue; 

        utils->tokenArray += 1;

        Node* val2 = getE (utils);

        if (op == OP_ABV)
            return ABV(val, val2);
        else 
            return BLW(val, val2);
    }
    return val;
}

Node* getFunction (Utility* utils)
{
    
    assert (utils->tokenArray != nullptr); 

    Node* val = nullptr;


    if ((**(utils->tokenArray)).type == Key_t)
    {

        if (strcmp ((**(utils->tokenArray)).Name, "new") == 0)
        {
            utils->tokenArray += 1;
        }
        else 
        {
            return nullptr;
        }

        if ((**(utils->tokenArray)).type == Key_t)
        {
            if (strcmp ((**(utils->tokenArray)).Name, "technique") == 0)
            {
                utils->tokenArray += 1;

                val = *(utils->tokenArray);
                val->type = Func_t;
                Name* curName = findInTable (val->Name, utils->nameTable.data);

                curName->type = FUNC_TYPE;

                val = FUNC(val, nullptr);

                utils->tokenArray += 1;
            }
            if ((**(utils->tokenArray)).opValue == OP_LBR)
            {
                utils->tokenArray += 1;

                if ((**(utils->tokenArray)).type == Key_t)
                {
                    Node* curNode = nullptr;

                    if (strcmp ((**(utils->tokenArray)).Name, "var") == 0)
                    {
                        utils->tokenArray += 1;
                        curNode = val->left;
                        
                        curNode->left = PARAM(WORD("VAR"), nullptr);
                        curNode->left->left->type = Key_t;
                        curNode->left->left->left = getVar (utils);

                        curNode = curNode->left;
                    }

                    while ((**(utils->tokenArray)).opValue == OP_COM)
                    {
                        utils->tokenArray += 1;

                        if (strcmp ((**(utils->tokenArray)).Name, "var") == 0)
                        {
                            utils->tokenArray += 1;

                            curNode->right = PARAM(WORD("VAR"), nullptr);
                            curNode->right->left->type = Key_t;
                            curNode->right->left->left = getVar (utils);
                        }

                        curNode = curNode->right;
                    }
                }
                 
            }
            else 
                assert (0);

            if ((**(utils->tokenArray)).opValue == OP_RBR)
                utils->tokenArray += 1;
            else 
                assert (0);

            if ((**(utils->tokenArray)).opValue == OP_FLB)
            {
                utils->tokenArray += 1;

                val->right = ST(getStatement (utils), nullptr);
                val->right->type = Key_t;


                Node* curNode = val->right;
                Node* newNode = nullptr;

                while ((newNode = getStatement (utils)))
                {
                    curNode->right = ST(newNode, nullptr);
                    curNode = curNode->right;
                }
            }
            else assert (0);

            if ((**(utils->tokenArray)).opValue == OP_FRB)
            {
                utils->tokenArray += 1;

                return val;
            }
            else assert (0);
            
        }
    }
    else if ((**(utils->tokenArray)).type == Unknown)
    {
        char* tokenName = (**(utils->tokenArray)).Name;
        Name* curName = nullptr;

        if ((curName = findInTable (tokenName, utils->nameTable.data)))
        {
            if (curName->type == FUNC_TYPE)
            {
            
                val = *(utils->tokenArray);
                val->type = Func_t;

                val = FUNC(val, nullptr);
                fprintf (stderr, "NAME %s\n", val->left->Name);

                val->Name = "CALL";

                utils->tokenArray += 1;
            }
            if ((**(utils->tokenArray)).opValue == OP_LBR)
            {
                utils->tokenArray += 1;

                Node* curNode = nullptr;

                if ((**(utils->tokenArray)).opValue != OP_RBR)
                {
                    curNode = val->left;
                    
                    curNode->left = PARAM (getE (utils), nullptr);

                    curNode = curNode->left;

                    while ((**(utils->tokenArray)).opValue == OP_COM)
                    {
                        utils->tokenArray += 1;

                        curNode->right = PARAM(getE (utils), nullptr);

                        curNode = curNode->right;
                    }
                }
            }
            else 
            {
                printError ("No left bracket while calling function %s", val->left->Name);
                assert (0);
            }

            if ((**(utils->tokenArray)).opValue == OP_RBR)
                utils->tokenArray += 1;
            else 
                assert (0);
            
        }
    }

    else if ((**(utils->tokenArray)).type == BuiltIn_t)
    {
        fprintf (stderr, "Was here\n");
        Name* curName = nullptr;
        Node* curNode = nullptr;
        val = *(utils->tokenArray);

        utils->tokenArray += 1;

        if ((**(utils->tokenArray)).opValue == OP_LBR)
        {
            utils->tokenArray += 1;

            if ((**(utils->tokenArray)).opValue != OP_RBR)
            {
                curNode = val;
                
                curNode->left = PARAM (getE (utils), nullptr);

                curNode = curNode->left;

                while ((**(utils->tokenArray)).opValue == OP_COM)
                {
                    utils->tokenArray += 1;

                    curNode->right = PARAM(getE (utils), nullptr);

                    curNode = curNode->right;
                }
            }
        }

        if ((**(utils->tokenArray)).opValue == OP_RBR)
            utils->tokenArray += 1;
        else 
            assert (0);

    }


    return val;
}


Node* getStatement (Utility* utils)
{
    
    if ((**(utils->tokenArray)).type == Unknown && (**(utils->tokenArray)).Name == nullptr)
        return nullptr;

    Node *val = nullptr;

    if ((val = getVar (utils)))
    {

        val = EQ(val, getE (utils));


        if ((**(utils->tokenArray)).opValue == OP_EQ)
            utils->tokenArray += 1;
        else 
            assert (0);

        if ((**(utils->tokenArray)).opValue == OP_TER)
            utils->tokenArray += 1;

        else 
            assert (0);

        return val;
    }

    else if ((val = getE (utils)))
    {

        if ((**(utils->tokenArray)).type == OP_t)
        {
            if ((**(utils->tokenArray)).opValue == OP_TER)
            {
                utils->tokenArray += 1;
                return val;
            }
            else 
                assert (0);
        }
        else 
        {
            printError ("No ; in the end of statement");
            assert (0);
        }
    }

    else if ((**(utils->tokenArray)).type == Key_t)
    {
        if (strcmp ((**(utils->tokenArray)).Name, "return") == 0)
        {
            utils->tokenArray += 1;
            val = WORD("RET");
            val->type = Key_t;
            

            val->left = getE (utils);

            if ((**(utils->tokenArray)).opValue == OP_TER)
                utils->tokenArray += 1;
            else 
                assert (0);

            return val;
        }

        else if (strcmp ((**(utils->tokenArray)).Name, "var") == 0)
        {
            utils->tokenArray += 1;
            val = WORD("VAR");
            val->type = Key_t;

            val->left = getVar (utils);

            val->right = getE (utils);

            if ((**(utils->tokenArray)).opValue == OP_EQ)
            {
                utils->tokenArray += 1;

                if ((**(utils->tokenArray)).opValue == OP_TER)
                {
                    utils->tokenArray += 1;
                    return val;
                }
                else 
                {
                    printError ("No ; in the end of statement");
                    assert (0);
                }
            }

            printError ("NO hp in the end of inititalization");
            assert (0);
        }

        else if (strcmp ((**(utils->tokenArray)).Name, "Ccombo!!") == 0)
        {
            utils->tokenArray += 1;
            if ((**(utils->tokenArray)).opValue == OP_LBR)
            {
                utils->tokenArray += 1;
                val = WORD("WHILE");
                val->type = Key_t;
                val->left = getCondition (utils);
            }
            else assert (0);

            if ((**(utils->tokenArray)).opValue == OP_RBR)
            {
                utils->tokenArray += 1;
            }
            else assert (0);
            
            if ((**(utils->tokenArray)).opValue == OP_FLB)
            {
                utils->tokenArray += 1;

                val->right = ST(getStatement (utils), nullptr);
                val->right->type = Key_t;


                Node* curNode = val->right;
                Node* newNode = nullptr;

               while ((newNode = getStatement (utils)))
                {
                    curNode->right = ST(newNode, nullptr);
                    curNode = curNode->right;
                }
            }
            else assert (0);

            if ((**(utils->tokenArray)).opValue == OP_FRB)
            {
                utils->tokenArray += 1;

                return val;
            }
            else assert (0);
        }

        else if (strcmp ((**(utils->tokenArray)).Name, "right") == 0)
        {
            utils->tokenArray += 1;
            if ((**(utils->tokenArray)).opValue == OP_LBR)
            {
                utils->tokenArray += 1;
                val = WORD("IF");
                val->type = Key_t;
                val->left = getCondition (utils);
            }
            else assert (0);

            if ((**(utils->tokenArray)).opValue == OP_RBR)
            {
                utils->tokenArray += 1;
            }
            else assert (0);
            
            if ((**(utils->tokenArray)).opValue == OP_FLB)
            {
                utils->tokenArray += 1;

                val->right = ST(getStatement (utils), nullptr);
                val->right->type = Key_t;


                Node* curNode = val->right;
                Node* newNode = nullptr;

                while ((newNode = getStatement (utils)))
                {
                    curNode->right = ST(newNode, nullptr);
                    curNode = curNode->right;
                }
            }
            else assert (0);

            if ((**(utils->tokenArray)).opValue == OP_FRB)
            {
                utils->tokenArray += 1;
            }
            else assert (0);

            if ((**(utils->tokenArray)).type == Key_t)
            {
                Node* elseNode = nullptr;

                if (strcmp((**utils->tokenArray).Name, "left") == 0)
                {
                    utils->tokenArray += 1;
                    elseNode = ELSE(val->right, nullptr);

                    if ((**(utils->tokenArray)).opValue == OP_FLB)
                    {
                        utils->tokenArray += 1;

                        elseNode->right = ST(getStatement (utils), nullptr);
                        elseNode->right->type = Key_t;


                        Node* curNode = elseNode->right;
                        Node* newNode = nullptr;

                        while ((newNode = getStatement (utils)))
                        {
                            curNode->right = ST(newNode, nullptr);
                            curNode = curNode->right;
                        }
                        val->right = elseNode;
                    }
                    else 
                    {
                        printError ("No { in else statement");
                        assert (0);
                    }
                    if ((**(utils->tokenArray)).opValue == OP_FRB)
                    {
                        utils->tokenArray += 1;
                    }
                    else 
                    {
                        printError ("No } in else statement");
                        assert (0);
                    }
                }

            }
            return val;
        }

        else if (strcmp ((**(utils->tokenArray)).Name, "K.O.") == 0)
        {
            utils->tokenArray += 1;
            val = WORD("RET");
            val->type = Key_t;
            
            val->left = getE (utils);
            if ((**(utils->tokenArray)).opValue == OP_TER)
                utils->tokenArray += 1;
            else assert (0);

            return val;
        }

        assert (0);
    }

    return val;
}

Node* getG (Utility* utils) 
{

    assert (utils->tokenArray != nullptr);

    Node* mainNode = getStatement (utils);
    mainNode = ST(mainNode, nullptr);
    mainNode->type = Key_t;

    Node* curNode = mainNode;
    Node* newNode = nullptr;


    while ((newNode = getStatement (utils)))
    {
        curNode->right = ST(newNode, nullptr);
        curNode = curNode->right;
        curNode->type = Key_t;
    }

    return mainNode;

}

void includeStdLib (Name* data, FILE* asmFile)
{
    if (data == nullptr)
    {
        printError ("function includeStdLib, data has nullptr");
        assert (0);
    }
    if (asmFile ==  nullptr)
    {
        printError ("function includeStdLib, file has nullptr");
        assert (0);
    }

    Name* curName = tableAdd ("in", data);

    if (curName == nullptr)
    {
        printError ("Name \"in\" already in table");
        assert (0);
    }
    curName->type = FUNC_TYPE;

    printFuncIn (asmFile);

    curName = tableAdd ("print", data);

    if (curName == nullptr)
    {
        printError ("Name \"print\" already in table");
        assert (0);
    }

    curName->type = FUNC_TYPE;

    printFuncPrint (asmFile);

    curName = tableAdd ("sqrt", data);

    if (curName == nullptr)
    {
        printError ("Name \"sqrt\" already in table");
        assert (0);
    }

    curName->type = FUNC_TYPE;

    printFuncSqrt (asmFile);

    curName = tableAdd ("abs", data);

    if (curName == nullptr)
    {
        printError ("Name \"abs\" already in table");
        assert (0);
    }

    curName->type = FUNC_TYPE;

    printFuncAbs (asmFile);

}

void printFuncIn (FILE* asmFile)
{
    fprintf (asmFile, "IN:\n");
    fprintf (asmFile, "    IN\n");
    fprintf (asmFile, "    RET\n");
}

void printFuncPrint (FILE* asmFile)
{
    fprintf (asmFile, "OUT:\n");
    fprintf (asmFile, "    OUT\n");
    fprintf (asmFile, "    RET\n");
}
void printFuncSqrt (FILE* asmFile)
{
    fprintf (asmFile, "SQRT:\n");
    fprintf (asmFile, "    SQRT\n");
    fprintf (asmFile, "    RET\n");
}

void printFuncAbs (FILE* asmFile)
{
    fprintf (asmFile, "ABS:\n");
    fprintf (asmFile, "ABS\n");
    fprintf (asmFile, "RET\n");
}

void initAsmFile (FILE* asmFile)
{
    if (asmFile == nullptr)
    {
        printError ("asmFile has nullptr in function initAsmFile");
        assert (0);
    }

    fprintf (asmFile, "CALL main:\n");
    fprintf (asmFile, "HLT\n");
}

