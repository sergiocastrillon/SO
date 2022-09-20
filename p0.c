#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
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

void infosis(){
    struct utsname info;
    int fallo = 0;
    fallo = uname(&info);
    if(fallo != 0) perror("uname failed: ");
    else{
        printf("%s %s %s %s %s\n",info.sysname,info.nodename,info.release,info.version,info.machine);
    }
}





bool procesarEntrada(char entrada[],tList lista){
    bool continuar = true;
    char* trozos[3]; // *** stack smashing detected ***: terminated si el numero de trozos supera el tamaño de char*??
    // Si se introduce un espacio al final del comando el programa peta, relacionado con lo de arriba??
    int ntrozos = TrocearCadena(entrada,trozos);
    // Si la cadena está vacía ntrozos = 0, tener en cuenta para comprobación de errores?
    // Las opciones conservan el guión (por ejemplo: fecha "-n")
    if(ntrozos!=0){
        if(strcmp(trozos[0],"fin") == 0 || strcmp(trozos[0],"salir") == 0 || strcmp(trozos[0],"bye") == 0) continuar = false;
        else if(strcmp(trozos[0],"autores")==0) autores(trozos,ntrozos);
        else if(strcmp(trozos[0],"pid")==0) pid(trozos,ntrozos);
        else if(strcmp(trozos[0],"carpeta")==0) printf("Prueba 2");
        else if(strcmp(trozos[0],"fecha")==0) fecha(trozos,ntrozos);
        else if(strcmp(trozos[0],"hist")==0) hist(trozos,ntrozos,lista);
        else if(strcmp(trozos[0],"comando")==0) comando(trozos,ntrozos,lista);
        else if(strcmp(trozos[0],"infosis")==0) infosis();
        else if(strcmp(trozos[0],"ayuda")==0) printf("Prueba 7");
        else printf("Comando no reconocido\n");
        // Estructura if else?? Funciona aparentemente
        // solo usar esta sección para llamar a una función o para usar una sola intrucción (posible para el fin??)
        // para evitar usar parentesis y dificultando la lectura del codigo
    }
    return continuar;
}



int main() {
    bool continuar = true;
    tList lista;
    CreateList(&lista);
    while (continuar){
        imprimirPrompt();
        char entrada[20];
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



void comando(char* trozos[], int ntrozos, tList lista){
    char* error;
    bool ejecucion = true;
    char comandoHist[MAX];
    if(ntrozos > 1){
        // Obtenemos la orden de la lista asignada al número introducido
        long n = strtol(trozos[1],&error,10);
        if(error!=trozos[1] && n >=0){
            tPosL pos = lista;
            int i = 0;
            while(i<n && pos != NULL){
                pos = next(pos,lista);
                i++;
            }
            getElement(pos,comandoHist,lista);
            char* trozos1[3];
            char copia[MAX];
            strcpy(copia,comandoHist); // Creamos una copia para evitar problemas si la orden del historial es "comando"
            TrocearCadena(copia,trozos1);

            // Comprobamos si la orden del historial es "comando" si lo es toca mirar el número para evitar bucles
            if(strcmp(trozos1[0],"comando")==0 && trozos1[1] != NULL){
                long z = strtol(trozos1[1],&error,10);
                if(error!=trozos1[1] && n==z){
                    printf("Error: Detenido el comando por posibilidad de bucle infinito\n");
                    ejecucion = false;
                } 
            }
        }
    }else{
        printf("Error: No se ha reconocido el número o es menor que 0\n");
        ejecucion = false;
    }

    if(ejecucion){
        printf("Ejecutando orden del hist (%s): %s\n",trozos[1],comandoHist);
        procesarEntrada(comandoHist,lista);
    }
}