#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#define check_error(cond,msg)\
  do{\
     if(!(cond)){\
       perror(msg);\
       fprintf(stderr, "Greska!");\
     }\
  } while(0)\
  
  
int main(int arc, char **argv){

  pid_t child=fork();
  
  if(child==0){
    printf("Hello from child, my PID is %d\n", getpid());
    exit(EXIT_SUCCESS);
    
  } else {
    printf("Hello from parent, my PID is %d\n", getpid());
  }
  
  int wstatus;
  wait(&wstatus);
  printf("This line executes only parent!\n");


 return 0;
} 
