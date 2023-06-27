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
#define osThreadCheck(pthreadError, msg)\
   do{\
     int _pthreadError=pthreadError;\
     if(_pthreadError>0){\
        errno=_pthreadError;\
        check_error(false, msg);\
     }\
   }while(0)   
 
/*
Poziv programa:
./sync_mutex file0 file1 file2 ...

Kao argumenti komandne linije zadaju se
putanje do fajlova koji sadrze cele brojeve.
Potrebno je ispisati ukupnu sumu svih brojeva
u tim fajlovima. Obezbediti da svaki fajl obradjuje 
tacno jedna nit, kao i zastitu kriticne sekcije.
*/
typedef struct{
    int data;
    pthread_mutex_t lock;
} osSafeInt;

osSafeInt globalSum;

void *osThreadFunction(void *arg){

    char *fpath=arg;
    FILE *f=fopen(fpath,"r");
    int broj;
    int localsum=0;
    while(fscanf(f,"%d", &broj)==1){
       localsum+=broj;
    }
    
    fprintf(stderr,"Nit ciji je TID %ld je pronasla ovoliku lokalnu sumu %d!\n", pthread_self(), localsum);
    
    osThreadCheck(pthread_mutex_lock(&globalSum.lock), "pthread_mutex_lock_failed");
    globalSum.data+=localsum;
    osThreadCheck(pthread_mutex_unlock(&globalSum.lock), "pthread_mutex_unlock_failed");
    
    return NULL;
}

int main(int argc, char** argv){
    check_error(argc>=2, "argc_failed");
    
    int numopt=argc-1;
    pthread_t *threads=malloc(numopt*sizeof(pthread_t));
    check_error(threads!=NULL, "Allocation failed!");
    
    globalSum.data=0;
    osThreadCheck(pthread_mutex_init(&globalSum.lock, NULL), "pthread_mutex_init_failed");
    
    for(int i=0;i<numopt;i++){
       osThreadCheck(pthread_create(&threads[i], NULL, osThreadFunction, argv[i+1]), "pthread_create_failed");
    } 
    
    for(int i=0;i<numopt;i++){
       osThreadCheck(pthread_join(threads[i], NULL), "pthread_join_failed");
    }
    
    osThreadCheck(pthread_mutex_destroy(&globalSum.lock), "pthread_mutex_destroy_failed");
    
    printf("Nakon sto su sve pokrenute niti zavrsile svoj rad, globalna suma iznosi %d!\n", globalSum.data);
    
    
    free(threads);
    
    exit(EXIT_SUCCESS);
}

