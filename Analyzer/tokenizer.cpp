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

void getTokens (Utility utils, char* string)
{
    while (*string != '\0')
    {
        if (isspace (*string))
            skipSpaces (&string);

        else if (strchr ("+-*/^(){};", *string))
        {
            getBOpToken (&(utils.tokenArray), &string);
        }

        else if (isdigit (*string))
        {
            double number = 0;
            int numOfScanf = 0;
            sscanf (string, "%lf%n", &number, &numOfScanf);

            *(utils.tokenArray) = createNum (number);
            utils.tokenArray++;
            string += numOfScanf;
        }

        else if (isalpha (*string))
        {
            getOpOrWordToken (&(utils.tokenArray), &string, utils);
        }

    }

    *(utils.tokenArray) = nodeCtor ();
}

void getOpOrWordToken (Node*** tokenArray, char** string, Utility utils)
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

        sscanf (*string, "%[^()-*+/^{}\n ;]%n", data, &numOfScan);

        if (!tableAdd (data, utils.nameTable))
        {
            char* newData = findInTable (data, utils.nameTable);
            free (data);
            data = newData;
        }

        fprintf (stderr, "%s\n", data);

        assert (numOfScan != 0);
        *string += numOfScan;

        **tokenArray = WORD (data);
        *tokenArray += 1;
        printf ("Ok\n");
    }
}

char* findInTable (char* name, Name* table)
{
    assert (name != nullptr);
    assert (table != nullptr);

    for (size_t index = 0; index < NUMOFNAMES && table[index].name; ++index)
    {
        if (strcmp (table[index].name, name) == 0)
        {
            return table[index].name;
        }
    }

    return nullptr;
}

int tableAdd (char* name, Name* table)
{
    assert (name != nullptr);
    assert (table != nullptr);

    char* nameInTable = findInTable (name, table);

    if (nameInTable)
    {
        return 0;
    }

    else
    {
        for (size_t index = 0; index < NUMOFNAMES; ++index)
        {
            if (table[index].name == nullptr)
            {
                table[index].name = name;
                return 1;
            }
        }

        assert (0);
        return 0;
    }
    
}
