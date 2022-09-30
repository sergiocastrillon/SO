#include "funciones1.h"
#include "funciones.h" // ???

void create(char * trozos[], int ntrozos){
    if(ntrozos == 1) printf("Imprimir directorio\n");
    // Llamada a carpeta ??
    else{
        if(strcmp(trozos[1],"-f")==0){ // Crear fichero
        mode_t permisos; // Modificar permisos
        // No detecta si el fichero ya está ??
        open(trozos[2], O_CREAT|O_WRONLY|O_TRUNC , permisos);
        }else{ // Crear directorio con lo que está en trozos[1]
        // mkdir
        printf("Hola");

        }
    }
}