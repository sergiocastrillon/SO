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

#include "listam.h"

#define TAMANO 2048

//
void allocate(char * trozos[], int ntrozos, tListM listaM);


