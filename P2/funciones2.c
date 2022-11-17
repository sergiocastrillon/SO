#include "funciones2.h"


// Variables globales de memoria
int mv1 = 1;
double mv2 = 2.7;
char mv3 = 'n';

// Imprime la lista (0 imprime todo, 1 malloc, 2 shared, 3 map)
void printList(int rev,tListM list){
   char time[40];
   tItemM alloc;

   pid_t nprocess;
   nprocess = getpid();
   

   if(rev == 0) // general
   printf("******Lista de bloques asignados para el proceso %d\n",nprocess);

   if(rev == 1) // malloc
   printf("******Lista de bloques asignados malloc para el proceso %d\n",nprocess);

   if(rev == 2) // shared
   printf("******Lista de bloques asignados shared para el proceso %d\n",nprocess);

   if(rev == 3) // shared
   printf("******Lista de bloques asignados mmap para el proceso %d\n",nprocess);

   tPosM pos = firstM(list);

   while(pos != NULL){

      alloc = pos->data;
      strftime(time, sizeof(time), "%b %d  %H:%M", localtime(&alloc.time));

      if(strcmp(alloc.type,"malloc") == 0 && (rev == 0 || rev == 1)){
         printf("%p\t %12ld %s malloc\n",alloc.direction,alloc.tam,time);
      }

      if(strcmp(alloc.type,"shared") == 0 && (rev == 0 || rev == 2)){
         printf("%p\t %12ld %s shared (key %d)\n",alloc.direction,alloc.tam,time,alloc.key);
      }

      if(strcmp(alloc.type,"mmap") == 0 && (rev == 0 || rev == 3)){
         printf("%p\t %12ld %s %s (descriptor %d)\n",alloc.direction,alloc.tam,time,alloc.filename,alloc.descriptor);
      }
      pos = nextM(pos,list);
   }
}
//


void Recursiva (int n)
{
  char automatico[TAMANO];
  static char estatico[TAMANO];

  printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

  if (n>0)
    Recursiva(n-1);
}


void LlenarMemoria (void *p, size_t cont, unsigned char byte){
  unsigned char *arr=(unsigned char *) p;
  size_t i;

  for (i=0; i<cont;i++) arr[i]=byte;
}



void * ObtenerMemoriaShmget (key_t clave, size_t tam,tListM list)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)  /*no nos vale*/
        {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
             shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
////// Guardar reserva en lista
   insertShared(p,s.shm_segsz,clave,list);
//////
    return (p);
}



void do_AllocateCreateshared (char *tr[],tListM list){
   key_t cl;
   size_t tam;
   void *p;

   if (tr[0]==NULL || tr[1]==NULL) {
      printList(2,list);
		return;
   }
  
   cl=(key_t)  strtoul(tr[0],NULL,10);
   tam=(size_t) strtoul(tr[1],NULL,10);
   if (tam==0) {
	printf ("No se asignan bloques de 0 bytes\n");
	return;
   }
   if ((p=ObtenerMemoriaShmget(cl,tam,list))!=NULL)
		printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
   else
		printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}



void * MapearFichero (char * fichero, int protection,tListM list){
   
   int df, map=MAP_PRIVATE,modo=O_RDONLY;
   struct stat s;
   void *p;

   if (protection&PROT_WRITE)
      modo=O_RDWR;
   if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
      return NULL;
   if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
      return NULL;

   
   insertMap(p,s.st_size,fichero,df,list);
   return p;
}

void do_AllocateMmap(char *arg[],tListM list)
{ 
     char *perm;
     void *p;
     int protection=0;
     
     if (arg[0]==NULL){
     printList(3,list);
     return;
     }
     if ((perm=arg[1])!=NULL && strlen(perm)<4) {
            if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
            if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
            if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
     }
     if ((p=MapearFichero(arg[0],protection,list))==NULL)
             perror ("Imposible mapear fichero");
     else
             printf ("fichero %s mapeado en %p\n", arg[0], p);
}

void do_DeallocateDelkey (char *args[])
{
   key_t clave;
   int id;
   char *key=args[0];

   if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return;
   }
   if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
   }
   if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}



