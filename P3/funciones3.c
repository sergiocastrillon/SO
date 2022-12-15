#include "funciones3.h"

extern char **environ;


//las siguientes funciones nos permiten obtener el nombre de una senal a partir
//del número y viceversa
static struct SEN sigstrnum[200]={
	{"HUP", SIGHUP},
	{"INT", SIGINT},
	{"QUIT", SIGQUIT},
	{"ILL", SIGILL},
	{"TRAP", SIGTRAP},
	{"ABRT", SIGABRT},
	{"IOT", SIGIOT},
	{"BUS", SIGBUS},
	{"FPE", SIGFPE},
	{"KILL", SIGKILL},
	{"USR1", SIGUSR1},
	{"SEGV", SIGSEGV},
	{"USR2", SIGUSR2},
	{"PIPE", SIGPIPE},
	{"ALRM", SIGALRM},
	{"TERM", SIGTERM},
	{"CHLD", SIGCHLD},
	{"CONT", SIGCONT},
	{"STOP", SIGSTOP},
	{"TSTP", SIGTSTP},
	{"TTIN", SIGTTIN},
	{"TTOU", SIGTTOU},
	{"URG", SIGURG},
	{"XCPU", SIGXCPU},
	{"XFSZ", SIGXFSZ},
	{"VTALRM", SIGVTALRM},
	{"PROF", SIGPROF},
	{"WINCH", SIGWINCH},
	{"IO", SIGIO},
	{"SYS", SIGSYS},
//senales que no hay en todas partes
#ifdef SIGPOLL
	{"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
	{"PWR", SIGPWR},
#endif
#ifdef SIGEMT
	{"EMT", SIGEMT},
#endif
#ifdef SIGINFO
	{"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
	{"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
	{"CLD", SIGCLD},
#endif
#ifdef SIGLOST
	{"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
	{"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
	{"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
	{"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
	{"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
	{"WAITING", SIGWAITING},
#endif
	{NULL,-1},
	};    //fin array sigstrnum




char *NombreSenal(int sen)  /*devuelve el nombre senal a partir de la senal */
{			 //para sitios donde no hay sig2str
 int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
	if (sen==sigstrnum[i].senal)
		return sigstrnum[i].nombre;
 return ("SIGUNKNOWN");
}

// Imprime la información de un tItemP
void printItem(tItemP item){
	char time[40];
	strftime(time, sizeof(time), "%Y/%m/%d  %X", localtime(&item.time));
		// Señal, si se reconoce la señal se muestra como string, si no como número
		char* a; 
		a = NombreSenal(item.signal);
		// Imprime el nombre de la señal solo cuando señalado
		if(item.status != SENALADO){
			printf("%d\t p=%d %s %s (%03d) %s\n",item.pid,item.priority,time,
			statusToString(item.status),item.signal,item.command);
		}else{
			printf("%d\t p=%d %s %s (%s) %s\n",item.pid,item.priority,time,
			statusToString(item.status),a,item.command);
		}
}


// Imprime la lista de procesos
void printListP(tListP listp){
	tItemP item;
	for(tPosP i = firstP(listp); i != NULL; i = nextP(i,listp)){
		item = getItemP(i,listp);
		printItem(item);
	}
}





//void Cmd_fork(char *tr[])
void Cmd_fork(tListP listp){
	pid_t pid;

	if ((pid = fork()) == 0)
	{
		cleanListP(listp);
		printf("ejecutando proceso %d\n", getpid());
	}
	else if (pid != -1)
		waitpid(pid, NULL, 0);
}

int BuscarVariable(char const *var, char *e[]) /*busca una variable en el entorno que se le pasa como parámetro*/
{
	int pos = 0;
	char aux[MAXVAR];

	strcpy(aux, var);
	strcat(aux, "=");

	while (e[pos] != NULL)
		if (!strncmp(e[pos], aux, strlen(aux)))
			return (pos);
		else
			pos++;
	errno = ENOENT; /*no hay tal variable*/
	return (-1);
}

int CambiarVariable(char *var, char *valor, char *e[]) /*cambia una variable en el entorno que se le pasa como parámetro*/
{													   /*lo hace directamente, no usa putenv*/
	int pos;
	char *aux;

	if ((pos = BuscarVariable(var, e)) == -1)
		return (-1);

	if ((aux = (char *)malloc(strlen(var) + strlen(valor) + 2)) == NULL)
		return -1;
	strcpy(aux, var);
	strcat(aux, "=");
	strcat(aux, valor);
	e[pos] = aux;
	return (pos);
}

/*para sistemas donde no hay (o no queremos usuar) execvpe*/
char *Ejecutable(char *s)
{
	char path[MAXNAME];
	static char aux2[MAXNAME];
	struct stat st;
	char *p;
	if (s == NULL || (p = getenv("PATH")) == NULL)
		return s;
	if (s[0] == '/' || !strncmp(s, "./", 2) || !strncmp(s, "../", 3))
		return s; /*is an absolute pathname*/
	strncpy(path, p, MAXNAME);
	for (p = strtok(path, ":"); p != NULL; p = strtok(NULL, ":"))
	{
		sprintf(aux2, "%s/%s", p, s);
		if (lstat(aux2, &st) != -1)
			return aux2;
	}
	return s;
}

int OurExecvpe(char *file, char *argv[MAXVAR], char *envp[MAXVAR])
{
	return (execve(Ejecutable(file), argv, envp));
}







void execute(char *trozos[], int ntrozos,bool exec){
	int i, ex,z;
	int a;
	bool vars = true;


	char *aux2[MAXVAR];
	char *aux[MAXVAR];

	z = 0;
	if(exec){
		i = 1;
	}else i = 0;
	
	ex = i;
	while (i < ntrozos){ // Buscamos variables de entorno
		if ((a = BuscarVariable(trozos[i], environ)) == -1) break;
		aux[z] = strdup(environ[a]);
		i++;
		z++;
	}
	aux[z] = NULL;

	// Si no encontramos ninguna variable entonces pasamos todo el entorno
	if (i == ex) vars = false;

	ex = i; // guardamos la posicion donde se encuentra ejecutable

	z = 0;
	while (i < ntrozos){ // Buscamos argumentos opcionales (ejecutable siempre se copia)
		if (trozos[i][0] == '@' || trozos[i][0] == '&') break;
		aux2[z] = strndup(trozos[i], MAXVAR);
		i++;
		z++;
	}
	aux2[z] = NULL;

	// Buscamos si nos han pasado la prioridad
	if(i < ntrozos && trozos[i][0] == '@'){
		int x = 1;
		char a[4];
		while(trozos[i][x-1] != '\0'){
			a[x-1] = trozos[i][x];
			x++;
		}
		
		int pri = atoi(a);
		if (setpriority(PRIO_PROCESS, getpid(), pri) == -1){
			printf("Error al intentar ejecutar con prioridad %d: %s\n",pri, strerror(errno));
			if(!exec) exit(0); // Si se creo el proceso con fork entonces acaba
			return;
		}
		
	}
	int err = 0;
	if (vars)
		err = OurExecvpe(trozos[ex], aux2, aux);
	else
		err = OurExecvpe(trozos[ex], aux2, environ);

	if(err == -1){
		printf("No ejecutado: %s\n",strerror(errno));
		if(!exec) exit(0); // si exec false entonces acabamos el proceso creado con fork
	}
}



void newProcessExec(char * trozos[], int ntrozos,tListP listp){
	int pid;
	if(trozos[ntrozos-1][0] == '&'){ // Background
		if ((pid = fork()) == 0){ // Proceso hijo ejecuta programa
			execute(trozos,ntrozos,false);
		}else if(pid != -1){ // Proceso padre (si no hay error) añade a lista
			// Espera un poco por el hijo por si devuelve un error y lo tiene que mostrar
			sleep(1); 


			char a[100];
			strcpy(a,trozos[0]);

			for(int i = 1; i < ntrozos; i++){ // Concatenar para añadir a la lista
				if(trozos[i][0] == '&') break;
				strcat(a," ");
				strcat(a,trozos[i]);
			}
			insertProcess(pid,a,listp);
		}
	}else{ // Foreground
		if ((pid = fork()) == 0) execute(trozos,ntrozos,false); // Proceso hijo ejecuta
		else if (pid != -1) waitpid(pid, NULL, 0); // Proceso padre espera fin de ejecucion
	}
}


void priority(char *trozos[], int ntrozos){
	int pid;
	if (ntrozos < 3){
		if (ntrozos == 1) pid = getpid();

		if (ntrozos == 2) pid = atoi(trozos[1]);

		errno = -1;
		int prio = getpriority(PRIO_PROCESS, pid);
		if (errno == -1)
			printf("La prioridad del proceso %d es: %d\n", pid, prio);
		else
			printf("Imposible obtener la prioridad del proceso %d: %s\n", pid, strerror(errno));
		return;
	}

	pid = atoi(trozos[1]);
	// Si pid 0 y trozos[1][0] no es 0 entonces es un error de atoi, cambiar pid a -1 para evitar cambios de prioridad no requeridos
	if (pid == 0 && trozos[1][0] != '0')
		pid = -1;

	int prio = atoi(trozos[2]);
	if (setpriority(PRIO_PROCESS, pid, prio) != -1)
		printf("La prioridad del proceso %d"
		" fue cambiada a %d\n",pid, prio);
	else
		printf("Error al intentar cambiar la prioridad del proceso %d: %s\n", pid, strerror(errno));
}

void showvar(char *trozos[], int ntrozos, char *arg3[]){
	if (ntrozos < 2) return;
	
	int a;
	// BuscarVariable hace un set de errno si no se encuentra la variable

	// Arg3
	if ((a = BuscarVariable(trozos[1], arg3)) != -1)
		printf("Con arg3 main %s(%p) @%p\n", arg3[a],
		arg3[a], &arg3[a]);
	else
		printf("Error buscando la variable %s: %s\n", trozos[1], strerror(errno));

	// environ
	if ((a = BuscarVariable(trozos[1], environ)) != -1)
		printf("  Con environ %s(%p) @%p\n", environ[a],
		environ[a], &environ[a]);
	else
		printf("Error buscando la variable %s: %s\n", trozos[1], strerror(errno));

	// getenv
	char *b;
	if ((b = getenv(trozos[1])) != NULL)
		printf("   Con getenv %s(%p)\n", b, b);
	else
		printf("Error buscando la variable\n");

	// Anotación importante, las direcciones entre paréntesis indican donde se
	// encuentra el primer carácter que se imprime, eso quiere decir que las direcciones de arg3
	// y de environ deben coincidir, sin embargo con getenv no se tiene en cuenta el nombre
	// de la variable ni el '=' por lo que aparecerá desplazado.
	// Ej: Si HOME=/user/sergio está en la 0x0 con getenv sería /user/sergio y estaría en 0x5
}

void changevar(char *trozos[], int ntrozos, char *arg3[]){
	if (ntrozos < 4)
		return;
	int aux;

	if (strcmp(trozos[1], "-a") == 0) // arg3
		aux = CambiarVariable(trozos[2], trozos[3], arg3);
	if (strcmp(trozos[1], "-e") == 0) // enviro
		aux = CambiarVariable(trozos[2], trozos[3], environ);
	if (strcmp(trozos[1], "-p") == 0){ //putenv
		char *a;
		if ((a = (char *)malloc(strlen(trozos[2]) + strlen(trozos[3]) + 2)) == NULL)
			return;

		strcpy(a, trozos[2]);
		strcat(a, "=");
		strcat(a, trozos[3]);
		strcat(a, "\0");
		aux = putenv(a);
		if (aux != 0)
			aux = -1;
		free(a);
	}

	if (aux == -1)
		printf("Imposible cambiar variable %s: %s\n", trozos[2], strerror(errno));
}

// Mostrar todas las variables de entorno
void showenv(char *trozos[], int ntrozos, char *arg3[]){
	int i = 0;
	if (ntrozos == 1){
		while (arg3[i] != NULL){
			printf("%p->main arg3[%d]=(%p) %s\n", &arg3[i],
			i, arg3[i], arg3[i]);
			i++;
		}
		return;
	}

	if (strcmp(trozos[1], "-environ") == 0){
		while (environ[i] != NULL){
			printf("%p->environ[%d]=(%p) %s\n", &environ[i],
			i, environ[i], environ[i]);
			i++;
		}
		return;
	}

	if (strcmp(trozos[1], "-addr") == 0){
		printf("environ: %p (almacenado en %p)\n", environ, &environ);
		printf("main arg3: %p (almacenado en %p)\n", arg3, &arg3);
	}
}


tItemP actualizarProceso(tItemP p, int est){
	if(WIFEXITED(est)){
		p.status = TERMINADO;
		p.signal = WEXITSTATUS(est);
	}else if(WIFSIGNALED(est)){
		p.status = SENALADO;
		p.signal = WTERMSIG(est);
	}else if(WIFSTOPPED(est)){
		p.status = PARADO;
		p.signal = WSTOPSIG(est);
	}else if(WIFCONTINUED(est)){
		p.status = ACTIVO;
		p.signal = 0;
	}

	return p;
}
// Actualiza la lista de procesos en busca de cambios de estado (o de prioridad)
void actualizarProcesos(tListP list){
	for(tPosP i = firstP(list); i != NULL; i = nextP(i,list)){
		tItemP p = getItemP(i,list);

		p.priority = getpriority(PRIO_PROCESS, p.pid);
		int est;

		

		if(waitpid(p.pid,&est,WNOHANG|WUNTRACED|WCONTINUED)==p.pid){
			p = actualizarProceso(p,est);
		}
		updateItem(p,i,list);
	}
}

// Muestra la lista de procesos
void listjobs(tListP list){
	actualizarProcesos(list);
	printListP(list);
}

// Borra, según el argumento pasado, procesos de la lista
void deljobs(char * trozos[], int ntrozos,tListP list){
	if(ntrozos < 2) listjobs(list);
	bool term = false;
	bool sig = false;
	if(strcmp(trozos[1],"-term")==0) term = true;
	if(strcmp(trozos[1],"-sig")==0) sig = true;


	tPosP i = firstP(list);
	tPosP x = NULL;
	tItemP item;
	while(i!=NULL){
		item = getItemP(i,list);
		x = nextP(i,list);
		
		if(term && (item.status == TERMINADO)){
			removeItemP(i,list);
		} 
		else if(sig && (item.status == SENALADO)){
			removeItemP(i,list);
		} 

		i = x; // Solo si no se borra ningun elemento
	}
	printListP(list);
}


void job(char * trozos[], int ntrozos,tListP list){
	if(ntrozos < 2) return;

	actualizarProcesos(list);
	if(ntrozos < 3){
		int pid = atoi(trozos[1]);
		printf("ok");
		tPosP p = findPid(pid,list);
		if(p == NULL) return;
		tItemP i = getItemP(p,list);
		printItem(i);

	}else if(strcmp(trozos[1],"-fg")==0){

		int pid = atoi(trozos[2]);
		tPosP p = findPid(pid,list);

		if(p == NULL) return;
		tItemP i = getItemP(p,list);

		if(i.status == TERMINADO){
			printf("El proceso %d ya esta terminado\n",pid);
			return;
		} 
		int est;
		removeItemP(p,list);

		waitpid(pid, &est, 0);
		i = actualizarProceso(i,est);
		if(i.status == TERMINADO) printf("Proceso %d terminado normalmente. Valor devuelto %d\n",pid,i.signal);
		if(i.status == SENALADO) printf("Proceso %d terminado por señal %s\n",pid,NombreSenal(i.signal));
	}
}