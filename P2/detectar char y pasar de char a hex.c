#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL) return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL) i++;
    return i;
}


int main() {
    char  h[10];
    char * trozos[10];
    scanf("%s",h);
    TrocearCadena(h,trozos);
    if(strlen(trozos[0])==3){
        void * p = strchr(trozos[0],39);
        if(p != NULL && p != strrchr(trozos[0],39)){
            char a = trozos[0][1];
            printf("%c %x",a,a);
        }
    }
    //printf("%s",trozos[0]);
    return 0;
}