ssize_t LeerFichero (char *f, void *p, size_t cont)
{
   struct stat s;
   ssize_t  n;  
   int df,aux;

   if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
	return -1;     
   if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
	cont=s.st_size;
   if ((n=read(df,p,cont))==-1){
	aux=errno;
	close(df);
	errno=aux;
	return -1;
   }
   close (df);
   return n;
}

//
void * cadtop(char direction[]){
   void * p;
   sscanf(direction, "%p", &p);
   return p;
}
//



void do_I_O_read (char *ar[])
{
   void *p;
   size_t cont=-1;
   ssize_t n;
   if (ar[0]==NULL || ar[1]==NULL){
	printf ("faltan parametros\n");
	return;
   }
   p=cadtop(ar[1]);  /*convertimos de cadena a puntero*/
   if (ar[2]!=NULL)
	cont=(size_t) atoll(ar[2]);

   if ((n=LeerFichero(ar[0],p,cont))==-1)
	perror ("Imposible leer fichero");
   else
	printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[0],p);
}

ssize_t EscribirFichero (char *f, void *p, size_t cont,int overwrite)
{
   ssize_t  n;
   int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;

   if (overwrite)
	flags=O_CREAT | O_WRONLY | O_TRUNC;

   if ((df=open(f,flags,0777))==-1)
	return -1;

   if ((n=write(df,p,cont))==-1){
	aux=errno;
	close(df);
	errno=aux;
	return -1;
   }
   close (df);
   return n;
}



void Do_pmap (void) /*sin argumentos*/
 { pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
   char elpid[32];
   char *argv[4]={"pmap",elpid,NULL};
   
   sprintf (elpid,"%d", (int) getpid());
   if ((pid=fork())==-1){
      perror ("Imposible crear proceso");
      return;
      }
   if (pid==0){
      if (execvp(argv[0],argv)==-1)
         perror("cannot execute pmap (linux, solaris)");
         
      argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;   
      if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
         perror("cannot execute procstat (FreeBSD)");
         
      argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;    
            if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
         perror("cannot execute procmap (OpenBSD)");
         
      argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
      if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
         perror("cannot execute vmmap (Mac-OS)");      
      exit(1);
  }
  waitpid (pid,NULL,0);
}







// Practica 2

void memfill(char * trozos[], int ntrozos){ // comprobar que funciona
   size_t tam;
   unsigned char a;
   void * e;

   if(ntrozos < 2) return;
   void * p = cadtop(trozos[1]);
   if(ntrozos < 3) tam=(size_t) 128;
   else tam=(size_t) strtoul(trozos[2],NULL,10);

   // Obtención del caracter
   if(ntrozos < 4) a = 65;
   else{
      if(strlen(trozos[3])==3){ // si longitud 3 es posible que nos hayan pasado un caracter
         e = strchr(trozos[3],39); // si detectamos ' entonces miramos con strrchr si
         if(e != NULL && e != strrchr(trozos[3],39)) a = trozos[3][1]; // hay otro ' de cierre
         else a = atoi(trozos[3]); // Si no lo hay 
      }else a = atoi(trozos[3]);
   }
   printf("Llenando %ld bytes de memoria con el byte %c(%x) a partir de la direccion %p\n",tam,a,a,p);
   LlenarMemoria(p,tam,a);
}


void memdump(char * trozos[], int ntrozos){
   int cont;
   void *p;


   if (ntrozos < 2) return;

   if(ntrozos < 3) cont = 25;
   else cont = atoi(trozos[2]);

   // obtenemos la direccion de trozos[1] y hacemos que p apunte a ella
   // para ello pasamos &p y le hacemos un cast a void ** que es lo que pide sscanf
   sscanf(trozos[1], "%p", (void**)&p);
   char * mem = p;


   for(int i = 0; i < cont; i += 25){ // 25 son los caracteres imprimidos por linea
      for(int j = i; (j < 25 + i) && (j < cont); j++){ // j empieza donde se quedó i
         // caracteres no imprimibles
         //if(mem[j] >= 32  && mem[j] < 170) 
         if(mem[j] >= 7 && mem[j] <= 13){
            switch (mem[j]) {
               case '\a':
                  printf("\\a ");
                  break;
               case '\b':
                  printf("\\b ");
                  break;
               case '\t':
                  printf("\\t ");
                  break;
               case '\n':
                  printf("\\n ");
                  break;
               case '\v':
                  printf("\\v ");
                  break;
               case '\f':
                  printf("\\f ");
                  break;
               case '\r':
                  printf("\\r ");
                  break;
            }
         }else{ // Tras comprobar que no es un carácter de control miramos si es printeable
            if(isprint(mem[j])) printf("%2c ",mem[j]);
            else printf("   ");
         } 
         
      }
      printf("\n");
      for (int j = i; (j < 25 + i) && (j < cont); j++){
         // Si no pasamos a unsigned char estaremos causando overflow
         // y se imprimirá un hexadecimal negativo
         printf("%02x ", (unsigned char) mem[j]); 
      }
      printf("\n");
   }
   printf("\n\n");
}



