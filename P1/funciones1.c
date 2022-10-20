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


void statAux(char * fichero, char * nombre, bool lng, bool acc, bool link){
    if(lng){
        struct stat e; // Struct con las estadisticas

        if(lstat(fichero, &e) == -1) perror("failed accesing file");
        else{
            // Datos de tiempo (modificación y acceso)
            struct tm timea;
            struct tm timem;
            char timeacc[50];
            char timemod[50];
            char symlink[600] = "\0";

            struct group *group;
            struct passwd *user;

            char *permisos = ConvierteModo2(e.st_mode);
            
            localtime_r(&e.st_mtime, &timem);
            strftime(timemod, 50, "%Y/%m/%d-%H:%M", &timem);
            localtime_r(&e.st_atime, &timea);
            strftime(timeacc, 50, "%Y/%m/%d-%H:%M", &timea);
            char time[50];

            // Obtenemos nombre de usuario y grupo
            if((user = getpwuid(e.st_uid)) == NULL) perror(""); // No se si perror funciona aqui
            if((group = getgrgid(e.st_gid)) == NULL) perror("");
            // Si acc mostramos la fecha de acceso si no la fecha de modificación
            if(acc) strcpy(time,timeacc);
            else strcpy(time,timemod);

            if(link){
                char ficheroCopia[600];
                strncpy(ficheroCopia,fichero, strlen(fichero)+1);
                if(readlink(ficheroCopia, symlink, 50) != -1){
                    printf("%s   %2ld (%8ld) %8s %8s %10s %8ld %-1s -> %-1s\n",time,e.st_nlink,e.st_ino,
                    user->pw_name,group->gr_name,permisos,e.st_size,nombre,symlink);
                }else{
                    printf("%s   %2ld (%8ld) %8s %8s %10s %8ld %-1s\n",time,e.st_nlink,e.st_ino,user->pw_name,
                    group->gr_name,permisos,e.st_size,nombre);
                } 
            }else{
                printf("%s   %2ld (%8ld) %8s %8s %10s %8ld %-1s\n",time,e.st_nlink,e.st_ino,user->pw_name,
                group->gr_name,permisos,e.st_size,nombre); // long
            } 
        } 
    }else{// Información corta de uno o varios archivos
        struct stat e;
        if(lstat(fichero, &e) == -1) perror("failed accesing file");
        else printf("%10ld  %s\n",e.st_size,nombre);
    }
}




void stat_o(char * trozos[], int ntrozos){
    if(ntrozos < 2) directorio();
    else{
        bool lng = false,acc = false,link = false;
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
        for(int i = control + 1; i < ntrozos; i++){
            statAux(trozos[i],trozos[i],lng,acc,link);
        }
        if(control + 1 == ntrozos) directorio();
    }
}



// Función recursiva auxiliar de list (-reca)
void recA(char * directorio, bool hid, bool acc, bool link, bool lng){
    
    DIR *d;
    struct dirent *fic;
    tList ldir;

    // Lista para guardar directorios
    //CreateList(&ldir);

    d = opendir(directorio);
    if (d) {
        printf("*********  %s\n",directorio);
        while ((fic = readdir(d)) != NULL){

            // Encadenamos el directorio a la dirección actual para
            // que el ordenador pueda acceder a carpetas con una profundidad
            // mayor a 1
            char destino[600];
            strncpy(destino,directorio,600);
            strcat(destino,"/");
            strcat(destino,fic->d_name);
            struct stat e;
            // Guardamos en la listas los directorios (distintos de . y ..)
            if(lstat(destino, &e) == -1) perror("list error1");
            else{
                if(hid || fic->d_name[0] != '.'){
                    statAux(destino, fic->d_name,lng,acc,link);
                } 
            }
            
        }
        closedir(d);
    }
    d = opendir(directorio);
    if(d){
        while ((fic = readdir(d)) != NULL){

            
            char destino[600];
            strncpy(destino,directorio,600);
            strcat(destino,"/");
            strcat(destino,fic->d_name);
            struct stat e;
            // Guardamos en la listas los directorios (distintos de . y ..)
            if(lstat(destino, &e) == -1) perror("list error1");
            else{
                if(S_ISDIR(e.st_mode) && strcmp(fic->d_name,".") != 0
                    && strcmp(fic->d_name,"..") != 0) recA(destino,hid,acc,link,lng);
            }
        }
    }
    closedir(d);
}




// Función recursiva auxiliar recB
void recB(char * directorio, bool hid, bool acc, bool link, bool lng) {
    DIR *d;
    struct dirent *fic;

    d = opendir(directorio);
    if (d) {
        while ((fic = readdir(d)) != NULL){
            // Concatenamos las rutas para que siempre sean relativas
            // al directorio actual
            char destino[MAX];
            strcpy(destino,directorio);
            strcat(destino,"/");
            strcat(destino,fic->d_name);

            struct stat e;
            // Llamada recursiva a la función para los subdirectorios
            if(lstat(destino, &e) == -1) perror("list error");
            else{
                if(S_ISDIR(e.st_mode) && strcmp(fic->d_name,".") != 0
                && strcmp(fic->d_name,"..") != 0){
                    if(fic->d_name[0] != '.' || hid) recB(destino,hid,acc,link,lng);
                }
            }
        }
        closedir(d);
        printf("*********  %s\n",directorio);
        d = opendir(directorio); // No tenemos permisos en el directorio a listar
        while ((fic = readdir(d)) != NULL){
            if(hid || fic->d_name[0] != '.'){
            // Usar una dirección relativa a la carpeta actual
                char reldir[MAX];
                strcpy(reldir,directorio);
                strcat(reldir,"/");
                strcat(reldir,fic->d_name);
                statAux(reldir,fic->d_name,lng,acc,link);
            }  
        }
        closedir(d);
    } else return;
}

