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
   check_error(argc==2, "argc");
   
   int fd=open(argv[1], O_RDONLY);
   check_error(fd!=-1, "open");
   
   struct stat fileInfo;
   check_error(stat(argv[1], &fileInfo)!=-1, "stat");
   
   printf("Velicina prosledjenog fajla iznosi: %ldB\n", fileInfo.st_size);
   
   /*
   int size=lseek(fd, 0, SEEK_END);
   printf("Velicina prosledjenog fajla iznosi: %dB\n", size);
   */


 return 0;
}

