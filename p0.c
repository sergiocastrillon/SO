#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
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
    printf("~$ ");
}

void leerEntrada(char entrada[]){
    fgets(entrada, MAX + 1, stdin);
}

void opcionInvalida(char comando[],char opcion[]){
    printf("%s: %s no es una opción válida\n",comando,opcion);
}

void fin(){
    printf("Fin del programa\n");
    // Terminación del programa
    /* Opciones:
     * 1. Devolver 1, y que procesarEntrada devuelva bool que se asigne a "terminado" para salir del bucle
     * 2. Función exit??
     ? */
}

void autores(char* trozos[],int ntrozos){
    if(ntrozos==1) printf("Mario : mario.freire\nSergio : s.castrillon\n");
    else if(strcmp(trozos[1],"-l")==0) printf("mario.freire\ns.castrillon\n");
    else if((strcmp(trozos[1],"-n")==0)) printf("Mario\nSergio\n");
    else opcionInvalida(trozos[0],trozos[1]);
    
}

void pid(char* trozos[],int ntrozos){
    pid_t nprocess;
    nprocess = getpid();
    printf("Pid de shell: %d",nprocess);
}


void procesarEntrada(char comando[]){
    char* trozos[3]; // *** stack smashing detected ***: terminated si el numero de trozos supera el tamaño de char*??
    // Si se introduce un espacio al final del comando el programa peta, relacionado con lo de arriba??
    int ntrozos = TrocearCadena(comando,trozos);
    // Si la cadena está vacía ntrozos = 0, tener en cuenta para comprobación de errores?
    // Las opciones conservan el guión (por ejemplo: fecha "-n")
    if(ntrozos!=0){
        if(strcmp(trozos[0],"fin") == 0 || strcmp(trozos[0],"salir") == 0 || strcmp(trozos[0],"bye") == 0) fin();
        else if(strcmp(trozos[0],"autores")==0) autores(trozos,ntrozos);
        else if(strcmp(trozos[0],"pid")==0) printf("Prueba 1");
        else if(strcmp(trozos[0],"carpeta")==0) printf("Prueba 2");
        else if(strcmp(trozos[0],"fecha")==0) printf("Prueba 3");
        else if(strcmp(trozos[0],"hist")==0) printf("Prueba 4");
        else if(strcmp(trozos[0],"comando")==0) printf("Prueba 5");
        else if(strcmp(trozos[0],"infosis")==0) printf("Prueba 6");
        else if(strcmp(trozos[0],"ayuda")==0) printf("Prueba 7");
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
