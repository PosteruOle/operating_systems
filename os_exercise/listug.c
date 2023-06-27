#define _X_OPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <pwd.h>
#include <grp.h>

#define check_error(cond,msg)\
  do{\
    if(!(cond)){\
      perror(msg);\
      fprintf(stderr,"File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
      exit(EXIT_FAILURE);\
    }\
  }while(0)


int main(){
   
   /*
   setpwent();

   struct passwd* currentUser;
   fprintf(stdout,"==============================\n");
   while((currentUser=getpwent())!=NULL){
      fprintf(stdout,"User name: %s\n", currentUser->pw_name);
      fprintf(stdout,"User password: %s\n", currentUser->pw_passwd);
      fprintf(stdout,"User id: %d\n", currentUser->pw_uid);
      fprintf(stdout,"Group id: %d\n", currentUser->pw_gid);
      fprintf(stdout,"==============================\n");
   }

   endpwent();
   */
   
   setgrent();
   struct group* currGroup;
   fprintf(stdout,"==============================\n");
   while((currGroup=getgrent())!=NULL){
      fprintf(stdout,"Group name: %s\n", currGroup->gr_name);
      fprintf(stdout,"Group id: %d\n", currGroup->gr_gid);
      for(int i=0;currGroup->gr_mem[i]!=NULL;i++){
         fprintf(stdout,"\t%s\n", currGroup->gr_mem[i]);
      }
      fprintf(stdout,"==============================\n");
   }
   
   endgrent();
   
 return 0;
}
