#include "lista.h"

bool isEmptyList(tList list){
    return list->sig == NULL;
}

void CreateList(tList* list){
    // calloc reserva memoria y inicializa a 0 pero mejor usar malloc
    //*list = calloc(1,sizeof(struct tNode));
    *list = malloc(sizeof(struct tNode));
    strcpy((*list) -> data,"\0");
    (*list) -> sig = NULL;
}

bool InsertElement (char item[], tList list){
    tPosL lastPos = list;
    while(lastPos -> sig != NULL) lastPos = lastPos -> sig;
    lastPos -> sig = malloc(sizeof(struct tNode));
    lastPos = lastPos->sig;
    strncpy(lastPos->data,item,MAX-1);
    lastPos->sig = NULL;
    return true;
    }
     // Devuelve true si la ejecución se pudo realizar



// Adaptar a la nueva implementación
void RemoveElement(tPosL pos, tList list){
    tPosL temp = list;
    // Si el elemento a borrar no es el último de la lista
    if(pos->sig == NULL){
        while(temp -> sig != pos) temp = temp -> sig;
        free(pos);
        pos = NULL;
        temp -> sig = NULL;
    }else{
        temp = pos->sig;
        strcpy(pos->data,temp->data); // Sustituimos la información en el nodo que queremos borrar por la del nodo siguiente
        pos->sig = temp->sig;
        free(temp); // Borramos el nodo siguiente que contiene la misma información que pos
        temp = NULL; // El nodo no apunta a nada ahora
    }
    
}


// previous
tPosL previous(tPosL pos, tList list){
    tPosL temp1,temp2;
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
tPosL next(tPosL pos, tList list){
    return pos->sig;
}
// next



// first
tPosL first(tList list){
    return list->sig;
}
// first

// last
tPosL last(tList list){
    tPosL temp = list;
    if(list->sig == NULL) return NULL;
    while(temp->sig != NULL) temp = temp->sig; // Recorremos toda la lista hasta llegar al último nodo
    return temp;
}
// last




// getItem
void getElement(tPosL pos, char* element, tList list){
    strcpy(element,pos->data); // Devolvemos una copia para evitar modificaciones accidentales
}
// getItem

// deleteList
void deleteList(tList *list){
    tPosL i = *list;
    while(i!=NULL){
        tPosL x = i->sig;
        // No volver a usar una lista borrada así
        // el insertElement de la lista no detecta lista a NULL, el primer elemento
        // de la lista debe estar reservado (usar CreateList otra vez)
        free(i);
        i = x;
    }
    *list = NULL;
}