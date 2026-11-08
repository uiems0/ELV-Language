//I may need to remake how this language works as im not sure on how ill make it run functions and local + global vars. Will most likely make it read the code first, and then retroactivally run activate things, so that global vars can exist

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int CurLine=0; // Keeps track of the line, 

void ReturnError(char *ErrorMessage) {
    printf("Error on line %d: %s\n", CurLine, ErrorMessage);
    return;
}

inline float CalcExpression(float Num1,float Num2, char Operator) {//Last layer of math funcs, it is for solving small expressions like 2+2 or 3*4, etc
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
        return Num1 % Num2;
        break;
        //Square roots arent nessecary as they can be done with the ^ operator
        default:
            ReturnError("Invalid operator");
    }
}

static inline int GetBlock(char ValuedString[], int StartIndex, int EndIndex, char ValidOp1[], char ValidOp2[]) {//Gets a block of a string from the start index to the end index
        int Number1BlockStart = -1;
        int Number1BlockEnd = -1;
        int Number2BlockStart = -1;
        int Number2BlockEnd = -1;
        char Operator = '\0';

        //Not Done, I plan to add a way to have an array like ReturnedValues[] = {"Val1StringBlock"=Val1} to save on performance, but for now this will do

        for (int i = 0; i < strlen(ValuedString); i++) {
            if (ValuedString[i] == '0' || ValuedString[i] == '1' || ValuedString[i] == '2' || ValuedString[i] == '3' || ValuedString[i] == '4' || ValuedString[i] == '5' || ValuedString[i] == '6' || ValuedString[i] == '7' || ValuedString[i] == '8' || ValuedString[i] == '9' || ValuedString[i] == '.') {
                if (Number1BlockStart == -1) {//This still needs to able to to follow the order of operations
                    Number1BlockStart = i;
                } else if (Number2BlockEnd == -1 && (ValuedString[i +1] == '^' || ValuedString[i+1] == '_' || ValuedString[i+1] == '*' || ValuedString[i+1] == '/' || ValuedString[i+1] == '-' || ValuedString[i+1] == '+')) {
                    Number2BlockEnd = i;
                    char Num1Str[] = "";
                    for (int j = Number1BlockStart; j <= Number1BlockEnd; j++) {
                        strcat(Num1Str, ValuedString[j]);
                    }
                    char Num2Str[] = "";
                    for (int j = Number2BlockStart; j <= Number2BlockEnd; j++) {
                        strcat(Num2Str, ValuedString[j]);
                    }
                    int AcqVal = CalcExpression(atof(Num1Str), atof(Num2Str), Operator);
                    return AcqVal;
                }
                if (Number2BlockStart == -1) {
                    Number2BlockStart = i;
                }
            } else if (ValuedString[i] == ValidOp1 || ValuedString[i] == ValidOp2) {//Parenthesis are handled in the DoMath function, so they are not needed here, Instead this loop is for the exponents in PEMDAS
                Operator = ValuedString[i];//Assume Operator is valid, CalcExpression will check if it is valid or not
                Number1BlockEnd = ValuedString[i - 1];
            }
    }
    ReturnError("Invalid expression");
}

int CalcEquation(char ValuedString[]) {//Second layer of math funcs, it is for getting the values and operators from a string and then sending them to the CalcExpression function. Im sick of working on math so ima do smth else
    float NewVal;
    bool Finished = false;

    while (Finished == false) {
        if (Finished == true) {
            return NewVal;
        }
        


    }
    printf("Equation: %s\n", ValuedString);
}