void allocate(char * trozos[], int ntrozos, tListM list){
    // Sin parámetros o sin opción imprime lista de memoria reservada
   if(ntrozos < 2){
      printList(0,list);
      return;
   } 
      // malloc
   if(strcmp(trozos[1],"-malloc")==0){
      if(ntrozos == 2){
         printList(1,list);
         return;
      } 
      // Llamada a malloc con la memoria a reservar
      size_t tam = strtol(trozos[2],NULL,10);
      if(tam <= 0){
         printf("No se reservan bloques de 0 bytes\n");
         return;
      } 
      void * direccion;
      if((direccion = malloc(tam)) == NULL){
         perror("No se pudo reservar memoria: ");
         return;
      } 

      // Guardamos la información en la lista
      insertMalloc(direccion,tam,list);

      printf("Asignados %ld bytes en %p\n", tam,direccion);
      return;
   }

   if(strcmp(trozos[1],"-createshared")==0){
      char* datos[2];
      datos[0] = trozos[2];
      datos[1] = trozos[3];
      //strncpy(datos[0],trozos[2],10);
      //strncpy(datos[1],trozos[3],10);
      do_AllocateCreateshared(datos,list);
   }

   if(strcmp(trozos[1],"-shared")==0){
      key_t cl;
      size_t tam;
      void *p;
      if(ntrozos < 3){
         printList(2,list);
         return;
      }

      cl=(key_t) strtoul(trozos[2],NULL,10);
      tam=(size_t) 0;
      if((p=ObtenerMemoriaShmget(cl,tam,list))!=NULL)
		printf("Memoria compartida de clave %d en %p\n",cl, p);
      else
		printf ("Imposible asignar memoria compartida clave %d:%s\n",cl,strerror(errno));
   }


   if(strcmp(trozos[1],"-mmap")==0){
      // Si el usuario no pasa los permisos, los permisos están todos a 0 con lo cual
      // intentar acceder a la direccion de alguna manera producira un segfault
      char* datos[2];
      datos[0] = trozos[2];
      datos[1] = trozos[3];

      do_AllocateMmap(datos,list);
   }
}


void deallocate_aux(tPosM pos,tListM list){
   tItemM item = pos->data;
   if(strcmp("malloc",item.type)==0){
      void * p = item.direction;
      free(p);
      removeItem(pos,list);
      return;
   }


   if(strcmp("shared",item.type)==0){
      if(shmdt(item.direction) == -1) perror("Imposible hacer shmdt:");
      else removeItem(pos,list);
      return;
   }


   if(strcmp("mmap",item.type)==0){
      if(munmap(item.direction,item.tam) == -1) perror("Imposible hacer mummap:");
      else removeItem(pos,list);
      return;
   }  
}



