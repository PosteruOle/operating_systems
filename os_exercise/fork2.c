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

#include <sys/wait.h>

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
 
/*
Program demonstrira kreiranje novih procesa. Nakon kreiranja,
roditelj treba da ispise svoj PID i PID svog deteta, a dete
svoj PID i PID svog roditelja.
Dodatno, dete poziva exit da bi zavrsilo izvrsavanje, dok roditelj ceka 
da dete zavrsi svoj rad.
*/

int main(int argc, char** argv){
   
   
   pid_t child=fork();
   check_error(child!=-1, "fork_filed");
   
   float prosek=9.24;
   
   if(child>0){ //roditelj
     
     printf("Pozdrav od roditelja, moj pid %d i pid mog deteta %d!\n", getpid(), child);
     //check_error(wait(NULL)!=-1, "wait_failed");
     
   } else if(child==0){ //dete
     prosek=9.65;
     sleep(2);
     printf("Pre pozdrava, prosek iznosi %.2f!\n", prosek);
     printf("Pozdrav od deteta, moj pid %d i pid mog roditelja %d!\n", getpid(), getppid());
     exit(EXIT_SUCCESS);
   
   }
   
   int status;
   //check_error(waitpid(child, &status, 0)!=-1, "waitpid_failed");
   check_error(wait(NULL)!=-1, "wait_failed");
   printf("Sada kada nema deteta prosek iznosi %.2f :(\n", prosek);
   printf("Roditelj je sacekao da dete proces zavrsi svoje izvrsavanje!\n");




 return 0;
}

