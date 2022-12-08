#include "funciones.h"

// TrocearCadena

int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL) return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL) i++;
    return i;
}

// ***********


// imprimirPrompt
void imprimirPrompt(){
    printf("~$ ");
}

// ***********


// leerEntrada
void leerEntrada(char entrada[],tList lista){
    char clear[2];
    strcpy(clear,"\0"); // Evitar fallos de valgrind
    fgets(entrada, MAX + 1, stdin);
    // Limpieza de stdin en caso de que algún usuario exceda el maximo de carácteres que lee fgets
    while (strchr(entrada, '\n') == NULL && clear[0] != '\n') 
        fgets(clear, 2, stdin);
    clear[0] = 0;
    entrada[strcspn(entrada,"\n")]=0; // Eliminar problemas que pueda crear el \n que añade fgets
    InsertElement(entrada,lista); // Si InsertElement fuera en procesarEntrada no se podría llamar
    // a la función para la orden comando
}

// ***********


// procesarEntrada

// Modificar para añadir nuevos comandos
// Recuerda añadir también la función al comando "ayuda"

// No hay alguna manera de "implementar" un diccionario en C??

bool procesarEntrada(char entrada[],tList lista,tListM listam, char *arg3[]){
    bool continuar = true;
    char* trozos[50]; // *** stack smashing detected ***: terminated si el numero de trozos supera el tamaño de char*??
    // Si se introduce un espacio al final del comando el programa peta, relacionado con lo de arriba??
    char copia[MAX];
    strcpy(copia,entrada);
    int ntrozos = TrocearCadena(entrada,trozos);
    // Si la cadena está vacía ntrozos = 0, tener en cuenta para comprobación de errores?
    // Las opciones conservan el guión (por ejemplo: fecha "-n")
    if(ntrozos!=0){
        if(strcmp(trozos[0],"fin") == 0 || strcmp(trozos[0],"salir") == 0 || strcmp(trozos[0],"bye") == 0) continuar = false;
        else if(strcmp(trozos[0],"clear")==0) system("clear"); // Limpia la terminal
        else if(strcmp(trozos[0],"autores")==0) autores(trozos,ntrozos);
        else if(strcmp(trozos[0],"pid")==0) pid(trozos,ntrozos);
        else if(strcmp(trozos[0],"carpeta")==0) carpeta(trozos,ntrozos);
        else if(strcmp(trozos[0],"fecha")==0) fecha(trozos,ntrozos);
        else if(strcmp(trozos[0],"hist")==0) hist(trozos,ntrozos,lista);
        else if(strcmp(trozos[0],"comando")==0) comando(trozos,ntrozos,lista,copia,listam,arg3);
        else if(strcmp(trozos[0],"infosis")==0) infosis();
        else if(strcmp(trozos[0],"ayuda")==0) ayuda(trozos,ntrozos);
        // Sistema ficheros
        else if(strcmp(trozos[0],"create")==0) create(trozos,ntrozos);
        else if(strcmp(trozos[0],"stat")==0) stat_o(trozos,ntrozos);
        else if(strcmp(trozos[0],"list")==0) list(trozos,ntrozos);
        else if(strcmp(trozos[0],"delete")==0) delete(trozos,ntrozos);
        else if(strcmp(trozos[0],"deltree")==0) deleteRec(trozos,ntrozos);
        // Memoria
        else if(strcmp(trozos[0],"allocate")==0) allocate(trozos,ntrozos,listam);
        else if(strcmp(trozos[0],"memfill")==0) memfill(trozos,ntrozos);
        else if(strcmp(trozos[0],"memdump")==0) memdump(trozos,ntrozos);
        else if(strcmp(trozos[0],"deallocate")==0) deallocate(trozos,ntrozos,listam);
        else if(strcmp(trozos[0],"i-o")==0) io(trozos,ntrozos);
        else if(strcmp(trozos[0],"recursiva")==0) recursiva(trozos,ntrozos);
        else if(strcmp(trozos[0],"memoria")==0) memoria(trozos,ntrozos,listam);
        // Procesos
        else if(strcmp(trozos[0],"priority")==0) priority(trozos,ntrozos);
        else if(strcmp(trozos[0],"showvar")==0) showvar(trozos,ntrozos,arg3);
        else if(strcmp(trozos[0],"changevar")==0) changevar(trozos,ntrozos,arg3);
        else if(strcmp(trozos[0],"showenv")==0) showenv(trozos,ntrozos,arg3);
        else if(strcmp(trozos[0],"exec")==0) exec(trozos,ntrozos,true);
        else if(strcmp(trozos[0],"fork")==0) Cmd_fork();

        else newProcessExec(trozos,ntrozos);
        // solo usar esta sección para llamar a una función o para usar una sola intrucción (posible para el fin??)
        // para evitar usar parentesis y dificultando la lectura del codigo
    }
    return continuar;
}