void deallocate(char * trozos[], int ntrozos, tListM list){
   if(ntrozos < 2){
      printList(0,list);
      return;
   } 
      // malloc
   if(strcmp(trozos[1],"-malloc")==0){
      if(ntrozos == 2){
         printList(1,list);
         return;
      }
      size_t tam = atol(trozos[2]); // Devuelve 0 en caso de fallo, 
      //como no hay bloques de tam 0 no supone un problema
      if(tam <= 0){
         printf("No se asignan bloques de 0 bytes o tamaños negativos\n");
         return;
      }
      tPosM i;
      if((i = findMallocTam(tam,list))==NULL){
         printf("No hay bloques asignados con malloc de ese tamaño\n");
         return;
      }
      deallocate_aux(i,list);
      return;
   }

   if(strcmp(trozos[1],"-delkey")==0){
      char *args[1];
      args[0] = trozos[2];
      do_DeallocateDelkey(args);
   }

   if(strcmp(trozos[1],"-shared")==0){
      if(ntrozos < 3){
         printList(2,list);
         return;
      }
      key_t key = atoi(trozos[2]);
      if(key <= 0){
         printf("No se pueden desasignar claves menores a 1\n");
         return;
      }
      tPosM i;
      if((i = findSharedKey(key,list)) == NULL){
         printf("No hay ningún bloque con la clave %d\n",key);
         return;
      }

      deallocate_aux(i,list);
      return;
   }


   if(strcmp(trozos[1],"-mmap")==0){
      if(ntrozos < 3){
         printList(3,list);
         return;
      }
      tPosM i;
      if((i = findMappedFile(trozos[2],list)) == NULL){
         printf("Fichero %s no mapeado\n",trozos[2]);
         return;
      }
      deallocate_aux(i,list);
      return;
   }

   
   // Si no es ninguna de las anteriores entonces entendemos que nos están pasando una direccion

   void * p = cadtop(trozos[1]);
   tPosM i = findDirection(p,list);
   if(i == NULL){ 
      printf("La direccion %p no ha sido asignada\n",p);
      return;
   }
   deallocate_aux(i,list);
}


void io(char * trozos[], int ntrozos){
   if(ntrozos<2) return;
   if(strcmp(trozos[1],"read")==0){
      char* arg[4];
      arg[0] = trozos[2];
      arg[1] = trozos[3];
      arg[2] = trozos[4];
      do_I_O_read(arg);
   }

   if(strcmp(trozos[1],"write")==0){
      if(ntrozos<5){
         printf("faltan parametros\n");
         return;
      }
      ssize_t  n;
      int desp = 0;
      if(strcmp(trozos[2],"-o")==0) desp = 1;

      

      if (trozos[2+desp]==NULL || trozos[3+desp]==NULL || trozos[4+desp]==NULL){
         printf("faltan parametros\n");
         return;
      }
      void * p=cadtop(trozos[3+desp]);  /*convertimos de cadena a puntero*/
      size_t cont=(size_t) atoll(trozos[4+desp]);

      if ((n=EscribirFichero(trozos[2+desp],p,cont,desp))==-1)
      perror ("Imposible leer fichero");
      else
      printf ("escritos %lld bytes en %s desde %p\n",(long long) n,trozos[2+desp],p);
   }
}


void recursiva(char * trozos[], int ntrozos){
   if(ntrozos<2) return;
   int a;
   if((a = atoi(trozos[1]))>=0) Recursiva(a);
}

void memoria(char * trozos[], int ntrozos,tListM list){
   int mv4 = 9;
   double mv5 = 2.7;
   char mv6 = 'n';

   static int mv7 = 1;
   static double mv8 = 2.7;
   static char mv9 = 'n';

   int p;
   if(ntrozos < 2) p = 0;
   else{
      if(strcmp(trozos[1],"-blocks")==0) p = 1;
      if(strcmp(trozos[1],"-funcs")==0) p = 2;
      if(strcmp(trozos[1],"-vars")==0) p = 3;
      if(strcmp(trozos[1],"-all")==0) p = 0;
      if(strcmp(trozos[1],"-pmap")==0){
         Do_pmap();
         return;
      }
   }

   if((p == 0) || (p == 3)){
      printf("Variables locales\t %14p, %14p, %14p\n", &mv4,&mv5,&mv6);
      printf("Variables globales\t %14p, %14p, %14p\n", &mv1,&mv2,&mv3);
      printf("Variables estaticas\t %14p, %14p, %14p\n", &mv7,&mv8,&mv9);
   }

   if(p == 0 || p == 2){
      printf("Funciones programa\t %14p, %14p, %14p\n", memoria,recursiva,printList);
      printf("Funciones libreria\t %14p, %14p, %14p\n", printf,exit,system);
   }

   if(p == 0 || p == 1){
      printf("\n\n");
      printList(0,list);
   }
}