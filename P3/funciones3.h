#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/resource.h>


#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "listap.h"

#define MAXVAR 1000
#define MAXNAME 400


void Cmd_fork();


void newProcessExec(char * trozos[], int ntrozos);
void priority(char * trozos[], int ntrozos);
void showvar(char * trozos[], int ntrozos,char *arg3[]);
void changevar(char * trozos[], int ntrozos, char *arg3[]);
void showenv(char * trozos[], int ntrozos, char *arg3[]);
void exec(char *trozos[], int ntrozos,bool exec);