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
Program demonstrira rad sa zakljucavanjem fajlova. Poziv programa:
./lock_file  path/to/file  start  bytes  sleepTimeInSec  lockType (r or w)

Potrebno je zakljucati deo fajla za citanje/pisanje na putanji 'path/to/file' 
pocev od bajta 'start' duzine 'bytes'. Nakon toga uspavati proces
na 'sleepTimeInSec' sekundi, da bi drugi procesi stigli da probaju 
sa zakljucavanjem.
*/
int main(int argc, char** argv){
   check_error(argc==6, "argc_failed");
   
   int fd=open(argv[1], O_RDWR | O_CREAT);
   check_error(fd!=-1, "open_failed");
   
   char *path=argv[1];
   int start=atoi(argv[2]);
   int bytes=atoi(argv[3]);
   int sleepsec=atoi(argv[4]);
   int lockType;
   if(argv[5][0]=='r')
     lockType=F_RDLCK;
   else if(argv[5][1]=='w')
     lockType=F_WRLCK;
   
   struct flock lock;
   lock.l_start=start;
   lock.l_whence=SEEK_SET;
   lock.l_len=bytes;
   lock.l_type=lockType;
   
   check_error(fcntl(fd, F_SETLK, &lock)!=-1, "fcntl_failed");
   
   
   sleep(sleepsec);
   
   lock.l_type=F_UNLCK;
   check_error(fcntl(fd, F_SETLK, &lock)!=-1, "fcntl_failed");
   close(fd);
   
     




 return 0;
}

