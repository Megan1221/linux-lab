/*三线程未加锁程序*/
struct apple
{
     unsigned long long a;
    unsigned long long b;
    pthread_rwlock_t rwLock;
};
 
void* addx(void* x)
{
    pthread_rwlock_wrlock(&((struct apple *)x)->rwLock);
    for(sum=0;sum<APPLE_MAX_VALUE;sum++)
    {
        ((struct apple *)x)->a += sum;
    }
    pthread_rwlock_unlock(&((struct apple *)x)->rwLock);
     
    return NULL;
}
 
void* addy(void* y)
{
    pthread_rwlock_wrlock(&((struct apple *)y)->rwLock);
    for(sum=0;sum<APPLE_MAX_VALUE;sum++)
    {
        ((struct apple *)y)->b += sum;
    }
    pthread_rwlock_unlock(&((struct apple *)y)->rwLock);
     
    return NULL;
}
 
 
 
int main (int argc, const char * argv[]) {
    // insert code here...
     struct apple test;
    struct orange test1={{0},{0}};
    pthread_t ThreadA,ThreadB;
     
    pthread_create(&ThreadA,NULL,addx,&test);
    pthread_create(&ThreadB,NULL,addy,&test);
 
    for(index=0;index<ORANGE_MAX_VALUE;index++)
    {
        sum+=test1.a[index]+test1.b[index];
    }
     
     pthread_join(ThreadA,NULL);
     pthread_join(ThreadB,NULL);
     
     return 0;
}
