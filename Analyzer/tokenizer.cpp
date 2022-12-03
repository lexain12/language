#include <cassert>
#include <ctype.h>
#include <cstring>
#include <stdio.h>
#include "../common.h"
#include "analyzer.h"


const size_t MAXDATASIZE   = 20;

void getBOpToken (Node*** tokenArray, char** string)
{
    switch (**string)
    {
        case '+':
            **tokenArray = ADD (nullptr, nullptr);
            break;
        
        case '-':
            **tokenArray = SUB (nullptr, nullptr);
            break;
        
        case '*':
            **tokenArray = MUL (nullptr, nullptr);
            break;
        
        case '/':
            **tokenArray = DIV (nullptr, nullptr);
            break;
        
        case '^':
            **tokenArray = POW (nullptr, nullptr);
            break;
        
        case '(':
            **tokenArray = LBR ();
            break;
        
        case ')':
            **tokenArray = RBR ();
            break;

        case '}':
            **tokenArray = FRB ();
            break;

        case '{':
            **tokenArray = FLB ();
            break;
        case ';':
            **tokenArray = TER ();
            break;

        default:
            assert (0);
    }

    *tokenArray += 1;
    *string += 1;
}

void skipSpaces (char** string)
{
    while (isspace(**string))
    {
        *string += 1;
    }
}

void getTokens (Node** tokenArray, char* string)
{
    while (*string != '\0')
    {
        if (isspace (*string))
            skipSpaces (&string);

        else if (strchr ("+-*/^(){}", *string))
        {
            getBOpToken (&tokenArray, &string);
        }

        else if (isdigit (*string))
        {
            double number = 0;
            int numOfScanf = 0;
            sscanf (string, "%lf%n", &number, &numOfScanf);

            *tokenArray = createNum (number);
            tokenArray++;
            string += numOfScanf;
        }

        else if (isalpha (*string))
        {
            getOpOrWordToken (&tokenArray, &string);
        }

    }

    *tokenArray = nodeCtor ();
}

void getOpOrWordToken (Node*** tokenArray, char** string)
{
    char* ptr = *string;

    if (strEqual (ptr, "sin"))
    {
        *string += 3;
        **tokenArray = SIN (nullptr);
        *tokenArray += 1;
    }
    else if (strEqual (ptr, "cos"))
    {
        *string += 3;
        **tokenArray = COS (nullptr);
        *tokenArray += 1;
    }
    else if (strEqual (ptr, "ln"))
    {
        *string += 2;
        **tokenArray = LN (nullptr);
        *tokenArray += 1;
    }
    else if (strEqual (ptr, "log"))
    {
        *string += 3;
        **tokenArray = LOG (nullptr, nullptr);
        *tokenArray += 1;
    }
    else 
    {
        char*  data = (char*) calloc (MAXDATASIZE, sizeof(*data));
        int numOfScan = 0;

        sscanf (*string, "%[^()-*+/^{}]%n", data, &numOfScan);

        assert (numOfScan != 0);
        *string += numOfScan;

        **tokenArray = WORD (data);
        *tokenArray += 1;
    }
}
