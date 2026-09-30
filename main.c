#include <stdio.h>

char InputText[] = "Temp";

#define get_type(type) _Generic((type), \
    int: "int", \
    float: "float", \
    double: "double", \
    char *: "string", \
    default: "unknown")

const char *CreatedVars[0] = {};

int InputedTextLength = sizeof(InputText) - 2;
char Setting[] = "Cont";//Cont means Continue
char CurString[] = "";

int ReturnError(char *ErrorMessage) {
    printf("Error: %s\n", ErrorMessage);
    return 0;
}

for (int i = 0; i <= InputedTextLength ; i++ ) {
    CurString = strcat(CurString,InputText[i]);
    
    if (strcamp(Setting, "Log") == 0) {
        if (strstr(CurString,")") != NULL) {

            //I need to implement the ability to make vars


            Setting = "Cont";
            CurString = "";
            continue;
        }
    }

    if (strcamp(Setting, "MultilineCommenting") == 0) {
        if (strstr(CurString,"~|") != NULL) {
            Setting = "Cont";
            CurString = "";
            continue;
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
