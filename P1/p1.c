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
    deleteList(&lista);
    return 0;
}






