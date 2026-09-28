#include <stdio.h>

char InputText[] = "Temp";

const char *Keywords[13] = {
    "Func",
    "For",
    "While",
    "If",
    "Do",`                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
    "Then",
    "End",
    "~",
    "|~",
    "~|",
    "Elseif",
    "Log",
    "Arr"
};

const char *CreatedVars[0] = {};

int InputedTextLength = sizeof(InputText) - 2;
char Setting[] = "Cont";
char CurString[] = "";


for (int i = 0; i <= length ; i++ ) {
    CurString = strcat(CurString,InputText[i]);
};
