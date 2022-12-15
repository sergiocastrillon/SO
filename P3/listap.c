#include "listap.h"




char * statusToString(int n){
    switch(n){
        case 0: 
            return "TERMINADO";
        case 1: 
            return "SENALADO";
        case 2: 
            return "PARADO";
        case 3: 
            return "ACTIVO";
        break;
        default:
            return "UNKNOWN";
    }
}



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
    while(temp -> sig != pos) temp = temp -> sig;
    temp -> sig = pos->sig;
    free(pos);
    pos = NULL;
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
    item.status = ACTIVO;
    item.signal = 0;

    insertItemP(item,list);
}

void cleanListP(tListP list){
    tPosP i = firstP(list);
    tPosP x;
    while(i!=NULL){
        x = i->sig;
        free(i);
        i = x;
    }
    list->sig = NULL;
}

void updateItem(tItemP item,tPosP pos, tListP list){
    pos->data = item;
}

tPosP findPid(int pid, tListP list){
    tPosP pos = firstP(list);
    while(pos != NULL){
        if(pid == pos->data.pid) break;
        pos = nextP(pos,list);
    }
    return pos;
}