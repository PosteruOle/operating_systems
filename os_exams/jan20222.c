#define _XOPEN_SOURCE 700
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
#include <ftw.h>

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
Napisati program koji kao argumente komande linije dobija putanju do direktorijuma i ekstenziju. Ekstenzija se
programu prosleđuje sa tačkom. Program treba rekurzivno da obiđe direktorijum i da izdvoji nazive svih regularnih
fajlove sa traženom ekstenzijom koji su modifikovani u poslednjih sedam dana. Za obilazak direktorijuma koristiti
ugrađenu funkciju nftw().
*/

char *ekstension;

int filterFiles(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf); 

int main(int argc, char** argv){
   check_error(argc==3, "argc_failed");
   ekstension=malloc(256*sizeof(char));
   strcpy(ekstension, argv[2]);

   check_error(nftw(argv[1], filterFiles, 50, 0)!=-1, "nftw_failed");

   free(ekstension);
   exit(EXIT_SUCCESS);
}
int filterFiles(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
   
   if(!S_ISREG(sb->st_mode))
     return 0;
     
   char *nameExt=strrchr(fpath, '.');
   if(nameExt==NULL)
     return 0;
   
   time_t now=time(NULL);
   check_error(now!=-1, "time_failed");
   
   if(((now-sb->st_mtime)/(24*60*60))<7){
     if(!strcmp(nameExt,ekstension)){
       fprintf(stdout,"\t%s\n", fpath);
     }
   }  
     
     
   return 0;
}

