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
   
   char* newName=NULL;
   
   int fdSrc=open(argv[1], O_RDONLY); 
   check_error(fdSrc!=-1, "open_failed");
   
   //char *oldName;
   char *oldName=strrchr(argv[1], '.');
   //check_error(oldName!=NULL, "strrchr_failed");
   
   int leno=strlen(argv[1]);
   int lenon=strlen(oldName);
   int lenn=strlen(argv[2]);

   newName=(char *)malloc(leno-lenon+lenn);
   strncpy(newName, argv[1], leno-lenon);
   strcat(newName, argv[2]);
   
   //printf("%s\n", newName);
   int fdDest=open(newName, O_RDWR | O_CREAT | O_TRUNC);
   check_error(fdDest!=-1, "open_failed");
   
   struct stat fileInfo;
   check_error(stat(argv[1], &fileInfo)!=-1, "stat_failed");
   close(fdSrc);
   check_error(unlink(argv[1])!=-1, "unlink_failed");
   
   check_error(chmod(newName, fileInfo.st_mode)!=-1, "chmod_failed");
   
   close(fdDest);
   

 return 0;
}

