#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <pthread.h>
#include <errno.h>

#define check_error(cond,msg)\
   do{\
     if(!(cond)){\
       perror(msg);\
       fprintf(stderr,"File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
       exit(EXIT_FAILURE);\
     }\
   }while(0)

// makro za obradu gresaka pristiglih od niti   
#define osPThreadCheck(pthreadError, msg)\
   do{\
     int _pthreadError=pthreadError;\
     if(_pthreadError>0){\
        errno=_pthreadError;\
        check_error(false, msg);\
     }\
   }while(0)   

extern char **environ; 

void printenv(){
    char **env;
    for(env=environ; *env!=NULL; env++){
       printf("\t%s\n", *env);
    }
}
int main(int argc, char** argv){
   
   check_error(argc==2, "argc_failed");
   char *found=getenv(argv[1]);
   if(found!=NULL)
     printf("Pronasli smo promenljivu okruzenja cije je ime %s! Njena vrednost je: %s\n",argv[1], found);
   else
     printf("Ne postoji promenljiva okruzenja sa prosledjenim imenom!\n");  
   
   //printenv();
   


 return 0;
}
