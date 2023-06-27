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

#include <string.h>

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
Napisati višenitni program koji kao argumente komandne linije dobija putanje do regularnih fajlova. Za svaki fajl
treba pokrenuti odvojenu nit, u niti odrediti koliko reči ima u fajlu i dobijeni broj vratiti kao povratnu vrednost niti.
Po završetku rada program treba da ispiše ukupan broj reči u svim fajlovima i naziv fajla koji ima najviše reči.
Određivanje fajla sa najviše reči ne sme da se radi u main() funkciji, već mora u nitima. Za sinhronizaciju koristiti
muteks.
*/

typedef struct{
  int max;
  char *maxfile;
  pthread_mutex_t lock;
} osSafeType;

osSafeType globalMax;

typedef struct{
  int localwords;
} osThreadFunctionRetVal;

osThreadFunctionRetVal result;

void *osThreadFunction(void *arg){

    char *farg=arg;
    FILE *f=fopen(farg,"r");
    check_error(f!=NULL, "fopen_failed");
    
    int count=0;
    char **words;
    words=(char**)malloc(256*sizeof(char *));
    for(int i=0;i<256;i++){
       words[i]=(char *)malloc(60*sizeof(char));
    }
    int read=0;
    while(fscanf(f,"%s", words[read])==1){
       read++;
       count++;
    }
    printf("Nit ciji je TID %ld procitala sledece: %d %d\n", pthread_self(), read, count);
    for(int i=0;i<256;i++)
       free(words[i]);
    free(words);   
    
    osThreadCheck(pthread_mutex_lock(&globalMax.lock), "pthread_mutex_lock");
    if(count>globalMax.max){
      globalMax.max=count;
      strcpy(globalMax.maxfile,farg);
    }
    osThreadCheck(pthread_mutex_unlock(&globalMax.lock),"pthread_mutex_unlock");
    
    //osThreadFunctionRetVal result;
    result.localwords=count;
    //result.maxFile=NULL;
    return &result;

}

int main(int argc, char** argv){
   check_error(argc>=2, "argc");
   
   int n=argc-1;
   pthread_t *threads=malloc(n*sizeof(pthread_t));
   check_error(threads!=NULL, "Allocation failed!");
   
   int totalwords=0;
   osThreadFunctionRetVal *local;
   void *local1;
   
   globalMax.max=0;
   globalMax.maxfile=NULL;
   globalMax.maxfile=malloc(256*sizeof(char));
   check_error(globalMax.maxfile!=NULL, "Allocation failed!");
   
   osThreadCheck(pthread_mutex_init(&globalMax.lock,NULL),"pthread_mutex_init");
   
   for(int i=0;i<n;i++){
      osThreadCheck(pthread_create(&threads[i], NULL, osThreadFunction, argv[i+1]),"pthread_create");
   }
   int localwords;
   for(int i=0;i<n;i++){
      osThreadCheck(pthread_join(threads[i], (void **)&local), "pthread_join");
      totalwords+=local->localwords;
   }
   
   osThreadCheck(pthread_mutex_destroy(&globalMax.lock),"pthread_mutex_destroy");
   printf("%d %s\n", totalwords, globalMax.maxfile);
   
   free(threads);
   free(globalMax.maxfile);


 return 0;
}

