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


void statAux(char * fichero, bool lng, bool acc, bool link){
    if(lng){
        struct stat e; // Struct con las estadisticas

        if(lstat(fichero, &e) == -1) perror("stat failed");
        else{
            // Datos de tiempo (modificación y acceso)
            struct tm timea;
            struct tm timem;
            char timeacc[50];
            char timemod[50];
            char symlink[50];

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
                if(readlink(fichero, symlink, 50) != -1){
                    strcat(fichero,"->");
                    strcat(fichero,symlink);
                }
                printf("%s   %ld (%ld)   %s   %s %s    %ld %s\n",time,e.st_nlink,e.st_ino,
                user->pw_name,group->gr_name,permisos,e.st_size,fichero);
                
            }else{
                printf("%s   %ld (%ld)   %s   %s %s    %ld %s\n",time,e.st_nlink,e.st_ino,user->pw_name,
                group->gr_name,permisos,e.st_size,fichero); // long
            } 
        } 
    }else{// Información corta de uno o varios archivos
        struct stat e;
        if(lstat(fichero, &e) == -1) perror("stat failed:");
        else printf("%10ld  %s\n",e.st_size,fichero);
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
            statAux(trozos[i],lng,acc,link);
        }
        if(control + 1 == ntrozos) directorio();
    }
}



// Función recursiva auxiliar de list (-reca)
void recA(char * directorio, bool hid){
    printf("*********  %s\n",directorio);
    DIR *d;
    struct dirent *fic;
    tList ldir;

    // Lista para guardar directorios
    CreateList(&ldir);

    d = opendir(directorio);
    if (d) {
        while ((fic = readdir(d)) != NULL){

            // Encadenamos el directorio a la dirección actual para
            // que el ordenador pueda acceder a carpetas con una profundidad
            // mayor a 1
            char destino[MAX];
            strcpy(destino,directorio);
            strcat(destino,"/");
            strcat(destino,fic->d_name);
            struct stat e;
            // Guardamos en la listas los directorios (distintos de . y ..)
            if(lstat(destino, &e) == -1) perror("list error");
            else{
                if(hid || fic->d_name[0] != '.'){
                    if(S_ISDIR(e.st_mode) && strcmp(fic->d_name,".") != 0
                    && strcmp(fic->d_name,"..") != 0) InsertElement(destino,ldir);

                    printf("%10ld  %s\n",e.st_size,fic->d_name); 
                } 
            }
            
        }
        closedir(d);
    }

    if(!isEmptyList(ldir)){
        for(tPosL pos = first(ldir); pos != NULL; pos = next(pos,ldir)){
            char directorio1[MAX];
            getElement(pos,directorio1,ldir);
            recA(directorio1,hid);
        }
    }
    deleteList(&ldir);
}




// Función recursiva auxiliar recB
void recB(char * directorio, bool hid){
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
            // Guardamos en la listas los directorios (distintos de . y ..)
            if(lstat(destino, &e) == -1) perror("list error");
            else{
                if(S_ISDIR(e.st_mode) && strcmp(fic->d_name,".") != 0
                && strcmp(fic->d_name,"..") != 0){
                    if(fic->d_name[0] != '.' || hid) recB(destino,hid);
                }
            }
        }
        closedir(d);
    }
    printf("*********  %s\n",directorio);
    d = opendir(directorio);
    // No hace falta comprobar si d se inicializó porque ya está de antes
    // comprobado
    while ((fic = readdir(d)) != NULL){
        if(hid || fic->d_name[0] != '.'){
            // Usar una dirección relativa a la carpeta actual
            char reldir[MAX];
            strcpy(reldir,directorio);
            strcat(reldir,"/");
            strcat(reldir,fic->d_name);
            struct stat e;
            if(lstat(reldir, &e) == -1) perror("");
            else printf("%10ld  %s\n",e.st_size,fic->d_name);
        }  
    }
    closedir(d);
}


void listAux(char * directorio, bool reca, bool recb, bool hid){
    
    if(reca) recA(directorio,hid);
    else if(recb) recB(directorio,hid); 
    else{ // Si no hay recursividad

        char destino[MAX];
        struct stat e;
        DIR *d;
        struct dirent *fic;

        printf("*********  %s\n",directorio);
        d = opendir(directorio);
        if (d) {
            while ((fic = readdir(d)) != NULL){
                if(hid || fic->d_name[0] != '.'){
                    // Concatenamos la ruta por si queremos listar un directorio distinto al actual
                    strcpy(destino,directorio);
                    strcat(destino,"/");
                    strcat(destino,fic->d_name);

                    if(lstat(destino, &e) == -1) perror("");
                    else printf("%10ld  %s\n",e.st_size,fic->d_name);
                }
            }   
            closedir(d);
        }
    }
}

void list(char * trozos[], int ntrozos){
    if(ntrozos < 2) directorio();
    else{
        bool reca = false, recb = false, hid = false;
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
        }

        for(int i = control + 1; i < ntrozos; i++){
            listAux(trozos[i],reca,recb,hid);
        }
        if(control + 1 == ntrozos) directorio();
    }   
}