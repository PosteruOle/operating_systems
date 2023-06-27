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

#include <sys/epoll.h>
#include <string.h>

#include <ctype.h>

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
 
// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
 #define MAX 8
 
int digits;
char *maxfifo;

int main(int argc, char** argv){
   check_error(argc>=2, "argc");
   
   digits=0;
   maxfifo=malloc(25*sizeof(char));
   
   int numfifos=argc-1;
   int epfd=epoll_create(numfifos);
   check_error(epfd!=-1, "epoll_create");
   
   struct epoll_event currevent;
   for(int i=0;i<numfifos;i++){
      currevent.data.fd=open(argv[i+1], O_RDONLY | O_NONBLOCK);
      check_error(currevent.data.fd!=-1,"open_failed");
      
      currevent.events=EPOLLIN;
      
      
      check_error(epoll_ctl(epfd, EPOLL_CTL_ADD, currevent.data.fd, &currevent)!=-1, "epoll_ctl");
   }
   
   struct epoll_event events[MAX];
   
   int ready;
   check_error((ready=epoll_wait(epfd, events, MAX, -1))!=-1, "epoll_wait");
   
   //int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
   while(numfifos){
      for(int i=0;i<ready;i++){
          if(events[i].event & EPOLLIN){
            char c;
            int localdigits=0;
            while((c=fgetc())!=EOF){
               if(isalpha(c)){
                  localdigits++;
               }
            }
            if(localdigits>digits){
              digits=localdigits;
              for(int i=0;i<numfifo;i++)
              
              char *name=
              strcpy(maxfifo,)
            }
            
          } else if(events[i].event & (EPOLLHUP | EPOLLERR)){
          
          
          
          }
      
      }
   
   
   
   }
   
   close(epfd);

   free(maxfifo);

   exit(EXIT_SUCCES);
}
