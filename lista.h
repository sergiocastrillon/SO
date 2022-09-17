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

bool isEmptyList(tList list); // Comprobado

void CreateList(tList* list); // Comprobado
bool InsertElement (char item[], tList list); // Comprobado
void RemoveElement(tPosL pos, tList list);
tPosL previous(tPosL pos, tList list); // Comprobado
tPosL next(tPosL pos, tList L); // Comprobado
tPosL first(tList list);
tPosL last(tList list); // Comprobado
char* getItem (tPosL pos, tList L); // Comprobado