// Función list aux
void listAux(char * directorio, bool reca, bool recb, bool hid, 
bool acc, bool link, bool lng){
    // Comprobamos que no hay problemas y que tenemos permisos para el directorio
    DIR *d;
    d = opendir(directorio);
    if(!d){
        perror("list failed");
        return; // Como la función solo recibe un directorio, si no podemos acceder a este
        // salimos de la función
    }else closedir(d);

    if(reca) recA(directorio,hid,acc,link,lng);
    else if(recb) recB(directorio,hid,acc,link,lng);
    else{ // Si no hay recursividad

        char destino[MAX];
        struct stat e;
        DIR *d;
        struct dirent *fic;

        d = opendir(directorio);
        printf("*********  %s\n",directorio);
        while ((fic = readdir(d)) != NULL){
            if(hid || fic->d_name[0] != '.'){
                // Concatenamos la ruta por si queremos listar un directorio distinto al actual
                strcpy(destino,directorio);
                strcat(destino,"/");
                strcat(destino,fic->d_name);

                if(lstat(destino, &e) == -1) perror("error trying to access to a file");
                else statAux(destino,fic->d_name,lng,acc,link);
            }
        }   
        closedir(d);
    }
}

void list(char * trozos[], int ntrozos){
    if(ntrozos < 2) directorio();
    else{
        bool reca = false, recb = false, hid = false, lng = false, acc = false, link = false;
        int control = 0; // Contador de cuantos trozos son opciones del comando

        for(int i = 1; i < ntrozos; i++){
            if(strcmp(trozos[i],"-reca") == 0){
                reca = true;
                control++;
            } 
            if(strcmp(trozos[i],"-recb") == 0){
                recb = true;
                control++;
            }
            if(strcmp(trozos[i],"-hid") == 0){
                hid = true;
                control++;
            }
            if(strcmp(trozos[i],"-acc") == 0){
                acc = true;
                control++;
            }
            if(strcmp(trozos[i],"-long") == 0){
                lng = true;
                control++;
            }
            if(strcmp(trozos[i],"-link") == 0){
                link = true;
                control++;
            }
        }

        for(int i = control + 1; i < ntrozos; i++){
            listAux(trozos[i],reca,recb,hid,acc,link,lng);
        }
        if(control + 1 == ntrozos) directorio();
    }   
}



void delete(char * trozos[], int ntrozos){
    struct stat e; // Struct con las estadisticas
    
    for(int i = 1; i < ntrozos; i++){
        if(lstat(trozos[i], &e) == -1) perror("delete failed");
        else{
            if(S_ISDIR(e.st_mode)){ // Separamos las carpetas de los ficheros
                if(rmdir(trozos[i]) == -1) perror("delete failed");
            } 
            else{
                if(unlink(trozos[i]) == -1) perror("delete failed");
            }
        }
    }
}

void deleteRecAux(char * directorio){
    DIR *d;
    struct dirent *fic;
    char direccion[MAX];

    d = opendir(directorio);
    if (d) {
        while ((fic = readdir(d)) != NULL){
            // Direccion en base a la ruta actual
            strcpy(direccion,directorio);
            strcat(direccion,"/");
            strcat(direccion,fic->d_name);

            // Si estamos en ".." o ".", saltamos una iteracion del bucle
            if(strcmp(fic->d_name,".") == 0 || strcmp(fic->d_name,"..") == 0) continue;
            
            struct stat e;
            if(lstat(direccion, &e) == -1){
                // Si no se puede obtener información del
                // archivo es que nos faltan permisos o tenemos algun fallo, 
                // detenemos el borrado ya que no podremos terminar de borrar la carpeta original
                perror("failed removing");
                break;
            }else{
                if(S_ISDIR(e.st_mode)){
                    // Separamos las carpetas de los ficheros
                    if(rmdir(direccion) == -1){
                        deleteRecAux(direccion);
                        // Si no es capaz de borrar la carpeta tras llamar a deleteRecAux
                        // es que nos faltan permisos en ese directorio
                        if(rmdir(direccion) == -1) break;
                    }
                }else{
                    if(unlink(direccion) == -1){
                        // Si falla entonces posiblemente no tengamos permisos 
                        //en el directorio asi que podemos salir de la función
                        // El break evita saltarse el closedir
                        perror("failed trying to remove a file");
                        break;
                    } 
                } 
            }
        }
        closedir(d);
    }      
    
}

void deleteRec(char * trozos[], int ntrozos){
    struct stat e;
    for(int i = 1; i < ntrozos; i++){
        if(lstat(trozos[i], &e) == -1) perror("deltree failed");
        else{
            if(S_ISDIR(e.st_mode)){ // Separamos las carpetas de los ficheros
                if(rmdir(trozos[i]) == -1){
                    deleteRecAux(trozos[i]);
                    // rmdir debería poder borrar ahora, si no puede es que se ha presentado
                    // algún fallo vaciando el directorio, en cuyo caso el error correspondiente
                    // ya se habrá mostrado y no hace falta mostrarlo aquí
                    rmdir(trozos[i]);
                }
            }else{
                if(unlink(trozos[i]) ==-1 ) perror("deltree failed");
            }
        }
    }
}