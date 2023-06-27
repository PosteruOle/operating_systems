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

typedef struct{
  int id;
} osThreadFunctionArg; 


void *osThreadFunction(void *arg){
    
    osThreadFunctionArg *farg=arg;
    
    printf("Hello from %d. thread, my id is %ld!\n", farg->id, pthread_self());
    
    return NULL;
}

int main(int argc, char** argv){
   check_error(argc==2, "ARGC");
   
   int numopt=atoi(argv[1]);
   pthread_t *threads=malloc(numopt*sizeof(pthread_t));
   check_error(threads!=NULL, "Allocation failed!");
   
   osThreadFunctionArg *data=malloc(numopt*sizeof(osThreadFunctionArg));
   check_error(data!=NULL, "Allocation failed!");
   for(int i=0;i<numopt;i++){
      data[i].id=i;
      
      osPThreadCheck(pthread_create(&threads[i], NULL, osThreadFunction, &data[i]), "pthread_create");
      
   }
   
   for(int i=0;i<numopt;i++){
     
      osPThreadCheck(pthread_join(threads[i], NULL), "pthread_join");
   }

   free(threads);
   free(data);

 return 0;
}