//Storing for later as i need to redo the math system
/*                    int Num1 = atof(Num1Str);
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
    bool (*Compare)(const void *, const void *);
} Map;

void InitMap(Map *map, size_t entrySize, size_t keySize, bool (*compare)(const void *, const void *)) {
    free(map->Entries);
    map->Count = 0;
    map->Capacity = 0;
    map->EntrySize = NULL;
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


static inline bool isNull(uint8_t *bytes, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (bytes[i] != 0) return false;
  }
  return true;
}

static uint8_t *linearProbing(Map *map, void *entries, void *key,
                              size_t entrySize, size_t capacity) {
  uint32_t hash = HashFunction(key, map->KeySize);
  uint32_t index = hash % capacity;
  uint8_t *tombstone = NULL;

  for (;;) {
    uint8_t *entry = entries + (index * entrySize);
    void *value = entry + map->KeySize;

    if (isNull(entry, map->KeySize)) {
      if (isNull(value, map->ValueSize)) {
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
  int newCapacity = grow_capacity(map->Capacity);
  size_t entrySize = map->KeySize + map->ValueSize;

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

    if (isNull(key, map->KeySize)) continue;

    uint8_t *dest = linearProbing(map, newEntries, key, entrySize, newCapacity);

    memcpy(dest, key, map->KeySize);
    memcpy(dest + map->KeySize, value, map->ValueSize);
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
      map, map->Entries, key, (map->KeySize + map->ValueSize), map->Capacity);

  memset(entry, 0, map->KeySize);

  // TOMBSTONE IS 1
  memset(entry + map->KeySize, 0, map->ValueSize);
  memset(entry + map->KeySize + map->ValueSize - 1, 1, 1);
}

bool mapPut(Map *map, void *key, void *value) {
  if (map->Count >= map->Capacity * MAX_LOAD) {
    adjustArray(map);
  }

  uint8_t *entry = linearProbing(
      map, map->Entries, key, (map->KeySize + map->ValueSize), map->Capacity);

  bool isNewKey = isNull(entry, map->KeySize);
  if (isNewKey && isNull(entry + map->KeySize, map->ValueSize)) {
    map->Count++;
  }

  memcpy(entry, key, map->KeySize);
  memcpy(entry + map->KeySize, value, map->ValueSize);

  return isNewKey;
}

void *mapGet(Map *map, void *key) {
  if (map->Capacity == 0) return NULL;

  // Get destination in new array
  uint8_t *entry = linearProbing(
      map, map->Entries, key, (map->KeySize + map->ValueSize), map->Capacity);
  if (isNull(entry, map->KeySize)) return NULL;
  return entry + map->KeySize;
}

void mapClear(Map *map) {
  size_t entrySize = map->KeySize + map->ValueSize;
  memset(map->Entries, 0, map->Capacity * entrySize);
  map->Count = 0;
}
char InputText[] = "Temp";

bool Ent(const void* a, const void* b) {
  const char* sa = (const char*)a;
  const char* sb = (const char*)b;
  return strcmp(sa, sb) == 0;
}


Map &map;
initMap(&map, 32, 32, Ent);



#define get_type(type) _Generic((type), \
    int: "int", \
    float: "float", \
    double: "double", \
    char *: "string", \
    default: "unknown")
\


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

int InputedTextLength = strlen(InputText);
char Setting[] = "Cont";//Cont means Continue
char CurString[] = {};



char CurrentlyCreatingVar[] = "";

void CreateVar (char VarName[], map *map, bool Global, char CurString[], int CurStringNum) {
        if (strstr(VarName, "\"") != NULL) {
            int RemoveQuotes = 0;
            for (int j = 0; j < strlen(VarName); j++) {
                if (CurString[CurStringNum] == '\"') {
                    CurString[CurStringNum] = '\0';
                    RemoveQuotes++;
                }
            }
            if (RemoveQuotes != 2) {
                returnError("Invalid string value");
             }
             mapPut(&map, CurrentlyCreatingVar, VarName);
            } else {
            if (strstr(VarName, "true") != NULL) {
               mapPut(&map, CurrentlyCreatingVar, true);
            } else if (strstr(VarName, "false") != NULL) {
             mapPut(&map, CurrentlyCreatingVar, false);
            } else {
            char *endptr;
            strtol(VarName, &endptr, 10);
            if (errno != ERANGE) {
                  mapPut(&map, CurrentlyCreatingVar, strtol(VarName, &endptr, 10));
            } else {
                 returnError("Invalid value for variable");
            }
        }
    }
}

bool IsInFunction = false;

char NilUnasignedVars[][] = {};
char NilUnasignedGlobalVars[][] = {};



for (int i = 0; i <= InputedTextLength ; i++ ) 
    if (IsInFunction == true) {
    } else {
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
            if (strstr(CurString,"(") != NULL) {
            Setting = "StartLogging";
            CurString = "";
        }
        break;
        case "StartLogging":
            if (strstr(CurString,")") != NULL && strstr(CurString,"\t") != NULL) {

                char MinSetting[] = "nil";
                char ToPrint[] = "";
                for (int j = 0; j < strlen(CurString); j++) {
                    if (CurString[j] == '(' || CurString[j] == ')') {
                        CurString[j] = '\0';
                    }
                    if (MinSetting == "nil") {
                        if (CurString[j] == '\"') {
                            MinSetting = CurString[j];
                        }
                    }
                    if (MinSetting == '\"') {
                        if (CurString[j] == '\"') {
                        MinSetting = "nil";
                        ToPrint = strcat(ToPrint,CurString[j]);//Unfinished, needs to add variable reading and a way to differentiate between values like log("J",Var)
                    }
                }
                printf("%s", ToPrint);
            Setting = "Cont";
            CurString = "";
        }
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
        break;
        case "ValVar":
            if (strstr(CurString,"\t") != NULL) {
            
            char CheckingString[] = InputText[i] - strlen(CurString);

            CreateVar(CurString, &map, false, CurString, i);
            
            Setting = "Cont";
            CurString = "";
        }
        break;
        case "GSpaceVar":
            if (strstr(CurString," ") != NULL) {
            Setting = "GNameVar";
            CurString = "";
        }
        break;
        case "GNameVar":
            if (strstr(CurString," ") != NULL) {
            Setting = "GTypeVarVal";
            CurrentlyCreatingVar = CurString;
            CurString = "";
        }
        break;
        case "GTypeVarVal":
            if (strstr(CurString," ") != NULL) {
            Setting = "GValVar";
            CurString = "";
        }
        break;
        case "GValVar":
            if (strstr(CurString,"\t") != NULL) {
            
            char CheckingString[] = InputText[i] - strlen(CurString);

            CreateVar(CurString, &map, true, CurString, i);
            
            Setting = "Cont";
            CurString = "";
        }
        break;

    }

    switch (CurString) {

        case "/t":
        CurLine++;
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
        case "GlobalVar":
            printf("Keyword: GlobalVar\n");
            Setting = "GSpaceVar";
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
    }
}};
