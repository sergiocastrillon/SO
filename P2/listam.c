#include "listam.h"

bool isEmptyListM(tListM list){
    return list->sig == NULL;
}


void createListM(tListM* list){
    // calloc reserva memoria y inicializa a 0 pero mejor usar malloc
    //*list = calloc(1,sizeof(struct tNode));
    *list = malloc(sizeof(struct tNodeM));
    strcpy((*list) -> data.type,"\0");
    (*list) -> sig = NULL;
}

bool insertItem(tItemM item, tListM list){
    tPosM lastPos = list;
    while(lastPos -> sig != NULL) lastPos = lastPos -> sig;
    if((lastPos -> sig = malloc(sizeof(struct tNodeM))) == NULL) return false;
    lastPos = lastPos->sig;
    // Asignaciones
    lastPos -> data = item;
    /* strncpy(lastPos->data.type,item.type,10);
    strncpy(lastPos->data.filename,item.filename,64);
    lastPos->data.tam = item.tam;
    lastPos->data.time = item.time; */
    lastPos->sig = NULL;
    return true;
}



void removeItem(tPosM pos, tListM list){
    tPosM temp = list;
    // Si el elemento a borrar no es el último de la lista
    if(pos->sig == NULL){
        while(temp -> sig != pos) temp = temp -> sig;
        free(pos);
        pos = NULL;
        temp -> sig = NULL;
    }else{
        temp = pos->sig;
        pos->data = temp->data; // Sustituimos la información en el nodo que queremos borrar por la del nodo siguiente
        pos->sig = temp->sig;
        free(temp); // Borramos el nodo siguiente que contiene la misma información que pos
        temp = NULL; // El nodo no apunta a nada ahora
    }
    
}


// previous
tPosM previousM(tPosM pos, tListM list){
    tPosM temp1,temp2;
    if(list -> sig == pos) return NULL;
    temp1 = list;
    temp2 = NULL;
    while(temp1 != pos){
        temp2=temp1;
        temp1 = temp1->sig;
    }
    return temp2;
}
// previous


// next
tPosM nextM(tPosM pos, tListM list){
    return pos->sig;
}
// next


// first
tPosM firstM(tListM list){
    return list->sig;
}
// first


// last
tPosM lastM(tListM list){
    tPosM temp = list;
    if(list->sig == NULL) return NULL;
    while(temp->sig != NULL) temp = temp->sig; // Recorremos toda la lista hasta llegar al último nodo
    return temp;
}
// last

// getItem
tItemM getItem(tPosM pos,tListM list){
    return pos->data; // Devolvemos una copia para evitar modificaciones accidentales
}
// getItem

// deleteList
void deleteListM(tListM *list){
    tPosM i = *list;
    while(i!=NULL){
        tPosM x = i->sig;
        // No volver a usar una lista borrada así
        // el insertElement de la lista no detecta lista a NULL, el primer elemento
        // de la lista debe estar reservado (usar CreateList otra vez)
        free(i);
        i = x;
    }
    *list = NULL;
}
