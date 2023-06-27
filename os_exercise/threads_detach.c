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
   
#define osThreadCheck(pthreadError, msg)\
   do{\
     int _pthreadError=pthreadError;\
     if(_pthreadError>0){\
        errno=_pthreadError;\
        check_error(false, msg);\
     }\
   }while(0)   
   
typedef struct{
  int idx;
} osThreadFunctionArg;   

void *osThreadFunction(void *arg){
    sleep(2);
    printf("Hello from detached thread!\n");
    printf("My thread id is %jd!\n", pthread_self());
    
    return NULL;
}   

int main(int argc, char** argv){
   
   pthread_t thread;
   osThreadCheck(pthread_create(&thread, NULL, osThreadFunction, NULL), "pthread_create_failed");
   
   osThreadCheck(pthread_detach(thread), "pthread_detach_failed");
   
   sleep(5);  
   
   //osThreadCheck(pthread_join(thread, NULL), "pthread_join_failed");


   exit(EXIT_SUCCESS);
}
