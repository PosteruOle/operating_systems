#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX 256

#define check_error(cond, msg)\
   do{\
     if(!(cond)){\
        perror(msg);\
        fprintf(stderr,"File: %s; Line: %d\n", __FILE__, __LINE__);\
        exit(EXIT_FAILURE);\
     }\
   } while(0)   

int main(int argc, char** argv){
   check_error(argc==2, "argc");
   
   char pathFile[MAX];
   strcpy(pathFile, argv[1]);
   
   check_error(mkdir(pathFile, 0755)==0, "mkdir");
   printf("Uspesno smo napravili direktorijum!\n");

   return 0;
}
