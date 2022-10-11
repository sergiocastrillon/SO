#include "funciones1.h"





char LetraTF (mode_t m){
     switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */ 
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
     }
}


char * ConvierteModo2 (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}





void directorio(){
    char direccion[100];
        char *error = getcwd(direccion,100);
        if (error != NULL) printf("%s\n",direccion);
        else perror("No fue posible obtener la ruta del directorio");
}

void create(char * trozos[], int ntrozos){
    if(ntrozos == 1) {
        directorio();
    }
    // Llamada a carpeta ??
    else{
        mode_t permisos = S_IRWXU | S_IROTH | S_IRUSR | S_IXGRP | S_IXOTH | S_IRGRP;
        if(strcmp(trozos[1],"-f")==0){ // Crear fichero
            if(ntrozos >= 3){
                if(open(trozos[2], O_CREAT|O_EXCL, permisos) < 0) perror("create failed: ");
            }else{
                directorio();
            }
    
        }else{ // Crear directorio con lo que está en trozos[1]
        // mkdir
        if(mkdir(trozos[1],permisos) == -1) perror("create failed: ");

        }
    }
}

void stat_o(char * trozos[], int ntrozos){
    if(ntrozos >= 2){
        bool lng,acc,link;
        int control = 0;
        for(int i = 1; i < ntrozos; i++){
            if(strcmp(trozos[i],"-long") == 0){
                lng = true;
                control++;
            } 
            if(strcmp(trozos[i],"-acc") == 0){
                acc = true;
                control++;
            }
            if(strcmp(trozos[i],"-link") == 0){
                link = true;
                control++;
            }
        }
        if(lng || acc || link){
            struct stat estadisticas;
            if(lstat(trozos[control+2], &estadisticas) == -1) perror("stat failed:");
            else{
                if(acc) printf("%d\t %s\n",estadisticas.st_size,trozos[1]); // acc
                else if(link) printf("%d\t %s\n",estadisticas.st_size,trozos[1]); // link
                else printf("%d\t %s\n",estadisticas.st_size,trozos[1]); // long
            } 
        }else{// Información corta de uno o varios archivos
            struct stat estadisticas;
            if(lstat(trozos[1], &estadisticas) == -1) perror("stat failed:");
            else printf("%d\t %s\n",estadisticas.st_size,trozos[1]);
        }
        }else directorio();
    
        
             

    }
}
