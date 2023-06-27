/* 
exec je sistemski poziv koji omogucava izvrsavanje drugih programa. 
Fork vrsi kopiranje adresnog prostora, exec vrsi zamenu adresnog prostora
Nakon exec-a sadrzaj adresnog prostora se prepisuje novim procesom.
*/ 
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
 

int main(int argc, char** argv){
   
   fprintf(stderr,"Ovo se hoce ispisati!\n");
   
   //check_error(execlp("ls", "ls", "-al", NULL)!=-1, "execlp_failed");
   check_error(execlp("man", "man", "3", "shm_open", NULL)!=-1,"execlp_failed");
   
   fprintf(stderr, "Ovo se nece ispisati!\n");

 return 0;
}

