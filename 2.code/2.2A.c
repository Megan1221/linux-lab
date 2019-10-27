#include <sys/time.h>
#include <stdio.h>
#include <signal.h>

static int second=0;

static void myhandle(int s){
	second++;
	printf("%d\r",second);
	fflush(stdout);
}

int main(){
	struct itimerval value;
	signal(SIGALRM,myhandle);
	value.it_interval.tv_sec=1;
	value.it_interval.tv_usec=0;
	value.it_value.tv_sec=1;
	value.it_value.tv_usec=0;
	setitimer(ITIMER_REAL,&value,NULL);
	for (;;);
}
