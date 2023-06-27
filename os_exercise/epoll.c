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
Program demonstrira nacin za multipleksiranje fajlova putem 
interfejsa epoll. 

Kao argumenti komandne linije, zadaju se putanje do fifo fajlova.
 
Potrebno je sacekati na podatke u tim fajlovima, 
pri cemu se ne zna unapred kada ce i na kom fajlu pristici podaci. 
Poruke koje stizu potrebno je ispisati na standardni izlaz. Program
se zaustavlja kada programi koji su otvorili fifo fajlove za pisanje 
zatvore svoj kraj.
*/

// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

int main(int argc, char** argv){
   
   check_error(argc>=2, "argc_failed");
   int numfifos=argc-1;
   
   int epfd=epoll_create(numfifos);
   check_error(epfd!=-1,"epoll_create_failed");
   
   struct epoll_event currevent;
   for(int i=0;i<numfifos;i++){
      memset(&currevent, 0, sizeof(struct epoll_event));
      currevent.data.fd=open(argv[i+1], O_RDONLY | O_NONBLOCK, 0777);
      currevent.events=EPOLLIN;
   
      check_error(epoll_ctl(epfd, EPOLL_CTL_ADD, currevent.data.fd, &currevent)!=-1, "epoll_ctl");
   }
   
   struct epoll_event events[10];
   int fdready[10];
   int ready;
   //int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
   while(numfifos){
        check_error((ready=epoll_wait(epfd, events, 10, -1))!=-1, "epoll_wait_failed");
        
        for(int i=0;i<ready;i++){
            if(events[i].events & EPOLLIN){
              char *membuff=malloc(128*sizeof(char));
              int readbytes;
              
              readbytes=read(events[i].data.fd, membuff, 128);
              check_error(readbytes!=-1, "read_failed");
              membuff[readbytes]='\0';
              
              fprintf(stdout, "%d. FIFO sent: %s\n", i , membuff);
              //numfifos--;
              
              free(membuff);            
            } else if(events[i].events & (EPOLLHUP || EPOLLERR)) {
              //fprintf(stderr, "Dogodila se greska, prekidamo program!\n");
              //exit(EXIT_FAILURE);
              printf("FIFO %d is closed!\n", events[i].data.fd);
              close(events[i].data.fd);
              numfifos--;
              
            }
        } 
   }
   close(epfd);

 return 0;
}
