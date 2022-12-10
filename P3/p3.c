// Sergio Castrillón | s.castrillon
// Mario Freire | mario.freire

#include "funciones.h"
#include "funciones2.h"



int main(int argc, char *argv[], char *arg3[]){
    bool continuar = true;
    tList lista;
    tListM listaMem;
    tListP listp;
    CreateList(&lista);
    createListM(&listaMem);
    createListP(&listp);
    while (continuar){
        imprimirPrompt();
        char entrada[MAX];
        leerEntrada(entrada,lista);
        //printf("%s",linea); // Comprobar funcionamiento de leerEntrada
        continuar = procesarEntrada(entrada,lista,listaMem,arg3,listp);
    }
    deleteList(&lista);
    deallocate_all(listaMem);
    if(!isEmptyListM(listaMem)) printf("No se ha podido liberar toda la memoria\n");
    deleteListM(&listaMem);
    deleteListP(&listp);
    return 0;
}






