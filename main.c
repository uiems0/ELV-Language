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
  uint32_t hash = hashFunction(key, map->key_size);
  uint32_t index = hash % capacity;
  uint8_t *tombstone = NULL;

  for (;;) {
    uint8_t *entry = entries + (index * entrySize);
    void *value = entry + map->key_size;

    if (isNull(entry, map->key_size)) {
      if (isNull(value, map->value_size)) {
        return tombstone != NULL ? tombstone : entry;
      } else {
        if (tombstone == NULL) {
          tombstone = entry;
        }
      }
    } else if (map->cmp(entry, key)) {
      return entry;
    }

    // Wraps back to 0 when end hit
    index = (index + 1) % map->capacity;
  }
}

#define MAX_LOAD 0.75

static void adjustArray(Map *map) {
  int newCapacity = grow_capacity(map->capacity);
  size_t entrySize = map->key_size + map->value_size;

  uint8_t *oldEntries = (uint8_t *)map->entries;
  uint8_t *newEntries = malloc(newCapacity * entrySize);

  // Zero out new buffer
  memset(newEntries, 0, newCapacity * entrySize);
  map->count = 0;

  // Iterate over existing entries
  for (int i = 0; i < map->capacity; i++) {
    uint8_t *oldEntry = oldEntries + (i * entrySize);
    void *key = oldEntry;
    void *value = oldEntry + map->key_size;

    if (isNull(key, map->key_size)) continue;

    uint8_t *dest = linearProbing(map, newEntries, key, entrySize, newCapacity);

    memcpy(dest, key, map->key_size);
    memcpy(dest + map->key_size, value, map->value_size);
    map->count++;
  }

  free(map->entries);
  map->entries = newEntries;
  map->capacity = newCapacity;
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
