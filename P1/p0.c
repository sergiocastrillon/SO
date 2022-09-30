// Sergio Castrillón | s.castrillon
// Mario Freire | mario.freire

#include "funciones.h"


int main() {
    bool continuar = true;
    tList lista;
    CreateList(&lista);
    while (continuar){
        imprimirPrompt();
        char entrada[MAX];
        leerEntrada(entrada,lista);
        //printf("%s",linea); // Comprobar funcionamiento de leerEntrada
        continuar = procesarEntrada(entrada,lista);
    }
    tPosL i = lista; // Eliminar fugas de memoria valgrind??
    while(i!=NULL){
        tPosL x = next(i,lista);
        free(i);
        i = x;
    }
    return 0;
}






