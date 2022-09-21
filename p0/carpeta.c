#include<unistd.h> 

void carpeta(char* trozos[],int ntrozos){


 char  str[100];


   if(ntrozos == 1){

    char name = getcwd(100);

      if (name != NULL) printf("%s\n", name);
      else printf("Error");

    }else if(ntrozos == 2){ 
  
    char str = trozos[1];
	
       chdir("str");

          if (chdir("str")!= 0) printf("Error");
       
       
 }

}