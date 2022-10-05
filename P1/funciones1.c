#include "funciones1.h"

void create(char * trozos[], int ntrozos){
    if(ntrozos == 1) {
        char direccion[100];
        char *error = getcwd(direccion,100);
        if (error != NULL) printf("%s\n",direccion);
        else perror("No fue posible obtener la ruta del directorio");
    }
    // Llamada a carpeta ??
    else{
        mode_t permisos = S_IRWXU | S_IROTH | S_IRUSR | S_IXGRP | S_IXOTH | S_IRGRP;
        if(strcmp(trozos[1],"-f")==0){ // Crear fichero
            if(ntrozos >= 3){
                if(open(trozos[2], O_CREAT|O_EXCL, permisos) < 0) perror("create failed: ");
            }else{
                char direccion[100];
            char *error = getcwd(direccion,100);
            if (error != NULL) printf("%s\n",direccion);
            else perror("No fue posible obtener la ruta del directorio");
            }
    
        }else{ // Crear directorio con lo que está en trozos[1]
        // mkdir
        if(mkdir(trozos[1],permisos) == -1) perror("create failed: ");

        }
    }
}