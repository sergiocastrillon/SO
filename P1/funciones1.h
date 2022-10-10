#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>


char LetraTF (mode_t m);
char * ConvierteModo2 (mode_t m);
void directorio();
void create(char * trozos[], int ntrozos);
void stat_o(char * trozos[], int ntrozos);
