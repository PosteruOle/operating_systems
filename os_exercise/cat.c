#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define check_error(cond,msg)\
   do{\
     if(!(cond)){\
        perror(msg);\
        fprintf(stderr,"File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
        exit(EXIT_FAILURE);\
     }\
   }while(0)


int main(int argc, char** argv){
   check_error(argc==2, "argc");
   
   int fd=open(argv[1], O_RDONLY);
   check_error(fd!=-1, "open");
   
   char *memBuff=malloc(1024*sizeof(char));
   int size=1024;
   int readBytes=0;
   while((readBytes=read(fd,memBuff,1024))>0){
        check_error(write(STDOUT_FILENO, memBuff, readBytes)!=-1, "write");
   }
   
   check_error(readBytes!=-1, "open");
   free(memBuff);

 return 0;
}
