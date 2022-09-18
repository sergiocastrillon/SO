#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "lista.h"
#define MAX 30

void comando(char* trozos[], int ntrozos, tList lista);

int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL) return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL) i++;
    return i;
}

void imprimirPrompt(){
    printf("~$ ");
}

void leerEntrada(char entrada[],tList lista){
    fgets(entrada, MAX + 1, stdin);
    entrada[strcspn(entrada,"\n")]=0; // Eliminar problemas que pueda crear el \n que añade fgets
    InsertElement(entrada,lista);

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
    printf("Pid de shell: %d\n",nprocess);
}

void hist(char* trozos[],int ntrozos,tList lista){
    // Separamos casos, si el comando va acompañado de alguna opción o no
    if(ntrozos == 1){ // No se introdujo ninguna opción, imprimir historial
        int x = 0;
        for(tPosL i = lista; i!= NULL; i = next(i,lista)){
            char comando[20];
            getElement(i,comando,lista);
            printf("%d -> %s\n",x,comando);
            x++;
        }

    }else if(strcmp(trozos[1],"-c")==0){ // Borrar la lista
        tPosL pos = first(lista);
        char elemento[MAX];
        getElement(pos,elemento,lista);
        while(pos != NULL && strcmp(elemento,"\0")!=0){
            RemoveElement(pos, lista);
            getElement(pos,elemento,lista);
        }

    }else{
        char* error;
        long n = strtol(trozos[1],&error,10);
        if(error!=trozos[1]){ // Comprobamos strtol ha convertido el número bien o ha devuelto un error
            n = n * -1;
            int i = 0;
            tPosL pos = first(lista);
            while((i < n) && pos != NULL){ // Probar si hace falta poner \0
                char comando[MAX];
                getElement(pos,comando,lista);
                printf("%d -> %s\n",i,comando);
                i++;
                pos = next(pos,lista);
            }
        }else opcionInvalida("hist",trozos[1]);
    }
}

void fecha(char* trozos[], int ntrozos) {
// Obtenemos la hora
    time_t t = time(NULL);

    struct tm tiempoLocal = *localtime(&t);

    char fecha[70];
    char hora[70];
// Definimos el formato en el que se muestra el tiempo
    char *formato1 = "%d/%m/%Y";
    char *formato2 = "%H:%M:%S";
// Obtenemos la fecha y la hora
    strftime(fecha, sizeof fecha, formato1, &tiempoLocal);
    strftime(hora, sizeof hora, formato2, &tiempoLocal);


    if (ntrozos == 1) printf("%s %s\n", fecha, hora);
    else if(strcmp(trozos[1], "-d") == 0) printf("%s\n", fecha);
    else if(strcmp(trozos[1], "-h") == 0) printf("%s\n", hora);
    else opcionInvalida("fecha", trozos[1]);
}





void procesarEntrada(char entrada[],tList lista){
    char* trozos[3]; // *** stack smashing detected ***: terminated si el numero de trozos supera el tamaño de char*??
    // Si se introduce un espacio al final del comando el programa peta, relacionado con lo de arriba??
    int ntrozos = TrocearCadena(entrada,trozos);
    // Si la cadena está vacía ntrozos = 0, tener en cuenta para comprobación de errores?
    // Las opciones conservan el guión (por ejemplo: fecha "-n")
    if(ntrozos!=0){
        if(strcmp(trozos[0],"fin") == 0 || strcmp(trozos[0],"salir") == 0 || strcmp(trozos[0],"bye") == 0) fin();
        else if(strcmp(trozos[0],"autores")==0) autores(trozos,ntrozos);
        else if(strcmp(trozos[0],"pid")==0) pid(trozos,ntrozos);
        else if(strcmp(trozos[0],"carpeta")==0) printf("Prueba 2");
        else if(strcmp(trozos[0],"fecha")==0) fecha(trozos,ntrozos);
        else if(strcmp(trozos[0],"hist")==0) hist(trozos,ntrozos,lista);
        else if(strcmp(trozos[0],"comando")==0) comando(trozos,ntrozos,lista);
        else if(strcmp(trozos[0],"infosis")==0) printf("Prueba 6");
        else if(strcmp(trozos[0],"ayuda")==0) printf("Prueba 7");
        else printf("Comando no reconocido\n");
        // Estructura if else?? Funciona aparentemente
        // solo usar esta sección para llamar a una función o para usar una sola intrucción (posible para el fin??)
        // para evitar usar parentesis y dificultando la lectura del codigo
    }

}



int main() {
    int terminado = -8;
    tList lista;
    CreateList(&lista);
    while (terminado!=0){
        imprimirPrompt();
        char entrada[20];
        leerEntrada(entrada,lista);
        //printf("%s",linea); // Comprobar funcionamiento de leerEntrada
        procesarEntrada(entrada,lista);

        terminado++;
    }
    tPosL i = lista; // Eliminar fugas de memoria valgrind??
    while(i!=NULL){
        tPosL x = next(i,lista);
        free(i);
        i = x;
    }
    return 0;
}



void comando(char* trozos[], int ntrozos, tList lista){
    char* error;
    if(ntrozos > 1){
        long n = strtol(trozos[1],&error,10);
        if(error!=trozos[1] && n >=0){
            tPosL pos = lista;
            int i = 0;
            while(i<n && pos != NULL){
                pos = next(pos,lista);
                i++;
            }
            char comandoHist[MAX];
            getElement(pos,comandoHist,lista);
            char* trozos1[3];
            char copia[MAX];
            strcpy(copia,comandoHist); // Se borra N si el historial contiene la orden "comando N"
            TrocearCadena(copia,trozos1);
            if(strcmp(trozos1[0],"comando")==0){
                long z = strtol(trozos1[1],&error,10);
                if(error!=trozos1[1] && n==z) printf("Error: Detenido el comando por posibilidad de bucle infinito\n");
                else procesarEntrada(comandoHist,lista);
            } else procesarEntrada(comandoHist,lista);
            //printf("%s",comandoHist);
        }else printf("Error: No se ha reconocido el número o es menor que 0\n");
    }
}