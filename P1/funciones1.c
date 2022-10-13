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
    if(lng || acc || link){
            struct stat e; // Struct con las estadisticas
            if(lstat(fichero, &e) == -1) perror("stat failed");
            else{
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
                
                if((user = getpwuid(e.st_uid)) == NULL) perror(""); // No se si perror funciona aqui
                if((group = getgrgid(e.st_gid)) == NULL) perror("");

                // Print information
                if(link){
                    
                    if(readlink(fichero, symlink, 50) == -1){ // Si no es un enlace simbólico
                        printf("%s   %d (%d)   %s   %s %s    %d %s\n",timeacc,e.st_nlink,e.st_ino,user->pw_name,
                        group->gr_name,permisos,e.st_size,fichero);
                    }else printf("%s   %d (%d)   %s   %s %s    %d %s -> %s\n",timemod,e.st_nlink,e.st_ino,
                    user->pw_name,group->gr_name,permisos,e.st_size,fichero,symlink); // link

                }else if(acc){

                    localtime_r(&e.st_atime, &timea);
                    strftime(timeacc, 50, "%Y/%m/%d-%H:%M", &timea);
                    printf("%s   %d (%d)   %s   %s %s    %d %s\n",timeacc,e.st_nlink,e.st_ino,
                    user->pw_name,group->gr_name,permisos,e.st_size,fichero); // acc
                }
                else{
                    printf("%s   %d (%d)   %s   %s %s    %d %s\n",timemod,e.st_nlink,e.st_ino,user->pw_name,
                    group->gr_name,permisos,e.st_size,fichero); // long
                } 
            } 
        }else{// Información corta de uno o varios archivos
            struct stat e;
            if(lstat(fichero, &e) == -1) perror("stat failed:");
            else printf("%10d  %s\n",e.st_size,fichero);
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
        for(int i = control + 1; i < ntrozos; i++){
            statAux(trozos[i],lng,acc,link);
        }
        if(control + 1 == ntrozos) directorio();
    }else directorio();
}


void listAux(char * directorio, bool reca, bool recb, bool hid){
    // Si no hay recursividad
    if(!reca && !recb){
        char destino[MAX];
        struct stat e;
        printf("*********  %s\n",directorio);
        DIR *d;
        struct dirent *fic;
        d = opendir(directorio);
        if (d) {
            while ((fic = readdir(d)) != NULL) {
                if(hid || fic->d_name[0] != '.'){
                    
                    strcpy(destino,directorio);
                    strcat(destino,"/");
                    strcat(destino,fic->d_name);
                    if(lstat(destino, &e) == -1) perror("");
                    else printf("%10d  %s\n",e.st_size,fic->d_name);
                }
            }   
            closedir(d);
        }
    }else{
        // reca funciona como una cola, la primera carpeta en encontrarse se imprime
        if(reca){ // Recursividad hacia abajo
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
                    if(lstat(destino, &e) == -1) perror("Prueba");
                    if(S_ISDIR(e.st_mode) && strcmp(fic->d_name,".") != 0
                    && strcmp(fic->d_name,"..") != 0){
                        if(fic->d_name[0] != '.' || hid) InsertElement(destino,ldir);
                    } 
                    if(hid || fic->d_name[0] != '.'){
                        printf("%10d  %s\n",e.st_size,fic->d_name);
                    } 
                }
                closedir(d);
            }
            if(!isEmptyList(ldir)){
                for(tPosL pos = first(ldir); pos != NULL; pos = next(pos,ldir)){
                    char directorio1[MAX];
                    getElement(pos,directorio1,ldir);
                    listAux(directorio1,reca,recb,hid);
                }
            }
            tPosL i = ldir; // Eliminar fugas de memoria valgrind??
            while(i!=NULL){
            tPosL x = next(i,ldir);
            free(i);
            i = x;
            }
        }else{ // Recursivididad hacia arriba
            DIR *d;
            struct dirent *fic;

            d = opendir(directorio);
            if (d) {
                while ((fic = readdir(d)) != NULL){
                    char destino[MAX];
                    strcpy(destino,directorio);
                    strcat(destino,"/");
                    strcat(destino,fic->d_name);
                    struct stat e;
                    // Guardamos en la listas los directorios (distintos de . y ..)
                    if(lstat(destino, &e) == -1) perror("Prueba");
                    if(S_ISDIR(e.st_mode) && strcmp(fic->d_name,".") != 0
                    && strcmp(fic->d_name,"..") != 0){
                        if(fic->d_name[0] != '.' || hid) listAux(destino,reca,recb,hid);
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
                        else printf("%10d  %s\n",e.st_size,fic->d_name);
                    }  
                }
                closedir(d);   
        }
    }
}

void list(char * trozos[], int ntrozos){
    if(ntrozos < 2) directorio();
    else{
        bool reca,recb,hid;
        int control = 0;

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