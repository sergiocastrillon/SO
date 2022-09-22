#include "lista.h"

bool isEmptyList(tList list){
    return strcmp(list->data,"\0") == 0;
}

void CreateList(tList* list){
    // calloc reserva memoria y inicializa a 0 pero mejor usar malloc
    //*list = calloc(1,sizeof(struct tNode));
    *list = malloc(sizeof(struct tNode));
    strcpy((*list) -> data,"\0");
    (*list) -> sig = NULL;
}

bool InsertElement (char item[], tList list){
    //tPosL temp;
    //temp = malloc(sizeof(struct tNode));
    if(isEmptyList(list)==true) { // Caso 1: El elemento es el primero de la lista
        // Modificar isEmptyList por la condición que me permita saber si la lista está vacía
        strcpy(list->data,item);
        //*L = temp;
    }else{
        // Caso 2: El elemento no es el primero
        tPosL lastPos;
        lastPos = last(list);
        lastPos -> sig = calloc(1,sizeof(struct tNode));
        lastPos = lastPos->sig;
        strcpy(lastPos->data,item);
        lastPos->sig = NULL;

    }
    return true; // Devuelve true si la ejecución se pudo realizar
}



// Adaptar a la nueva implementación
void RemoveElement(tPosL pos, tList list){
    tPosL temp;
    if(pos->sig==NULL){ // El elemento a borrar es el último de la lista
        if(list==pos){ // El elemento a borrar es el único elemento de la lista
            strcpy(list->data,"\0");
        }else{ // Si no lo es tenemos que buscar el penúltimo elemento y declarar sig a NULL
            temp = previous(pos,list);
            temp->sig = NULL;
            free(pos); // Borramos la memoria reservada para la posición
            pos = NULL; // pos ahora apunta a NULL
        }

    }else{ // Si el elemento a borrar no es el último de la lista
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
    temp1 = list;
    temp2=NULL;
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
    return list;
}
// first

// last
tPosL last(tList list){
    tPosL temp = list;
    while(temp->sig != NULL) temp = temp->sig; // Recorremos toda la lista hasta llegar al último nodo
    return temp;
}
// last




// getItem
void getElement(tPosL pos, char* element, tList list){
    strcpy(element,pos->data); // Devolvemos una copia para evitar modificaciones accidentales
}
// getItem