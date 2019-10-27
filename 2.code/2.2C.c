#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static long p_rl_sec=0,c1_rl_sec=0,c2_rl_sec=0;
static long p_vt_sec=0,c1_vt_sec=0,c2_vt_sec=0;
static long p_pr_sec=0,c1_pr_sec=0,c2_pr_sec=0;

static struct itimerval p_rl_t,c1_rl_t,c2_rl_t;
static struct itimerval p_vt_t,c1_vt_t,c2_vt_t;
static struct itimerval p_pr_t,c1_pr_t,c2_pr_t;
static struct itimerval value;

int fibonacci(int n){
	if(n==0) return 0;
	else if(n==1 ||n==2) return 1;
	else return fibonacci(n-1)+fibonacci(n-2);
}

static void c1_sighandle(int s){
	switch(s){
		case SIGALRM:
			c1_rl_sec+=10;
			break;
		case SIGVTALRM:
			c1_vt_sec+=10;
			break;
		case SIGPROF:
			c1_pr_sec+=10;
			break;
		default:break;
	}
}

static void c2_sighandle(int s){
	switch(s){
		case SIGALRM:
			c2_rl_sec+=10;
			break;
		case SIGVTALRM:
			c2_vt_sec+=10;
			break;
		case SIGPROF:
			c2_pr_sec+=10;
			break;
		default:break;
	}
}

static void p_sighandle(int s){
	switch(s){
		case SIGALRM:
			p_rl_sec+=10;
			break;
		case SIGVTALRM:
			p_vt_sec+=10;
			break;
		case SIGPROF:
			p_pr_sec+=10;
			break;
		default:break;
	}
}