// ***********


// opcionInvalida

void opcionInvalida(char comando[],char opcion[]){
    // Llamar a esta función cuando se introduzca un modificador que no existe para ese comando
    printf("%s: %s no es una opción válida\n",comando,opcion);
}

// ***********



// autores

void autores(char* trozos[],int ntrozos){
    if(ntrozos==1) printf("Mario : mario.freire\nSergio : s.castrillon\n");
    else if(strcmp(trozos[1],"-l")==0) printf("mario.freire\ns.castrillon\n");
    else if((strcmp(trozos[1],"-n")==0)) printf("Mario\nSergio\n");
    else opcionInvalida(trozos[0],trozos[1]);
}

// ***********


// pid

void pid(char* trozos[],int ntrozos){
    pid_t nprocess;

    if(ntrozos == 1){
    nprocess = getpid();
    printf("Pid de la shell: %d\n",nprocess);
    }else if(strcmp(trozos[1],"-p")==0){
        nprocess = getppid();
        printf("Pid del padre de la shell: %d\n",nprocess);
    }else opcionInvalida(trozos[0],trozos[1]);
}

// ***********


// carpeta

void carpeta(char* trozos[],int ntrozos){
  char carpeta_actual[100];
  if(ntrozos == 1){
    char *error = getcwd(carpeta_actual,100);
    if (error != NULL) printf("%s\n",carpeta_actual );
    else perror("No fue posible obtener la ruta del directorio");

  }else if(ntrozos == 2){ 
	    int error = chdir(trozos[1]);
    if (error != 0) perror("No fue posible cambiar de directorio");
  }
}

// ***********


// fecha

void fecha(char* trozos[], int ntrozos){
// Obtenemos la hora
    time_t t = time(NULL);

    struct tm tiempoLocal = *localtime(&t);

    char fecha[100];
    char hora[100];
// Definimos el formato en el que se muestra el tiempo
    char *formato1 = "%d/%m/%Y";
    char *formato2 = "%H:%M:%S";
// Obtenemos la fecha y la hora
    strftime(fecha, sizeof fecha, formato1, &tiempoLocal);
    strftime(hora, sizeof hora, formato2, &tiempoLocal);


    if (ntrozos == 1) printf("%s %s\n", fecha, hora);
    else if(strcmp(trozos[1], "-d") == 0) printf("%s\n", fecha);
    else if(strcmp(trozos[1], "-h") == 0) printf("%s\n", hora);
    else opcionInvalida("fecha", trozos[1]);
}

// ***********


// hist

void hist(char* trozos[],int ntrozos,tList lista){
    // Separamos casos, si el comando va acompañado de alguna opción o no
    if(ntrozos == 1){ // No se introdujo ninguna opción, imprimir historial
        int x = 0;
        for(tPosL i = first(lista); i!= NULL; i = next(i,lista)){
            char comando[MAX];
            getElement(i,comando,lista);
            printf("%d -> %s\n",x,comando);
            x++;
        }

    }else if(strcmp(trozos[1],"-c")==0){ // Borrar la lista
        tPosL pos = first(lista);
        tPosL temp;
        // La lista siempre contiene al menos un elemento no hace falta comprobar si
        // la lista está vacía
        while(!isEmptyList(lista)){
            RemoveElement(pos, lista);
        }

    }else{
        char* error;
        long n = strtol(trozos[1],&error,10);
        if(error!=trozos[1]){ // Comprobamos strtol ha convertido el número bien o ha devuelto un error
            n = n * -1;
            int i = 0;
            tPosL pos = first(lista);
            while((i < n) && pos != NULL){ // Probar si hace falta poner \0
                char comando[MAX];
                getElement(pos,comando,lista);
                printf("%d -> %s\n",i,comando);
                i++;
                pos = next(pos,lista);
            }
        }else opcionInvalida("hist",trozos[1]);
    }
}

