#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <pthread.h>

#define check_error(cond,msg)\
   do{\
     if(!(cond)){\
       perror(msg);\
       fprintf(stderr,"File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
       exit(EXIT_SUCCESS);\
     }\
   }while(0)

// makro za obradu gresaka pristiglih od niti
#define osThreadCheck(pthreaderror, msg)\
   do{\
     int _pthreaderror=pthreaderror;\
     if(_pthreaderror>0){\
       errno=_pthreaderror;\
       check_error(false,msg);\
     }\
   }while(0)

typedef struct{
  int id;
} osThreadFunctionArg;

void *osThreadFunction(void *arg){
    osThreadFunctionArg *farg=arg;
    printf("Hello from %d. thread, my TID is %ld!\n", farg->id, pthread_self());
    
    return NULL;
}

int main(int argc, char** argv){
   check_error(argc==2, "argc");
   int numopt=atoi(argv[1]);
   pthread_t *threads=malloc(numopt*sizeof(pthread_t));
   check_error(threads!=NULL, "Allocation failed!");
   
   osThreadFunctionArg *data=malloc(numopt*sizeof(osThreadFunctionArg));
   check_error(data!=NULL, "Allocation failed!");
   
   for(int i=0;i<numopt;i++){
      data[i].id=i;
      
      check_error(pthread_create(&threads[i], NULL, osThreadFunction, &data[i])==0, "pthread_create_failed");
   }
   
   for(int i=0;i<numopt;i++){
      check_error(pthread_join(threads[i], NULL)==0, "pthread_join_failed");
   }
   
   free(threads);
   free(data);  

   exit(EXIT_SUCCESS);
}
