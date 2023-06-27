#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define check_error(cond, msg)\
    do{\
      if(!(cond)){\
         perror(msg);\
         fprintf(stderr,"File: %s;\nLine: %d\n", __FILE__, __LINE__);\
         exit(EXIT_FAILURE);\
      }\
    }while(0)


int main(int argc, char** argv){
   check_error(argc==3, "argc");
   
   if(argv[1][0]=='-'){
     if(argv[1][1]=='f'){
       check_error(unlink(argv[2])==0, "unlink");
     } else if(argv[1][1]=='d'){
       check_error(rmdir(argv[2])==0, "rmdir");
     } else {
       fprintf(stderr,"Not a valid command!\n");
       exit(EXIT_FAILURE);
     }
   
   
   } else {
     fprintf(stderr,"Inapropriate form for command line!\n");
     exit(EXIT_FAILURE);
   } 



   return 0;
}
