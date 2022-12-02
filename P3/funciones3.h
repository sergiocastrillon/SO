#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAXVAR 100
#define MAXNAME 400



void newProcessExec(char * trozos[], int ntrozos);
void priority(char * trozos[], int ntrozos);