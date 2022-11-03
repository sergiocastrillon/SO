#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef LISTAM_H
#define LISTAM_H

typedef struct{
    char type[10];	// Se podria hacer con un enumeraod	
    int tam;			//tamaño reservado en memoria
    time_t time;
    void * direction;		//dirección de memoria
    char filename[64];		//mmap
    int key;			//clave para shared
}tItemM;


typedef struct tNodeM *tPosM;

struct tNodeM{
    tItemM data;
    tPosM sig;
};

typedef tPosM tListM;


bool isEmptyListM(tListM list); 

void createListM(tListM* list); 
bool insertItem(tItemM item, tListM list);
void removeItem(tPosM pos, tListM list);
tPosM previousM(tPosM pos, tListM list); 
tPosM nextM(tPosM pos, tListM L); 
tPosM firstM(tListM list);
tPosM lastM(tListM list); 
tItemM getItem(tPosM pos,tListM list); 
void deleteListM(tListM *list);

#endif