#include <stdio.h>

char InputText[] = "Temp";

const char *Keywords[13] = {
    "Func",
    "For",
    "While",
    "If",
    "Do",
    "Then",
    "End",
    "~",
    "|~",
    "~|",
    "Elseif",
    "Log",
    "Arr"
};

int InputedTextLength = sizeof(InputText) - 2;
