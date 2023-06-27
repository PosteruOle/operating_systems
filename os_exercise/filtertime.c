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
#include <ftw.h>
#include <time.h>

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
 Program rekurzivno izdvaja sve fajlove koji su modifikovani 
 u poslednjih n dana na putanji koji prosledi korisnik
 Primer poziva:
 ./filterByTime dirPath brojDana
*/
int days;

int filterTime(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);
 
int main(int argc, char** argv){
   
   check_error(argc==3, "argc");
   days=atoi(argv[2]);
   
   check_error(nftw(argv[1], filterTime, 50, 0)!=-1, "nftw_failed");

   return 0;
}

int filterTime(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
   time_t now=time(NULL);
   time_t diff=now-sb->st_mtime;
   
   if(diff/(24*60*60)<days){
      fprintf(stdout,"\t%s\n", fpath);
   }
   
   return 0;
}


