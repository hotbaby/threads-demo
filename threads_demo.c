/*************************************************************************
> File Name: threads_demo.c
> Author: yy
> Mail: mengyy_linux@163.com
 ************************************************************************/
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#define __USE_GNU   /* resolve CPU_SET() undefined symbol*/
#include <sched.h>
#include <pthread.h>


#define LOOP_NUMBER (50)

void* func1(void *arg)
{
    int i;
    int ret;
    int policy; 
    cpu_set_t set, mask;
    struct sched_param param;
    int n = LOOP_NUMBER;
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF); 

    CPU_ZERO(&set);
    CPU_SET(1, &set);
    ret = pthread_setaffinity_np(pthread_self(), sizeof(set), &set);
    assert(ret == 0);
    
    CPU_ZERO(&mask);
    pthread_getaffinity_np(pthread_self(), sizeof(mask), &mask);
    for (i = 0; i < cpu_num; i++)
    {
        if (CPU_ISSET(i, &mask))
        {
            fprintf(stderr, "thread 1 run processer %d\n", i);
        }
    }

    pthread_getschedparam(pthread_self(), &policy, &param);
    if (policy == SCHED_OTHER)
    {
        fprintf(stderr, "thread t1 schedule policy SCHED_OTHER!\n");
    }
    else if (policy == SCHED_FIFO)
    {
        fprintf(stderr, "thread t1 schedule policy SCHED_FIFO!\n");
    }
    else if(policy == SCHED_RR)
    {
        fprintf(stderr, "thread t1 schedule policy SCHED_RR!\n");
    }
    else
    {
        fprintf(stderr, "tread t1 unsupported policy!\n");
    }
    
    while(n--)
    {
        fprintf(stderr, "thread 1!\n");
    }
    
    return NULL;
}

int main(int argc, char **argv)
{
    int i;
    int ret;
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF); 
    cpu_set_t set, mask;
    int policy, inher;        
    pthread_t tid1;
    pthread_attr_t attr;
    struct sched_param param; 

    fprintf(stderr, "cpu number:%d\n", cpu_num);

    CPU_ZERO(&set);
    CPU_SET(0, &set);
    ret = pthread_setaffinity_np(pthread_self(), sizeof(set), &set);
    assert(ret == 0);

    pthread_getschedparam(pthread_self(), &policy, &param);
    if (policy == SCHED_OTHER)
    {
        fprintf(stderr, "schedule policy SCHED_OTHER!\n");
    }
    else if (policy == SCHED_FIFO)
    {
        fprintf(stderr, "schedule policy SCHED_FIFO!\n");
    }
    else if(policy == SCHED_RR)
    {
        fprintf(stderr, "schedule policy SCHED_RR!\n");
    }
    else
    {
        fprintf(stderr, "unsupported policy!\n");
    }

    /* high prority thread */
    pthread_attr_init(&attr);
    
    pthread_attr_getinheritsched(&attr, &inher);
    if (inher == PTHREAD_EXPLICIT_SCHED)
    {
        fprintf(stderr, "inherit schedule policy PTHREAD_EXPLICIT_SCHED\n");
    }
    else if (inher == PTHREAD_INHERIT_SCHED)
    {
        fprintf(stderr, "inherit schedule policy PTHREAD_INHERIT_SCHED\n");
    }
    else
    {
        fprintf(stderr, "unsupported inherit schedule policy.\n");
        assert(0);
    }
    inher = PTHREAD_EXPLICIT_SCHED;
    ret = pthread_attr_setinheritsched(&attr, inher);
    assert(ret == 0);

    pthread_attr_getschedpolicy(&attr, &policy);
    policy = SCHED_FIFO;
    ret = pthread_attr_setschedpolicy(&attr, policy);
    assert(ret == 0);

    pthread_attr_getschedparam(&attr, &param);
    param.sched_priority = 3;
    ret = pthread_attr_setschedparam(&attr, &param);
    assert(ret == 0);
    
    pthread_create(&tid1, &attr, func1, NULL);
    pthread_attr_destroy(&attr);

    /* normal priority thread (main thread)*/
    CPU_ZERO(&mask);
    pthread_getaffinity_np(pthread_self(), sizeof(mask), &mask);
    for (i = 0; i < cpu_num; i++)
    {
        if (CPU_ISSET(i, &mask))
        {
            fprintf(stderr, "main thread run processer %d\n", i);
        }
    }

    int n = LOOP_NUMBER;
    while(n--)
    {
        fprintf(stderr, "main thread!\n");
    }

    pthread_join(tid1, NULL);
    
    return 0;
}
