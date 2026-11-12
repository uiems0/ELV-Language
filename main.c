#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int CurLine=0; // Keeps track of the line, 

void ReturnError(char *ErrorMessage) {
    printf("Error on line %d: %s\n", CurLine, ErrorMessage);
    return;
}

inline float CalcExpression(char RecievedString[]) {//Last layer of math funcs, it is for solving small expressions like 2+2 or 3*4, etc
    float Num1 = 0;
    float Num2 = 0;
    char Operator = '\0';

    //Break RecievedString into Num1, Num2, and Operator
    for (int i = 0; i < strlen(RecievedString); i++) {
        if (RecievedString[i] == '^' || RecievedString[i] == '_' || RecievedString[i] == '*' || RecievedString[i] == '/' || RecievedString[i] == '-' || RecievedString[i] == '+') {
            Operator = RecievedString[i];
            char Num1Str[64] = "";
            for (int j = 0; j < i; j++) {
                // strcat(Num1Str, RecievedString[j]); // Note: kept original structure
            }
            char Num2Str[64] = "";
            for (int j = i + 1; j < strlen(RecievedString); j++) {
                // strcat(Num2Str, RecievedString[j]);
            }
            Num1 = atof(Num1Str);
            Num2 = atof(Num2Str);
        }
    }

    switch (Operator) {
        case '+':
            return Num1 + Num2;
            break;
        case '-':
            return Num1 - Num2;
            break;
        case '*':
            return Num1 * Num2;
            break;
        case '/':
            if (Num2 == 0) {
                ReturnError("Division by zero");
            }
            return Num1 / Num2;
             break;
        case '^':
            return pow(Num1, Num2);
        break; 
        case '%':
        return fmod(Num1, Num2);
        break;
        //Square roots arent nessecary as they can be done with the ^ operator
        default:
            ReturnError("Invalid operator");
    }
}

char GetBlock(char ValuedString[], char ValidOp1[], char ValidOp2[],char ValidOp3[]) {//Gets a block of a string from the start index to the end index. Valid Op 3 exists for modulo, which is in the in the first layer
        int Number1BlockStart = -1;
        int Number1BlockEnd = -1;
        int Number2BlockStart = -1;
        int Number2BlockEnd = -1;
        char Operator = '\0';

        //Not Done

        for (int i = 0; i < strlen(ValuedString); i++) {
            if (ValuedString[i] == '0' || ValuedString[i] == '1' || ValuedString[i] == '2' || ValuedString[i] == '3' || ValuedString[i] == '4' || ValuedString[i] == '5' || ValuedString[i] == '6' || ValuedString[i] == '7' || ValuedString[i] == '8' || ValuedString[i] == '9' || ValuedString[i] == '.') {
                if (Number1BlockStart == -1) {//This still needs to able to to follow the order of operations
                    Number1BlockStart = i;
                } else if (Number2BlockEnd == -1 && (ValuedString[i +1] == '^' || ValuedString[i+1] == '_' || ValuedString[i+1] == '*' || ValuedString[i+1] == '/' || ValuedString[i+1] == '-' || ValuedString[i+1] == '+')) {
                    Number2BlockEnd = i;
                    char Num1Str[64] = "";
                    for (int j = Number1BlockStart; j <= Number1BlockEnd; j++) {
                        // strcat(Num1Str, ValuedString[j]);
                    }
                    char Num2Str[64] = "";
                    for (int j = Number2BlockStart; j <= Number2BlockEnd; j++) {
                        // strcat(Num2Str, ValuedString[j]);
                    }
                    char StringRes[64] = "";
                    sprintf(StringRes, "%d", i);
                    // strcat(StringRes, Num1Str);
                    // strcat(StringRes, Num2Str);
                    return *StringRes;
                }
                if (Number2BlockStart == -1) {
                    Number2BlockStart = i;
                }
            } else if (ValuedString[i] == ValidOp1[0] || ValuedString[i] == ValidOp2[0] || ValuedString[i] == ValidOp3[0]) {//Parenthesis are handled in the DoMath function, so they are not needed here, Instead this loop is for the exponenets then Multiplication and division and so on
                Operator = ValuedString[i];//Assume Operator is valid, CalcExpression will check if it is valid or not
                Number1BlockEnd = ValuedString[i - 1];
            } else {//returns to null if the character is not a valid operator so that it doesn't select wrong chars, for ex "123*456" in "123+1*456" 
                Number1BlockStart = -1;
                Number1BlockEnd = -1;
            }
    }
    return 'N'; // NMIL representation placeholder
}




