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

struct SEN{
	char *nombre;
	int senal;
};




void Cmd_fork(tListP listp);


void newProcessExec(char * trozos[], int ntrozos,tListP listp);
void priority(char * trozos[], int ntrozos);
void showvar(char * trozos[], int ntrozos,char *arg3[]);
void changevar(char * trozos[], int ntrozos, char *arg3[]);
void showenv(char * trozos[], int ntrozos, char *arg3[]);
void execute(char *trozos[], int ntrozos,bool exec);
void listjobs(tListP list);
void deljobs(char * trozos[], int ntrozos,tListP list);
void job(char * trozos[], int ntrozos,tListP list);