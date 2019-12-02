/*两线程*/
void* add(void* x)
{       
    for(sum=0;sum<APPLE_MAX_VALUE;sum++)
    {
        ((struct apple *)x)->a += sum;
        ((struct apple *)x)->b += sum;   
    }
         
    return NULL;
}
     
int main (int argc, const char * argv[]) {
        // insert code here...
    struct apple test;
    struct orange test1={{0},{0}};
    pthread_t ThreadA;
         
    pthread_create(&ThreadA,NULL,add,&test);
         
    for(index=0;index<ORANGE_MAX_VALUE;index++)
    {
        sum += test1.a[index]+test1.b[index];
    }       
     
     pthread_join(ThreadA,NULL);
 
    return 0;
}
