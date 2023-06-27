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
/*
Program demonstrira kreiranje novih procesa. Nakon kreiranja,
roditelj treba da ispise svoj PID i PID svog deteta, a dete
svoj PID i PID svog roditelja. Nakon toga, oba procesa ispisuju istu poruku.
*/ 

int main(int argc, char** argv){
   
   pid_t child=fork();
   check_error(child!=-1,"fork_failed");
   
   if(child==0){ // grana deteta
     printf("Pozdrav od dete procesa, moj PID je %d, PID mog roditelja je %d!\n\n", getpid(), getppid());
   } else if(child>0){ // roditeljeva grana
     printf("Pozdrav od roditelja, moj PID je %d, PID mog deteta je %d!\n\n", getpid(), child);
   }
   
   printf("I roditelj i dete ispisuju ovo!!!\n");   

 return 0;
}