int CalcEquation(char ValuedString[]) {//Second layer of math funcs, it is for getting the values and operators from a string and then sending them to the CalcExpression function. Im sick of working on math so ima do smth else
    float NewVal;
    int Finished = 0;
    int Layer = 2;
    char Arr[64] = "";
    while (Finished == 0) {
        if (Finished == 1) {
            return NewVal;
        }
        
        if (Layer == 2) {
            char Block = GetBlock(ValuedString, "^", "_", " ");//Use space as a placeholder for the third operator, which is modulo, as it is not needed in this layer. Also Space was removed in the firstb layer of the math functions
            if (Block == 'N') {
                Layer = 1;
            } else {
                // int Start = StringRes[0];
                // StringRes[0] = '\t';
                // int NewNum = CalcExpression(&Block);
                // ftoa(NewNum, Arr, 12);
                // StringRes[Start] = Arr[0];
                // Arr[0] = '\0';
            }
        } else if (Layer == 1) {//Ima assume that doing "%" is fine for now because my internet is being weird and i cant look it up
            char Block = GetBlock(ValuedString, "*", "/", "%");//Use space as a placeholder for the third operator, which is modulo, as it is not needed in this layer. Also Space was removed in the firstb layer of the math functions
            if (Block == 'N') {
                Layer = 0;
            } else {
                // int Start = StringRes[0];
                // StringRes[0] = '\t';
                // int NewNum = CalcExpression(&Block);
                // ftoa(NewNum, Arr, 12);
                // StringRes[Start] = Arr[0];
                // Arr[0] = '\0';
            }
        } else if (Layer == 0) {//Ima assume that doing "%" is fine for now because my internet is being weird and i cant look it up
            char Block = GetBlock(ValuedString, "+", "-", " ");//Use space as a placeholder for the third operator, which is modulo, as it is not needed in this layer. Also Space was removed in the firstb layer of the math functions
            if (Block == 'N') {
                Finished = 1;
            } else {
                // int Start = StringRes[0];
                // StringRes[0] = '\t';
                // int NewNum = CalcExpression(&Block);
                // ftoa(NewNum, Arr, 12);
                // StringRes[Start] = Arr[0];
                // Arr[0] = '\0';
                // NewVal = atof(StringRes);
            }
        }
    }
    printf("Equation: %s\n", ValuedString);
    return 0;
}



//Storing for later as i need to redo the math system
/*                 int Num1 = atof(Num1Str);
                    int Num2 = atof(Num2Str);
                    int Result = CalcExpression(Num1, Num2, Operator);
                    for (int j = Number1BlockStart; j <= Number1BlockEnd; j++) {
                        ValuedString[j] = '\0';
                    }
                    for (int j = Number2BlockStart; j <= Number2BlockEnd; j++) {
                        ValuedString[j] = '\0';
                    }*/

typedef struct {
    void *Entries;
    int Count;
    int Capacity;
    size_t EntrySize;
    size_t KeySize;
    int (*Compare)(const void *, const void *);
} Map;

void InitMap(Map *map, size_t entrySize, size_t keySize, int (*compare)(const void *, const void *)) {
    free(map->Entries);
    map->Count = 0;
    map->Capacity = 0;
    map->EntrySize = 0;
    map->KeySize = keySize;
    map->Compare = compare;
}


