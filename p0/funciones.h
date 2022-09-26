#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <errno.h> 
#include "lista.h"


#define MAX 40

int TrocearCadena(char * cadena, char * trozos[]);
void imprimirPrompt();
void leerEntrada(char entrada[],tList lista);
bool procesarEntrada(char entrada[],tList lista);
void opcionInvalida(char comando[],char opcion[]);

void autores(char* trozos[],int ntrozos);
void pid(char* trozos[],int ntrozos);
void carpeta(char* trozos[],int ntrozos);
void fecha(char* trozos[], int ntrozos);
void hist(char* trozos[],int ntrozos,tList lista);
void comando(char* trozos[], int ntrozos, tList lista, char entrada[]);
void infosis();
void ayuda(char* trozos[], int ntrozos);