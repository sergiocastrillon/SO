// Sergio Castrillón | s.castrillon
// Mario Freire | mario.freire

#include "funciones.h"



int main() {
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
        continuar = procesarEntrada(entrada,lista,listaMem);
    }
    deleteList(&lista);
    deleteListM(&listaMem);
    return 0;
}






