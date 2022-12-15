// Sergio Castrillón | s.castrillon
// Mario Freire | mario.freire

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
    lastPos -> data = item;
    
    lastPos->sig = NULL;
    return true;
}

bool insertMalloc(void * direction, size_t tam,tListM list){
    tItemM item;
    item.time = time(NULL);
    item.tam = tam;
    strcpy(item.type,"malloc");
    item.direction = direction;

    strcpy(item.filename,"\0");
    item.descriptor = -1;
    item.key = -1;

    return insertItem(item,list);
}

bool insertShared(void * direction, size_t tam,int key,tListM list){
    tItemM item;
    item.time = time(NULL);
    item.tam = tam;
    strcpy(item.type,"shared");
    item.direction = direction;
    item.key = key;

    strcpy(item.filename,"\0");
    item.descriptor = -1;
    
    return insertItem(item,list);
}

bool insertMap(void * direction, size_t tam,char file[],int descriptor,tListM list){
    tItemM item;

    item.time = time(NULL);
    item.tam = tam;
    strcpy(item.type,"mmap");
    item.direction = direction;
    item.descriptor = descriptor;
    strcpy(item.filename,file);

    item.key = -1;
    
    return insertItem(item,list);
}




void removeItem(tPosM pos, tListM list){ // Recordar que las posiciones pueden variar al borrar
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
    if(pos == NULL) return NULL;
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
    return pos->data;
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

//findDirection
tPosM findDirection(void * p,tListM list){
    tPosM i = firstM(list);
    while(i!=NULL){
        if(i->data.direction == p) break;
        i = nextM(i,list);
    }
    return i;
}

tPosM findMallocTam(size_t tam,tListM list){
    tPosM i = firstM(list);
    while(i != NULL){
        if(strcmp(i->data.type,"malloc") == 0 && i->data.tam == tam) break;
        i = nextM(i,list);
    }
    return i;
}

tPosM findSharedKey(key_t key,tListM list){
    tPosM i = firstM(list);
    while(i != NULL){
        if(i->data.key == key) break; // como en el resto de allocates se pone la key a -1
        // no hace falta comprobar que sea de tipo shared ya que es asegurado si es mayor que 0
        i = nextM(i,list);
    }
    return i;
}

tPosM findMappedFile(char* file,tListM list){
    tPosM i = firstM(list);
    while(i != NULL){
        if(strcmp(i->data.filename,file)==0) break;
        i = nextM(i,list);
    }
    return i;
}
