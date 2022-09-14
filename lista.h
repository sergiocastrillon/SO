


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct tNode* tPosL;
struct tNode{
    char data[20];
    tPosL sig;
};
typedef tPosL tList;

bool isEmptyList(tList list);
tPosL previous(tPosL pos, tList list);
tPosL next(tPosL pos, tList L);
void CreateList(tList* list);
tPosL last(tList list);
bool InsertElement (char item[], tList list);
char* getItem (tPosL pos, tList L);