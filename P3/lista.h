#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef LISTA_H
#define LISTA_H
#define MAX 300

typedef struct tNode* tPosL;
struct tNode{
    char data[MAX + 1]; // Evitar segmentation faults por muchos carácteres
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
void getElement (tPosL pos, char* item, tList L); // Comprobado
void deleteList(tList *list);

#endif
