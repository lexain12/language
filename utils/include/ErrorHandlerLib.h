#include <stdio.h>
void printErrorFunc (const char *message, ...);

#define printError(...)                                                        \
do                                                                                      \
{                                                                                       \
    printf ("---------------------------------------------------------\n");             \
    printf ("FILE: %s, Function: %s, Line: %d\n",__FILE__,  __func__, __LINE__);     \
                                                                                        \
    printErrorFunc (__VA_ARGS__ );                                              \
                                                                                        \
    printf ("---------------------------------------------------------\n");             \
} while (0)

