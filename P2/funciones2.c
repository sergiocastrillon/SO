#include "funciones2.h"

// Funciones de lista
void printCabeceraList(int rev){
   pid_t nprocess;
   nprocess = getpid();
   if(rev == 0) // general
   printf("******Lista de bloques asignados para el proceso %d\n",nprocess);
   if(rev == 1) // malloc
   printf("******Lista de bloques asignados malloc para el proceso %d\n",nprocess);
   if(rev == 2) // shared
   printf("******Lista de bloques asignados shared para el proceso %d\n",nprocess);
}

void printList(int rev,tListM list){
   char time[40];
   tItemM alloc;
   tPosM pos = firstM(list);

   while(pos != NULL){
      alloc = pos->data;
      strftime(time, sizeof(time), "%b %d  %H:%M", localtime(&alloc.time));
      if(strcmp(alloc.type,"malloc") == 0 && (rev == 0 || rev == 1)){
         printf("%p\t %8d %s\n",alloc.direction,alloc.tam,time);
      }

      if(strcmp(alloc.type,"shared") == 0 && (rev == 0 || rev == 2)){
         printf("%p\t %8d %s shared (key %d)\n",alloc.direction,alloc.tam,time,alloc.key);
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
   tItemM alloc;
   alloc.time = time(NULL);
   strcpy(alloc.type, "shared");
   alloc.tam = s.shm_segsz;
   alloc.direction = p;
   alloc.key = clave;
   strcpy(alloc.filename,"\0");
   insertItem(alloc,list);
//////
 /* Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    return (p);
}
void do_AllocateCreateshared (char *tr[],tListM list){
   key_t cl;
   size_t tam;
   void *p;

   if (tr[0]==NULL || tr[1]==NULL) {
      printf("Sin implementar printlistshared\n");
		//ImprimirListaShared(&L); // Implementar en la lista y pasar la lista como parámetro de la función
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


void * MapearFichero (char * fichero, int protection)
{
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
          modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
          return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
           return NULL;
/* Guardar en la lista    InsertarNodoMmap (&L,p, s.st_size,df,fichero); */
    return p;
}

void do_AllocateMmap(char *arg[])
{ 
     char *perm;
     void *p;
     int protection=0;
     
     if (arg[0]==NULL){
     printf("Sin implementar imprimirlista mmap\n");
     // ImprimirListaMmap(&L); 
     return;
     }
     if ((perm=arg[1])!=NULL && strlen(perm)<4) {
            if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
            if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
            if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
     }
     if ((p=MapearFichero(arg[0],protection))==NULL)
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


// Esto no sería para la siguiente practica ??
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
   if(ntrozos < 4) a = 69;
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
      for(int j = i; j < 25 + i; j++){ // j empieza donde se quedó i
         // caracteres no imprimibles
         if(mem[j] > 32  && mem[j] < 170) printf("%3c ", mem[j]);
      }
      printf("\n");
      for (int j = i; j < 25 + i; j++){
         printf("%3x ", mem[j]);
         //printf("   ");
      }
      printf("\n");
   }
   printf("\n\n");
}



void allocate(char * trozos[], int ntrozos, tListM list){
    // Sin parámetros o sin opción imprime lista de memoria reservada
   if(ntrozos < 2){
      printCabeceraList(0);
      printList(0,list);
      return;
   } 
      // malloc
   if(strcmp(trozos[1],"-malloc")==0){
      if(ntrozos == 2){
         printf("Llamada a lista de malloc\n");
         return;
      } 
      // Llamada a malloc con la memoria a reservar
      tItemM allo;
      int tam = atoi(trozos[2]);
      if(tam <= 0){
         printf("No se reservan bloques de 0 bytes\n");
         return;
      } 
      void * direccion;
      if((direccion = malloc(tam)) == NULL) perror("No se pudo reservar memoria: ");
      // Guardamos la información en la lista
      allo.time = time(NULL);
      strcpy(allo.type, "malloc");
      allo.tam = tam;
      allo.direction = direccion;
      // Parametros no usados a "null" para detectar posibles fallos
      allo.key = -1;
      strcpy(allo.filename,"\0");
      if(!insertItem(allo,list)) printf("No se pudo guardar la reserva en la lista\n");

      printf("Asignados %d bytes en %p\n", tam,direccion);
      return;
   }

   if(strcmp(trozos[1],"-createshared")==0){
      char* datos[10];
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
      cl=(key_t)  strtoul(trozos[2],NULL,10);
      tam=(size_t) 0;
      if((p=ObtenerMemoriaShmget(cl,tam,list))!=NULL) // Acordarse de borrar Clave
		printf ("Memoria compartida de clave %ld en %p\n",(unsigned long) cl, p);
      else
		printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
      //ObtenerMemoriaShmget (cl,tam);
      
   }



   void deallocate(char * trozos[], int ntrozos, tListM list){
      if(ntrozos < 2){
      printf("Llamada a lista de memoria\n");
      return;
   } 
      // malloc
   if(strcmp(trozos[1],"-malloc")==0){
      if(ntrozos == 2){
         printf("Llamada a lista de malloc\n");
         return;
      }else{

      }
   }
   }
}