int main(int argc,char* argv[]){
	int num[3]={20,30,36};
	for (int i=0;i<argc-1;i++){
		num[i]=atoi(argv[i+1]);
	}
	int fib=0;
	int pid1,pid2;
	int status;
	long moresecs,moremsecs,t1,t2;
	pid1=fork();
	if(pid1==0){
		signal(SIGALRM,c1_sighandle);
		signal(SIGVTALRM,c1_sighandle);
		signal(SIGPROF,c1_sighandle);

		value.it_interval.tv_sec=10;
		value.it_interval.tv_usec=0;
		value.it_value.tv_sec=10;
		value.it_value.tv_usec=0;

		setitimer(ITIMER_REAL,&value,NULL);
		setitimer(ITIMER_VIRTUAL,&value,NULL);
		setitimer(ITIMER_PROF,&value,NULL);

		fib=fibonacci(num[0]);

		getitimer(ITIMER_REAL,&c1_rl_t);
		getitimer(ITIMER_VIRTUAL,&c1_vt_t);
		getitimer(ITIMER_PROF,&c1_pr_t);

		moresecs=10-c1_rl_t.it_value.tv_sec;
		moremsecs=(1000000-c1_rl_t.it_value.tv_usec)/1000;
		printf("c1 fib(%d)=%d\nrealtime=%lds %ldms\n",num[0],fib,c1_rl_sec+moresecs,moremsecs);

		moresecs=10-c1_vt_t.it_value.tv_sec;
		moremsecs=(1000000-c1_vt_t.it_value.tv_usec)/1000;
		printf("usertime=%lds %ldms\n",c1_vt_sec+moresecs,moremsecs);

		moresecs=10-c1_pr_t.it_value.tv_sec;
		moremsecs=(1000000-c1_pr_t.it_value.tv_usec)/1000;
		printf("cputime=%lds %ldms\n",c1_pr_sec+moresecs,moremsecs);

		t1=(10-c1_pr_t.it_value.tv_sec)*1000+(1000000-c1_pr_t.it_value.tv_usec)/1000+c1_pr_sec*1000;
		t2=(10-c1_vt_t.it_value.tv_sec)*1000+(1000000-c1_vt_t.it_value.tv_usec)/1000+c1_vt_sec*1000;
		moresecs=(t1-t2)/1000;
		moremsecs=(t1-t2)%1000;
		printf("kerneltime=%lds %ldms\n",moresecs,moremsecs);
		exit(0);
	}
	else{
		pid2=fork();
		if(pid2==0){
		signal(SIGALRM,c2_sighandle);
		signal(SIGVTALRM,c2_sighandle);
		signal(SIGPROF,c2_sighandle);

		value.it_interval.tv_sec=10;
		value.it_interval.tv_usec=0;
		value.it_value.tv_sec=10;
		value.it_value.tv_usec=0;

		setitimer(ITIMER_REAL,&value,NULL);
		setitimer(ITIMER_VIRTUAL,&value,NULL);
		setitimer(ITIMER_PROF,&value,NULL);

		fib=fibonacci(num[1]);

		getitimer(ITIMER_REAL,&c2_rl_t);
		getitimer(ITIMER_VIRTUAL,&c2_vt_t);
		getitimer(ITIMER_PROF,&c2_pr_t);

		moresecs=10-c2_rl_t.it_value.tv_sec;
		moremsecs=(1000000-c2_rl_t.it_value.tv_usec)/1000;
		printf("c2 fib(%d)=%d\nrealtime=%lds %ldms\n",num[1],fib,c2_rl_sec+moresecs,moremsecs);

		moresecs=10-c2_vt_t.it_value.tv_sec;
		moremsecs=(1000000-c2_vt_t.it_value.tv_usec)/1000;
		printf("usertime=%lds %ldms\n",c2_vt_sec+moresecs,moremsecs);

		moresecs=10-c2_pr_t.it_value.tv_sec;
		moremsecs=(1000000-c2_pr_t.it_value.tv_usec)/1000;
		printf("cputime=%lds %ldms\n",c2_pr_sec+moresecs,moremsecs);

		t1=(10-c2_pr_t.it_value.tv_sec)*1000+(1000000-c2_pr_t.it_value.tv_usec)/1000+c1_pr_sec*1000;
		t2=(10-c2_vt_t.it_value.tv_sec)*1000+(1000000-c2_vt_t.it_value.tv_usec)/1000+c1_vt_sec*1000;
		moresecs=(t1-t2)/1000;
		moremsecs=(t1-t2)%1000;
		printf("kerneltime=%lds %ldms\n",moresecs,moremsecs);
		exit(0);
		}
	}
	signal(SIGALRM,p_sighandle);
	signal(SIGVTALRM,p_sighandle);
	signal(SIGPROF,p_sighandle);

	value.it_interval.tv_sec=10;
	value.it_interval.tv_usec=0;
	value.it_value.tv_sec=10;
	value.it_value.tv_usec=0;

	setitimer(ITIMER_REAL,&value,NULL);
	setitimer(ITIMER_VIRTUAL,&value,NULL);
	setitimer(ITIMER_PROF,&value,NULL);

	fib=fibonacci(num[2]);

	getitimer(ITIMER_REAL,&p_rl_t);
	getitimer(ITIMER_VIRTUAL,&p_vt_t);
	getitimer(ITIMER_PROF,&p_pr_t);

	moresecs=10-p_rl_t.it_value.tv_sec;
	moremsecs=(1000000-p_rl_t.it_value.tv_usec)/1000;
	printf("p fib(%d)=%d\nrealtime=%lds %ldms\n", num[2],fib,p_rl_sec+moresecs,moremsecs);

	moresecs=10-p_vt_t.it_value.tv_sec;
	moremsecs=(1000000-p_vt_t.it_value.tv_usec)/1000;
	printf("usertime=%lds %ldms\n",p_vt_sec+moresecs,moremsecs);

	moresecs=10-p_pr_t.it_value.tv_sec;
	moremsecs=(1000000-p_pr_t.it_value.tv_usec)/1000;
	printf("cputime=%lds %ldms\n",p_pr_sec+moresecs,moremsecs);

	t1=(10-p_pr_t.it_value.tv_sec)*1000+(1000000-p_pr_t.it_value.tv_usec)/1000+c1_pr_sec*1000;
	t2=(10-p_vt_t.it_value.tv_sec)*1000+(1000000-p_vt_t.it_value.tv_usec)/1000+c1_vt_sec*1000;
	moresecs=(t1-t2)/1000;
	moremsecs=(t1-t2)%1000;
	printf("kerneltime=%lds %ldms\n",moresecs,moremsecs);
	
	wait(&status);
	wait(&status);

}