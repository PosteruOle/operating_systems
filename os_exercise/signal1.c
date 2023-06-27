#define  _DEFAULT_SOURCE
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

#include <signal.h>

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
int sigusr1count=0;
int sigusr2count=0;
int sigintcount=0; 
bool shouldTerminate=false;
 
void osSignalHandler(int signum){
   
   switch(signum){
      case SIGUSR1:  sigusr1count++;
                     break;
      case SIGUSR2:  sigusr2count++;
                     break;
      case SIGINT:  sigintcount++;
                     break;
      case SIGTERM:  shouldTerminate=true;
                     break;                              
   }

}

int main(int argc, char** argv){
   
  check_error(signal(SIGUSR1, osSignalHandler)!=SIG_ERR, "signal_failed");
  check_error(signal(SIGUSR2, osSignalHandler)!=SIG_ERR, "signal_failed");
  check_error(signal(SIGINT,osSignalHandler)!=SIG_ERR, "signal_failed");
  check_error(signal(SIGTERM, osSignalHandler)!=SIG_ERR, "signal_failed");
  
  fprintf(stderr,"My PID is %d\n", getpid());
  
  
  do{
  
    fprintf(stderr,"We are waiting for a signal to arive!\n");
    //check_error(pause()!=-1, "pause_failed");
    pause();
    fprintf(stderr,"Signal has arrived!\n");
  
  } while(!shouldTerminate);
  
  fprintf(stdout, "%d %d %d\n", sigusr1count, sigusr2count, sigintcount);

 return 0;
}