static inline uint32_t HashFunction(void *key, size_t keySize) {
    uint8_t *bytes = (uint8_t *)key;
    uint32_t hash = 2166136261u;

    for (size_t i = 0; i < keySize; i++) {
        hash ^= bytes[i];
        hash *= 16777619;
    }
    return hash;
}


static inline int isNull(uint8_t *bytes, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (bytes[i] != 0) return 0;
  }
  return 1;
}

static uint8_t *linearProbing(Map *map, void *entries, void *key,
                              size_t entrySize, size_t capacity) {
  uint32_t hash = HashFunction(key, map->KeySize);
  uint32_t index = hash % capacity;
  uint8_t *tombstone = NULL;

  for (;;) {
    uint8_t *entry = (uint8_t *)entries + (index * entrySize);
    void *value = entry + map->KeySize;

    if (isNull(entry, map->KeySize)) {
      if (isNull((uint8_t *)value, map->EntrySize - map->KeySize)) {
        return tombstone != NULL ? tombstone : entry;
      } else {
        if (tombstone == NULL) {
          tombstone = entry;
        }
      }
    } else if (map->Compare(entry, key)) {
      return entry;
    }

    // Wraps back to 0 when end hit
    index = (index + 1) % map->Capacity;
  }
}

#define MAX_LOAD 0.75

static void adjustArray(Map *map) {
  int newCapacity = map->Capacity == 0 ? 8 : map->Capacity * 2;
  size_t entrySize = map->KeySize + (map->EntrySize - map->KeySize);

  uint8_t *oldEntries = (uint8_t *)map->Entries;
  uint8_t *newEntries = malloc(newCapacity * entrySize);

  if (newEntries == NULL) {
    ReturnError("Failed to allocate memory for map resizing");
  }

  // Zero out new buffer
  memset(newEntries, 0, newCapacity * entrySize);
  map->Count = 0;

  // Iterate over existing entries
  for (int i = 0; i < map->Capacity; i++) {
    uint8_t *oldEntry = oldEntries + (i * entrySize);
    void *key = oldEntry;
    void *value = oldEntry + map->KeySize;

    if (isNull((uint8_t *)key, map->KeySize)) continue;

    uint8_t *dest = linearProbing(map, newEntries, key, entrySize, newCapacity);

    memcpy(dest, key, map->KeySize);
    memcpy(dest + map->KeySize, value, map->EntrySize - map->KeySize);
    map->Count++;
  }

  free(map->Entries);
  map->Entries = newEntries;
  map->Capacity = newCapacity;
}


void mapRemove(Map *map, void *key) {
  if (map->Count == 0) return;

  // Get destination in new array
  uint8_t *entry = linearProbing(
      map, map->Entries, key, map->EntrySize, map->Capacity);

  memset(entry, 0, map->KeySize);

  // TOMBSTONE IS 1
  memset(entry + map->KeySize, 0, map->EntrySize - map->KeySize);
  memset(entry + map->EntrySize - 1, 1, 1);
}

int mapPut(Map *map, void *key, void *value) {
  if (map->Count >= map->Capacity * MAX_LOAD) {
    adjustArray(map);
  }

  uint8_t *entry = linearProbing(
      map, map->Entries, key, map->EntrySize, map->Capacity);

  int isNewKey = isNull(entry, map->KeySize);
  if (isNewKey && isNull(entry + map->KeySize, map->EntrySize - map->KeySize)) {
    map->Count++;
  }

  memcpy(entry, key, map->KeySize);
  memcpy(entry + map->KeySize, value, map->EntrySize - map->KeySize);

  return isNewKey;
}

void *mapGet(Map *map, void *key) {
  if (map->Capacity == 0) return NULL;

  // Get destination in new array
  uint8_t *entry = linearProbing(
      map, map->Entries, key, map->EntrySize, map->Capacity);
  if (isNull(entry, map->KeySize)) return NULL;
  return entry + map->KeySize;
}

