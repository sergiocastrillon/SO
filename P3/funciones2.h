#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdint.h>
#include <ctype.h>

#include "listam.h"


#define TAMANO 2048




//
void allocate(char * trozos[], int ntrozos, tListM listaM);
void memfill(char * trozos[], int ntrozos);
void memdump(char * trozos[], int ntrozos);
void deallocate(char * trozos[], int ntrozos, tListM list);
void io(char * trozos[], int ntrozos);
void recursiva(char * trozos[], int ntrozos);
void memoria(char * trozos[], int ntrozos,tListM list);
void deallocate_all(tListM list);


