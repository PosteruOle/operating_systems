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
Program implementira nacin za komunikaciju izmedju roditelja i deteta.
Roditelj ucitava liniju sa standardnog ulaza, koju zatim
prosledjuje detetu putem pajpa.
Dete cita tu liniju i ispisuje je na standardni izlaz.
*/
#define READ_END (0)
#define WRITE_END (1)

int main(int argc, char** argv){
   
   int pipefds[2];
   check_error(pipe(pipefds)!=-1, "pipe_failed");
   
   pid_t child=fork();
   check_error(child!=-1, "fork_failed");
   
   
   
   if(child>0){
     close(pipefds[READ_END]);
     char *line=NULL;
     size_t size;
     getline(&line, &size, stdin);
     
     //printf("Uneta je linija: %s\n", line);
     //int readBytes;
     
     check_error(write(pipefds[WRITE_END], line, size)!=-1, "write_failed");
     
     close(pipefds[WRITE_END]);
   
   } else if(child==0){
     close(pipefds[WRITE_END]);
     
     char *membuff=malloc(1024*sizeof(char));
     check_error(membuff!=NULL, "allocation_failed");
     int readBytes;
     
     readBytes=read(pipefds[READ_END], membuff, 1024);
     check_error(readBytes!=-1, "read_failed");
     membuff[readBytes]='\0';
     
     printf("-----------------------------------\n");
     printf("Poruka od roditelja je sledeca: \n");
     printf("\t%s\n", membuff);
     printf("-----------------------------------\n");
     free(membuff);
     
     close(pipefds[READ_END]);
     exit(EXIT_SUCCESS);
   }
   
  check_error(wait(NULL)!=-1, "wait_failed");
  printf("Valjda je sve okej proslo...rece roditelj i zakaslja se\n");

 return 0;
}