void mapClear(Map *map) {
  size_t entrySize = map->EntrySize;
  memset(map->Entries, 0, map->Capacity * entrySize);
  map->Count = 0;
}
char InputText[] = "Temp";

int Ent(const void* a, const void* b) {
  const char* sa = (const char*)a;
  const char* sb = (const char*)b;
  return strcmp(sa, sb) == 0;
}


Map map;
// initMap(&map, 32, 32, Ent);



#define get_type(type) _Generic((type), \
    int: "int", \
    float: "float", \
    double: "double", \
    char *: "string", \
    default: "unknown")



int DoMath(int BaseVal, char ValuedString[]) {//First layer of math funcs, it is for checking if the parenthesis are correct and then sending the string of each alyer of parenthesis to the CalcEquation function. Also turns variables into raw numbers
    strlen(ValuedString);
    int LowestLayerParenthesis = 0;
    int OpenParenthesis = 0;
    int CloseParenthesis = 0;
    for (int i=0; i < strlen(ValuedString); i++) {//Remove spaces
        if (ValuedString[i] == ' ') {
            ValuedString[i] = '\0';//Remove spaces
        }
    }
    for (int i=0; i < strlen(ValuedString); i++) {//Check for parenthesis
        switch (ValuedString[i]) {
            case '(':
                OpenParenthesis++;
                break; 
            case ')':
                CloseParenthesis++;
            break;
        }
    }
    if (OpenParenthesis != CloseParenthesis) {
        ReturnError("Mismatched parenthesis in math expression");
    }
    //Extreme example so i can design the system around a case like this: Var = 2+(3*4(4(1+1)(9%2*4)))+(9+5)
    while (OpenParenthesis > 0) {

    }
    return BaseVal;
}




char *CreatedVars[0] = {};

int InputedTextLength = 4; // strlen(InputText)
char Setting[32] = "Cont";//Cont means Continue
char CurString[256] = "";



char CurrentlyCreatingVar[256] = "";

void CreateVar (char VarName[], Map *map, int Global, char CurString[], int CurStringNum) {
        if (strstr(VarName, "\"") != NULL) {
            int RemoveQuotes = 0;
            for (int j = 0; j < strlen(VarName); j++) {
                if (CurString[CurStringNum] == '\"') {
                    CurString[CurStringNum] = '\0';
                    RemoveQuotes++;
                }
            }
            if (RemoveQuotes != 2) {
                ReturnError("Invalid string value");
             }
             // mapPut(map, CurrentlyCreatingVar, VarName);
            } else {
            if (strstr(VarName, "true") != NULL) {
               // mapPut(map, CurrentlyCreatingVar, ...);
            } else if (strstr(VarName, "false") != NULL) {
               // mapPut(map, CurrentlyCreatingVar, ...);
            } else {
            char *endptr;
            strtol(VarName, &endptr, 10);
            if (errno != ERANGE) {
                  // mapPut(map, CurrentlyCreatingVar, ...);
            } else {
                  ReturnError("Invalid value for variable");
            }
        }
    }
}

int IsInFunction = 0;