// ***********


// comando

void comando(char* trozos[], int ntrozos, tList lista,char entrada[],tListM listam,char *arg3[]){
    char* error;
    bool ejecucion = true;
    char comandoHist[MAX];
    if(ntrozos > 1){
        // Obtenemos la orden de la lista asignada al número introducido
        long n = strtol(trozos[1],&error,10);
        if(error != trozos[1] && n >= 0){
            tPosL pos = first(lista);
            int i = 0;
            while(i<n && pos != NULL){
                pos = next(pos,lista);
                i++;
            }
            if(pos == NULL){
                ejecucion = false;
                printf("Error: No existe una orden con ese número\n");
            }else{
                getElement(pos,comandoHist,lista);
                if(strcmp(comandoHist,entrada)==0){
                // Comprobamos si la orden del historial es la misma introducida en este ciclo
                    printf("Error: Detenido el comando por posibilidad de bucle infinito\n");
                    ejecucion = false;
                }
            }
            
        }else{
            ejecucion = false;
            printf("El número debe ser 0 o mayor\n");
        } 
    }else{
        printf("Error: No se ha reconocido el número o es menor que 0\n");
        ejecucion = false;
    }

    if(ejecucion){
        printf("Ejecutando orden del hist (%s): %s\n",trozos[1],comandoHist);
        procesarEntrada(comandoHist,lista,listam,arg3);
    }
}


// ***********

// infosis

void infosis(){
    struct utsname info;
    int fallo = 0;
    fallo = uname(&info);
    if(fallo != 0) perror("uname failed: ");
    else{
        printf("%s %s %s %s %s\n",info.sysname,info.nodename,info.release,info.version,info.machine);
    }
}


// ***********


// ayuda

