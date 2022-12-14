#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "../include/ErrorHandlerLib.h"
#include "../include/consoleColorLib.h"

void printErrorFunc (const char *message, ...)
{
    if (message == nullptr)
    {
        setForegroundColor (RED);
        printf ("ERROR: printError got bad message\n");
        resetConsole ();
        return;
    }

    va_list args = {};

    va_start(args, message);

    setForegroundColor (RED);

    printf ("ERROR:\n");

    resetConsole ();

    vprintf (message, args);

    putchar ('\n');

}