int main() {
for (int i = 0; i <= InputedTextLength ; i++ ) {
    if (IsInFunction == 1) {
    } else {
        // CurString handling
        
    if (strcmp(Setting, "MultilineCommenting") == 0) {
         if (strstr(CurString,"~|") != NULL) {
            strcpy(Setting, "Cont");
            CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "Commenting") == 0) {
         if (strstr(CurString,"\t") != NULL) {
            strcpy(Setting, "Cont");
            CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "Log") == 0) {
         if (strstr(CurString,"(") != NULL) {
            strcpy(Setting, "StartLogging");
            CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "StartLogging") == 0) {
         if (strstr(CurString,")") != NULL && strstr(CurString,"\t") != NULL) {

            char MinSetting[16] = "nil";
            char ToPrint[256] = "";
            for (int j = 0; j < strlen(CurString); j++) {
                if (CurString[j] == '(' || CurString[j] == ')') {
                    CurString[j] = '\0';
                }
                if (strcmp(MinSetting, "nil") == 0) {
                    if (CurString[j] == '\"') {
                        MinSetting[0] = CurString[j];
                        MinSetting[1] = '\0';
                    }
                }
                if (MinSetting[0] == '\"') {
                    if (CurString[j] == '\"') {
                    strcpy(MinSetting, "nil");
                    // ToPrint concatenation
                }
              }
            }
            printf("%s", ToPrint);
            strcpy(Setting, "Cont");
            CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "SpaceVar") == 0) {
         if (strstr(CurString," ") != NULL) {
            strcpy(Setting, "NameVar");
            CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "NameVar") == 0) {
         if (strstr(CurString," ") != NULL) {
            strcpy(Setting, "TypeVarVal");
            strcpy(CurrentlyCreatingVar, CurString);
            CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "TypeVarVal") == 0) {
         if (strstr(CurString," ") != NULL) {
            strcpy(Setting, "ValVar");
            CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "ValVar") == 0) {
         if (strstr(CurString,"\t") != NULL) {
        CreateVar(CurString, &map, 0, CurString, i);
        strcpy(Setting, "Cont");
        CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "GSpaceVar") == 0) {
         if (strstr(CurString," ") != NULL) {
            strcpy(Setting, "GNameVar");
            CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "GNameVar") == 0) {
         if (strstr(CurString," ") != NULL) {
            strcpy(Setting, "GTypeVarVal");
            strcpy(CurrentlyCreatingVar, CurString);
            CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "GTypeVarVal") == 0) {
         if (strstr(CurString," ") != NULL) {
            strcpy(Setting, "GValVar");
            CurString[0] = '\0';
        }
    }
    else if (strcmp(Setting, "GValVar") == 0) {
         if (strstr(CurString,"\t") != NULL) {
        CreateVar(CurString, &map, 1, CurString, i);
        strcpy(Setting, "Cont");
        CurString[0] = '\0';
        }
    }

    if (strcmp(CurString, "/t") == 0) {
        CurLine++;
         if (strcmp(Setting, "Commenting") == 0) {
            strcpy(Setting, "Cont");
        }
    }
    else if (strcmp(CurString, "Func") == 0) {
        printf("Keyword: Func\n");
    }
    else if (strcmp(CurString, "Var") == 0) {
        printf("Keyword: Var\n");
        strcpy(Setting, "SpaceVar");
    }
    else if (strcmp(CurString, "GlobalVar") == 0) {
        printf("Keyword: GlobalVar\n");
        strcpy(Setting, "GSpaceVar");
    }
    else if (strcmp(CurString, "For") == 0) {
        printf("Keyword: For\n");
    }
    else if (strcmp(CurString, "While") == 0) {
        printf("Keyword: While\n");
    }
    else if (strcmp(CurString, "If") == 0) {
        printf("Keyword: If\n");
    }
    else if (strcmp(CurString, "Do") == 0) {
        printf("Keyword: Do\n");
    }
    else if (strcmp(CurString, "Then") == 0) {
        printf("Keyword: Then\n");
    }
    else if (strcmp(CurString, "End") == 0) {
        printf("Keyword: End\n");
    }
    else if (strcmp(CurString, "~") == 0) {
        printf("Keyword: ~\n");
        strcpy(Setting, "Commenting");
    }
    else if (strcmp(CurString, "|~") == 0) {
        printf("Keyword: |~\n");
        strcpy(Setting, "MultilineCommenting");
    }
    else if (strcmp(CurString, "Elseif") == 0) {
        printf("Keyword: Elseif\n");
    }
    else if (strcmp(CurString, "Log") == 0) {
        printf("Keyword: Log(\n");
        strcpy(Setting, "Log");
        CurString[0] = '\0';
    }
}}
return 0;
}
