#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void *Entries;
    int Count;
    int Capacity;
    size_t EntrySize;
    size_t KeySize;
    bool (*Compare)(const void *, const void *);
} Map;

void InitMap(Map *map, size_t entrySize, size_t keySize, bool (*compare)(const void *, const void *)) {
    map->Entries = 0;
    map->Count = 0;
    map->Capacity = 0;
    map->EntrySize = NULL;
    map->KeySize = keySize;
    map->Compare = compare;
}




char InputText[] = "Temp";

#define get_type(type) _Generic((type), \
    int: "int", \
    float: "float", \
    double: "double", \
    char *: "string", \
    default: "unknown")

char *CreatedVars[0] = {};

int InputedTextLength = sizeof(InputText) - 2;
char Setting[] = "Cont";//Cont means Continue
char CurString[] = "";

void ReturnError(char *ErrorMessage) {
    printf("Error: %s\n", ErrorMessage);
    return;
}

char CurrentlyCreatingVar[] = "";


for (int i = 0; i <= InputedTextLength ; i++ ) {
    CurString = strcat(CurString,InputText[i]);
    
    switch(Setting) {

        case "MultilineCommenting":
         if (strstr(CurString,"~|") != NULL) {
            Setting = "Cont";
            CurString = "";
        }
        break;

        case "Commenting":
         if (strstr(CurString,"\t") != NULL) {
            Setting = "Cont";
            CurString = "";
        }

        break;
            case "Log":
            if (strstr(CurString,")") != NULL) {
            //I need to implement the ability to make vars
            Setting = "Cont";
            CurString = "";
        }
        break;
        case "SpaceVar":
            if (strstr(CurString," ") != NULL) {
            Setting = "NameVar";
            CurString = "";
        }
        break;
        case "NameVar":
            if (strstr(CurString," ") != NULL) {
            Setting = "TypeVarVal";
            CurrentlyCreatingVar = CurString;
            CurString = "";
        }
        break;
        case "TypeVarVal":
            if (strstr(CurString," ") != NULL) {
            Setting = "ValVar";
            CurString = "";
        }

    }

    switch (CurString) {

        case "/t":
         if (strcamp(Setting, "Commenting") == 0) {// If the setting is "Commenting", we ignore untill the tab character
            Setting = "Cont";
        }
        break;
        case "Func":
            printf("Keyword: Func\n");
            break;
            case "Var":
            printf("Keyword: Var\n");

            Setting = "SpaceVar";

            break;
        case "For":
            printf("Keyword: For\n");
            break;
        case "While":
            printf("Keyword: While\n");
            break;
        case "If":
            printf("Keyword: If\n");
            break;
        case "Do":
            printf("Keyword: Do\n");
            break;
        case "Then":
            printf("Keyword: Then\n");
            break;
        case "End":
            printf("Keyword: End\n");
            break;
            case  "~":
            printf("Keyword: ~\n");
            Setting = "Commenting";
            break;
        case "|~":
            printf("Keyword: |~\n");
            Setting = "MultilineCommenting";
            break;
        case "Elseif":
            printf("Keyword: Elseif\n");
            break;
        case "Log":
            printf("Keyword: Log(\n");
            Setting = "Log";
            CurString = "";
            break;
        case "Arr":
            printf("Keyword: Arr\n");
            break;
    }
};
