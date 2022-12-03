#include <cassert>
#include "../common.h"
#include "analyzer.h"

Node* nodeCtor ()
{
    Node* newDataPtr = (Node*) calloc (1, sizeof(*newDataPtr));
    assert (newDataPtr != nullptr);

    newDataPtr->type     = Unknown;
    newDataPtr->opValue  = UnknownOp;
    newDataPtr->numValue = 0;
    newDataPtr->var      = {};
    newDataPtr->left     = nullptr;
    newDataPtr->right    = nullptr;

    return newDataPtr;
}

void nodeDtor (Node* node)
{
    assert (node != nullptr);
    if (!node) return;

    node->type     = Unknown;
    node->opValue  = UnknownOp;
    node->numValue = 0;
    node->var      = {};
    node->left     = nullptr;
    node->right    = nullptr;

    free (node);
}

Node* createNode (Type type, OP opValue, double numValue, char* varName, char* Name, Node* left, Node* right)
{
    Node* newNode = nodeCtor ();

    newNode->type         = type;
    newNode->left         = left;
    newNode->right        = right;
    newNode->numValue     = numValue;
    newNode->Name         = Name;
    newNode->var.varName  = varName;
    newNode->opValue      = opValue;
    
    return newNode;
}

int strEqual(const char *l, const char *r)
{
    if (l == nullptr || r == nullptr) return 0;

    for (int i = 0; l[i] != '\0' && r[i] != '\0'; i++)
        if (l[i] != r[i]) return 0;

    return 1;
}
