// Sergio Castrillón | s.castrillon
// Mario Freire | mario.freire

#include "funciones.h"
#include "funciones2.h"



int main(int argc, char *argv[], char *arg3[]){
    bool continuar = true;
    tList lista;
    tListM listaMem;
    CreateList(&lista);
    createListM(&listaMem);
    while (continuar){
        imprimirPrompt();
        char entrada[MAX];
        leerEntrada(entrada,lista);
        //printf("%s",linea); // Comprobar funcionamiento de leerEntrada
        continuar = procesarEntrada(entrada,lista,listaMem,arg3);
    }
    deleteList(&lista);
    deallocate_all(listaMem);
    if(!isEmptyListM(listaMem)) printf("No se ha podido liberar toda la memoria\n");
    deleteListM(&listaMem);
    return 0;
}






