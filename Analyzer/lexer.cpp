#include <stdio.h>
#include <math.h>
#include <cassert>
#include <cstring>
#include <ctype.h>
#include "../common.h"
#include "analyzer.h"

Node* getN (Node*** tokenArray)
{
    double val = 0;
    int isMinus = 0;
 
    if ((***tokenArray).opValue == OP_SUB)
    {
         isMinus = 1;
         *tokenArray += 1;
    }

    assert ((***tokenArray).type == Num_t);

    val = (***tokenArray).numValue;
    printf ("VAL %lf\n", val);
    *tokenArray += 1;


     if (isMinus) 
        return createNum (-val);
     else
        return createNum (val);
}

Node* getT (Node*** tokenArray)
{
    Node* val = getPW (tokenArray);

    while ((***tokenArray).opValue == OP_DIV || (***tokenArray).opValue == OP_MUL)
    {
        int op = (***tokenArray).opValue;

        *tokenArray += 1;

        Node* val2 = getPW (tokenArray);

        if (op == OP_MUL)
            val = MUL (val, val2);            
        else
            val = DIV (val, val2);
    }

    return val;
}

Node* getE (Node*** tokenArray)
{
    Node* val = getT (tokenArray);

    while ((***tokenArray).opValue == OP_ADD || (***tokenArray).opValue == OP_SUB)
    {
        int op = (***tokenArray).opValue;

        *tokenArray += 1;

        Node* val2 = getT (tokenArray);

        if (op == OP_ADD)
            val = ADD (val, val2);
        else 
            val = SUB (val, val2);
    }

    return val;
}

Node* getP (Node*** tokenArray)
{ Node* val = nullptr;
    if ((***tokenArray).opValue == OP_LBR)
    {
        *tokenArray += 1;
        val = getE (tokenArray);
        assert ((***tokenArray).opValue == OP_RBR);
        *tokenArray += 1;
    }
    else 
    {
       val = getUnOP (tokenArray);
    }

    return val;
}

Node* getPW (Node*** tokenArray)
{
    Node* val = getP (tokenArray);

    if (**tokenArray == nullptr) 
    {
        return val;
    }

    while ((***tokenArray).opValue == OP_POW)
    {
        *tokenArray += 1;
        val = POW (val, getP (tokenArray));

        return val;
    }

    return val;
}

Node* getVar (Node*** tokenArray)
{
    assert (tokenArray != nullptr);
    Node* val = nullptr;

    if ((***tokenArray).type == Var_t)
    {
       tableInsert ((***tokenArray).varName);
       val = VAR((***tokenArray).varName);
       *tokenArray += 1;
        
    }

    return val;
}

Node* getUnOP (Node*** tokenArray)
{
    Node* val  = nullptr;

    if ((***tokenArray).type == OP_t)
    {
        switch ((***tokenArray).opValue)
        {
            case OP_SIN:
                *tokenArray += 1;
                val = SIN (getP (tokenArray));
                break;
            
            case OP_COS:
                *tokenArray += 1;
                val = COS (getP (tokenArray));
                break;
        }

    }
    else if ((***tokenArray).type == Var_t)
    {
        val = getVar (tokenArray);
    }
    else if ((***tokenArray).type == Num_t)
    {
        val = getN (tokenArray);
    }

    return val;
}
Node* getG (Node*** tokenArray) 
{
    Node* val = getE (tokenArray);
    return val;

}

int power(int base, int n)
{
    int p = 1;

    for (int i = 1; i <= n; ++i)
        p = p * base;

    return p;
}

int strEqual(const char *l, const char *r)
{
    if (l == nullptr || r == nullptr) return 0;

    for (int i = 0; l[i] != '\0' && r[i] != '\0'; i++)
        if (l[i] != r[i]) return 0;

    return 1;
}

