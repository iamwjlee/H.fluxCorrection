#include "timer.h"



void (*timer_func_handler_pntr)(void);


#ifdef __linux__

void timer_handler(int);

struct itimerval timervalue;

struct sigaction new_handler, old_handler;

void timer_sig_handler(int);



int start_timer(int mSec, void (*timer_func_handler)(void))
{
  timer_func_handler_pntr = timer_func_handler;

  timervalue.it_interval.tv_sec = mSec / 1000;
  timervalue.it_interval.tv_usec = (mSec % 1000) * 1000;
  timervalue.it_value.tv_sec = mSec / 1000;
  timervalue.it_value.tv_usec = (mSec % 1000) * 1000;
  if(setitimer(ITIMER_REAL, &timervalue, NULL))
  {
    printf("\nsetitimer() error\n");
    return(1);
  }

  new_handler.sa_handler = &timer_sig_handler;
  new_handler.sa_flags = SA_NOMASK;
  if(sigaction(SIGALRM, &new_handler, &old_handler))
  {
    printf("\nsigaction() error\n");
    return(1);
  }

  return(0);
}


void timer_sig_handler(int arg)
{
  timer_func_handler_pntr();
}


void stop_timer(void)
{
  timervalue.it_interval.tv_sec = 0;
  timervalue.it_interval.tv_usec = 0;
  timervalue.it_value.tv_sec = 0;
  timervalue.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &timervalue, NULL);

  sigaction(SIGALRM, &old_handler, NULL);
}

#else

HANDLE win_timer;

VOID CALLBACK timer_sig_handler(PVOID, BOOLEAN);


int start_timer(int mSec, void (*timer_func_handler)(void))
{
  timer_func_handler_pntr = timer_func_handler;

  if(CreateTimerQueueTimer(&win_timer, NULL, (WAITORTIMERCALLBACK)timer_sig_handler, NULL, mSec, mSec, WT_EXECUTEINTIMERTHREAD) == 0)
  {
    printf("\nCreateTimerQueueTimer() error\n");
    return(1);
  }

  return(0);
}


VOID CALLBACK timer_sig_handler(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
  timer_func_handler_pntr();
}


void stop_timer(void)
{
  DeleteTimerQueueTimer(NULL, win_timer, NULL);
  CloseHandle(win_timer);
}
//

#endif


