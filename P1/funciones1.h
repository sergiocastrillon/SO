#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

#include "lista.h"


char LetraTF (mode_t m);
char * ConvierteModo2 (mode_t m);
void directorio();
void create(char * trozos[], int ntrozos);
void statAux(char * fichero, bool lng, bool acc, bool link);
void stat_o(char * trozos[], int ntrozos);
void list(char * trozos[], int ntrozos);