void ayuda(char* trozos[], int ntrozos){
    if (ntrozos == 1) printf( "autores pid carpeta fecha hist comando infosis ayuda fin salir bye"
    "create stat list delete deltree\n");
    else{
        if(strcmp(trozos[1],"fin") == 0 || strcmp(trozos[1],"salir") == 0 || strcmp(trozos[1],"bye") == 0)
        printf("%s:\tfinaliza el shell.\n",trozos[1]);
        else if(strcmp(trozos[1],"autores")==0) printf("autores [-n|-l]:\t Imprime los nombres y"
        " logins de los autores del programa\n");       
        else if(strcmp(trozos[1],"pid")==0) printf("pid [-p]:\t Muestra el pid de la shell. [-p] imprime el pid"
        " del proceso padre del shell\n");
        else if(strcmp(trozos[1],"carpeta")==0) printf("carpeta [dir]:\t Cambia [o muestra] el directorio de trabajo actual del shell\n"); 
        else if(strcmp(trozos[1],"fecha")==0)   printf("fecha [-d|-h]:\t Imprime la fecha [-d] y hora [-h] actuales\n");
        else if(strcmp(trozos[1],"hist")==0) printf("hist [-c|-N]:\t Imprime los comandos introducidos anteriormente."
        " [-c] borra el histórico. [-N] imprime los N primeros comandos\n");
        else if(strcmp(trozos[1],"comando")==0) printf("comando [-N]:\t Ejecuta el comando número N del histórico\n");
        else if(strcmp(trozos[1],"infosis")==0) printf("infosis:\t Muestra la información de la máquina que ejecuta el shell\n");
        else if(strcmp(trozos[1],"ayuda")==0)  printf("ayuda [cmd]:\t Imprime un listado de los comandos existentes." 
        " [cmd] muestra informacion a cerca del comando cmd\n");
        else if(strcmp(trozos[1],"create")==0) printf("create [-f]:\t Crea uno o varios directorios [o archivos] con los nombres dados\n");
        else if(strcmp(trozos[1],"stat")==0) printf("stat [-long|-acc|-link]: \t Muestra información de uno o varios ficheros (o carpetas)"
        "\n -long:\t Muestra la información detallada de los ficheros\n-acc:\t Fecha de acceso en vez de mostrar la de modificación\n"
        "-link:\t Si el o los archivos dados son links simbolicos, muestra a donde apuntan,\nen caso contrario funciona como un -long\n");
        else if(strcmp(trozos[1],"list")==0) printf("list [-reca|-recb|-hid]:\tLista los ficheros y directorios del directorio dado\n"
        "-reca:\t Lista los directorios recursivamente hacia abajo\n-recb:\t Lista los directorios al revés\n-hid:\t"
        "Muestra los ficheros y directorios ocultos (empezados por '.')\nEl funcionamiento del resto de opciones es igual que en stat\n");
        else if(strcmp(trozos[1],"delete")==0) printf("delete:\tBorra los ficheros o directorios pasados\n");
        else if(strcmp(trozos[1],"deltree")==0) printf("deltree:\t Borra recursivamente los directorios ('o fichero') dados\n"
        "Tener cuidado con su uso\n");
        else if(strcmp(trozos[1],"clear")==0) printf("clear:\t Limpia la terminal. Comando añadido porque cerrar y limpiar es un coñazo ;)\n");
        else if(strcmp(trozos[1],"allocate")==0) printf("allocate [-malloc|-shared|-createshared|-mmap]:\tAsigna un bloque de memoria\n\t-malloc tam: asigna memoria malloc de tamaño tam\n"
        "\t-createshared cl tam: crea y asigna un bloque shared con la clave cl y el tamaño tam\n\t-shared cl: Asigna el bloque de memoria compartida con clave cl\n"
        "\t-mmap fich perm: mapea el fichero fich con los permisos perm (formato: rwx)\n");
        else if(strcmp(trozos[1],"deallocate")==0) printf("deallocate [-malloc|-shared|-delkey|-mmap|dir]:\t Desasigna un bloque de memoria\n"
        "\t-malloc tam: Desasigna el bloque malloc con tamaño tam\n\t-shared cl: Desasigna el bloque de memoria compartida con clave cl (la clave sigue existiendo)\n"
        "\t-delkey cl: Borra la clave cl (sin desmapear), es decir ya no se puede clonar la memoria con clave cl\n\t-mmap fich: desmapea el fichero fich\n"
        "\tdir: desasigna el bloque de memoria con dirección dir\n");
        else if(strcmp(trozos[1],"memfill")==0) printf("memfill dir n content:\tLlena n bytes a partir de la direccion dir con el char content (o por defecto 'A')\n");
        else if(strcmp(trozos[1],"memdump")==0) printf("memdump addr n:\tMuestra por pantalla el contenido de n bytes a partir de la dirección addr\n");
        else if(strcmp(trozos[1],"i-o")==0) printf("i-o [read|write] [-o]:\n\tread fich adrr n: Lee n bytes desde fich a adrr\n\twrite [-o] fich adrr n: Escribe"
        " n bytes desde adrr a fich. Con la opción -o si el fichero ya existe se sobreescribe\n");
        else if(strcmp(trozos[1],"memoria")==0) printf("memoria [-blocks|-funcs|-vars|-all|-pmap]:\t Muestra detalles de memoria del proceso de la shell\n"
        "\t-blocks: Muestra los bloques de memoria asignados\n\t-funcs: Muestra las direcciones de memoria de las funciones\n\t-vars: Muestra las direcciones de las variables\n"
        "\t-all: Muestra todo lo anterior\n\t-pmap: ejecuta el comando pmap o similar sobre el proceso de la shell\n");
        else if(strcmp(trozos[1],"recursiva")==0) printf("recursiva [-n]:\tLlama n veces a la función recursiva\n");
        else printf("ayuda: %s no encontrado\n",trozos[1]);
    }
}

// *********
