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

char *CreatedVars[0] = {};

int InputedTextLength = sizeof(InputText) - 2;
char Setting[] = "Cont";//Cont means Continue
char CurString[] = "";


char GlobalVarsInLimbo[][] = {};#For global vars, as they are created before the function is called, so they need to be stored somewhere until the function is called


for (int i = 0; i <= InputedTextLength ; i++ ) 
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

bool IsInFuncion = false;

for (int i = 0; i <= InputedTextLength ; i++ ) {
    if (IsInFuncion == true) {
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
