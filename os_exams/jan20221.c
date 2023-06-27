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
   check_error(argc==3, "argc_inapropriate");
   
   int fdSrc=open(argv[1], O_RDONLY);
   check_error(fdSrc!=-1, "open_failed");
   
   struct stat fileInfo;
   check_error(stat(argv[1], &fileInfo)!=-1, "stat_failed");
   
   int fdDest=open(argv[2], O_CREAT | O_RDWR);
   check_error(fdDest!=-1, "open_fialed");
   
   // prepisivanje sadrzaja prvog fajla u drugi!!!!
   char *memBuff=malloc(1024*sizeof(char));
   int readBytes=0;
   while((readBytes=read(fdSrc, memBuff, 1024))>0){
       check_error(write(fdDest, memBuff, readBytes)!=-1, "write_failed");
   }
   check_error(readBytes!=-1, "read_failed");
   free(memBuff);
   
   
   
   check_error(chmod(argv[2], fileInfo.st_mode)!=-1,"chmod_failed");
   check_error(unlink(argv[1])!=-1, "unlink_failed");



 return 0;
}
