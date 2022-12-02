#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

#ifndef LISTAP_H
#define LISTAP_H

typedef struct{
    pid_t pid;			//tamaño reservado en memoria
    time_t time;
    int status; // 0 para terminado, 1 señalado, 2 parado, 3 activo
    int signal;
    char command[100];
    int priority;
}tItemP;


typedef struct tNodeP *tPosP;

struct tNodeP{
    tItemP data;
    tPosP sig;
};

typedef tPosP tListP;


bool isEmptyListP(tListP list); 

void createListP(tListP* list); 
bool insertItemP(tItemP item, tListP list);
void removeItemP(tPosP pos, tListP list);
tPosP previousP(tPosP pos, tListP list); 
tPosP nextP(tPosP pos, tListP L); 
tPosP firstP(tListP list);
tPosP lastP(tListP list); 
tItemP getItemP(tPosP pos,tListP list); 
void deleteListP(tListP *list);




#endif