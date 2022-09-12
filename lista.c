#include <stdio.h>

typedef struct tNode* tPosL;
struct tNode{
    char data[30];
    tPosL sig;
};
typedef tPosL tList;



void CreateList(tList *L);





bool InsertElement (tItemL item, tList L){
    tPosL temp;
    temp = malloc(sizeof(struct tNode));
    if(temp==LNULL) return false; // Si no se ha podido reservar memoria devuelve false
    else{
        if(isEmptyList(L)==true) { // Caso 1: El elemento es el primero de la lista
        // Modificar isEmptyList por la condición que me permita saber si la lista está vacía
            temp->data = item;
            temp->sig = LNULL;
            //*L = temp;
        }else{
            // Caso 2: El elemento no es el primero
            tPosL lastItem;
            lastItem = last(*L);
            lastItem->sig = temp;
            temp = lastItem->sig;
            temp->data=item;
            temp->sig = LNULL;
            
            }
        }
        return true; // Devuelve true si la ejecución se pudo realizar
    }
}




void RemoveElement (tPosL pos, tList L){
    tPosL temp;
    if(pos->sig==LNULL){ // El elemento a borrar es el último de la lista
        if(*L==pos){ // El elemento a borrar es el único elemento de la lista
            *L=LNULL;
        }else{ // Si no lo es tenemos que buscar el penúltimo elemento y hacer que el siguiente sea LNULL
            temp = previous(pos,*L);
            temp->sig = LNULL;
        }
        free(pos); // Borramos la memoria reservada para la posición
        pos = LNULL; // pos ahora apunta a LNULL
    }else{ // Si el elemento a borrar no es el último de la lista
        temp = pos->sig;
        pos->data = temp->data; // Sustituimos la información en el nodo que queremos borrar por la del nodo siguiente
        pos->sig = temp->sig;
        free(temp); // Borramos el nodo siguiente que contiene la misma información que pos
        temp = LNULL; // El nodo no apunta a nada ahora
}

