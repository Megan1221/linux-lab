#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
static long real_second=0;
static long vt_second=0;
static long prof_second=0;
static struct itimerval real_t,vir_t,prof_t;

static void myhandle(int s){
	switch(s){
		case SIGALRM:
			real_second+=10;
			break;
		case SIGVTALRM:
			vt_second+=10;
			break;
		case SIGPROF:
			prof_second+=10;
			break;
		default:break;
	}
}

int main(){
	struct  itimerval value;
	int i,j;
	long moresec,moremsec,t1,t2;

	signal(SIGALRM,myhandle);
	signal(SIGVTALRM,myhandle);
	signal(SIGPROF,myhandle);

	value.it_interval.tv_sec=10;
	value.it_interval.tv_usec=0;
	value.it_value.tv_sec=10;
	value.it_value.tv_usec=0;

	setitimer(ITIMER_REAL,&value,NULL);
	setitimer(ITIMER_VIRTUAL,&value,NULL);
	setitimer(ITIMER_PROF,&value,NULL);

	for (i = 0; i < 1000; i++)
	{
		for (j = 0; j < 5000; j++)
		{
			int c=i+j;
			printf("%d\r",c);
			fflush(stdout);
		}
	}

	getitimer(ITIMER_REAL,&real_t);
	getitimer(ITIMER_VIRTUAL,&vir_t);
	getitimer(ITIMER_PROF,&prof_t);

	moresec=10-real_t.it_value.tv_sec;
	moremsec=(1000000-real_t.it_value.tv_usec)/1000;
	printf("realtime=%lds %ldms\n",real_second+moresec,moremsec );

	moresec=10-vir_t.it_value.tv_sec;
	moremsec=(1000000-vir_t.it_value.tv_usec)/1000;
	printf("usertime=%lds %ldms\n",vt_second+moresec,moremsec );

	moresec=10-prof_t.it_value.tv_sec;
	moremsec=(1000000-prof_t.it_value.tv_usec)/1000;
	printf("cputime=%lds %ldms\n",prof_second+moresec,moremsec );

	t1=(10-prof_t.it_value.tv_sec)*1000+(1000000-prof_t.it_value.tv_usec)/1000+prof_second*1000;
	t2=(10-vir_t.it_value.tv_sec)*1000+(1000000-vir_t.it_value.tv_usec)/1000+vt_second*1000;
	moresec=(t1-t2)/1000;
	moremsec=(t1-t2)%1000;
	printf("kerneltime=%lds %ldms\n",moresec,moremsec);
}