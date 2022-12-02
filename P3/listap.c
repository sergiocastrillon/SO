#include "listap.h"

bool isEmptyListP(tListP list){
    return list->sig == NULL;
}


void createListP(tListP* list){
    // calloc reserva memoria y inicializa a 0 pero mejor usar malloc
    //*list = calloc(1,sizeof(struct tNode));
    *list = malloc(sizeof(struct tNodeP));
    (*list) -> data.pid = -1;
    (*list) -> sig = NULL;
}

bool insertItemP(tItemP item, tListP list){
    tPosP lastPos = list;

    while(lastPos -> sig != NULL) lastPos = lastPos -> sig;
    if((lastPos -> sig = malloc(sizeof(struct tNodeP))) == NULL) return false;

    lastPos = lastPos->sig;
    lastPos -> data = item;
    
    lastPos->sig = NULL;
    return true;
}







void removeItemP(tPosP pos, tListP list){ // Recordar que las posiciones pueden variar al borrar
    tPosP temp = list;
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
tPosP previousP(tPosP pos, tListP list){
    tPosP temp1,temp2;
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
tPosP nextP(tPosP pos, tListP list){
    if(pos == NULL) return NULL;
    return pos->sig;
}
// next


// first
tPosP firstP(tListP list){
    return list->sig;
}
// first


// last
tPosP lastP(tListP list){
    tPosP temp = list;
    if(list->sig == NULL) return NULL;
    while(temp->sig != NULL) temp = temp->sig; // Recorremos toda la lista hasta llegar al último nodo
    return temp;
}
// last

// getItem
tItemP getItemP(tPosP pos,tListP list){
    return pos->data;
}
// getItem

// deleteList
void deleteListP(tListP *list){
    tPosP i = *list;
    while(i!=NULL){
        tPosP x = i->sig;
        // No volver a usar una lista borrada así
        // el insertElement de la lista no detecta lista a NULL, el primer elemento
        // de la lista debe estar reservado (usar CreateList otra vez)
        free(i);
        i = x;
    }
    *list = NULL;
}



void insertProcess(pid_t pid, char* command,tListP list){
    tItemP item;
    item.priority = getpriority(PRIO_PROCESS,pid);
    item.pid = pid;
    item.time = time(NULL);
    strncpy(item.command,command,100);

    insertItemP(item,list);
    

}