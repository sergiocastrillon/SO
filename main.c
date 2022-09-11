#include <stdio.h>
#include <string.h>
#define MAX 30
// Sería conveniente pasar los comentarios de errores o posibles alternativas a un archivo por separado
// o usar la sección de Issues de GitHub para solo dejar comentarios que ayuden a entender el código
// y marcas para saber en que parte se encuentra el codigo referido por los "comentarios de errores/alternativas"
int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL) return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL) i++;
    return i;
}

void imprimirPrompt(){
    printf("$ ");
}

void leerEntrada(char entrada[]){
    fgets(entrada, MAX + 1, stdin);
}

void fin(){
    printf("Fin del programa\n");
    // Terminación del programa
    /* Opciones:
     * 1. Devolver 1, y que procesarEntrada devuelva int que se asigne a "terminado" para salir del bucle
     * 2. Función exit??
     * 3. Goto hacia una etiqueta que quede fuera del bucle del prompt
     * 4. Usar break ???? */
}

void procesarEntrada(char comando[]){
    char* trozos[2]; // *** stack smashing detected ***: terminated si el numero de trozos supera el tamaño de char*??
    // Si se introduce un espacio al final del comando el programa peta, relacionado con lo de arriba??
    int ntrozos = TrocearCadena(comando,trozos);
    // Si la cadena está vacía ntrozos = 0, tener en cuenta para comprobación de errores?
    // Las opciones conservan el guión (por ejemplo: fecha "-n")
    if(ntrozos!=0){
        if(strcmp(trozos[0],"fin") == 0 || strcmp(trozos[0],"salir") == 0 || strcmp(trozos[0],"bye") == 0) fin();
        else if(strcmp(trozos[0],"prueba")==0) printf("Prueba");
        else if(strcmp(trozos[0],"prueba1")==0) printf("Prueba 1");
        else printf("Comando no reconocido\n");
        // Estructura if else?? Funciona aparentemente
        // solo usar esta sección para llamar a una función o para usar una sola intrucción (posible para el fin??)
        // para evitar usar parentesis y dificultando la lectura del codigo
        // Se podría usar un switch siendo esto string. Mejor no porque se complica el codigo con los breaks.
    }

}



int main() {
    int terminado = 0;
    while (terminado==0){
        imprimirPrompt();
        char comando[20];
        leerEntrada(comando);
        //printf("%s",linea); // Comprobar funcionamiento de leerEntrada
        procesarEntrada(comando);

        terminado=1;
    }
    return 0;
}
