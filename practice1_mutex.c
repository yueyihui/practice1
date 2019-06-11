#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

bool FLAG_THREAD_INTERFERENCE = false;
pthread_mutex_t mMutex;
pthread_t* t1_p;
pthread_t* t2_p;

typedef int* ACOUNT;

pthread_t* threadLaunch(int*, void* (*)(void*));
void killThread(pthread_t *p);
void* t1(void*);
void* t2(void*);

void sig_alrm(int signo)
{
    printf("Process stop by CTRL-C\n");
    killThread(t1_p);
    killThread(t2_p);
    exit(0);
}

int main(int argc, const char *argv[])
{
    signal(SIGINT, sig_alrm);
    pthread_mutex_init(&mMutex, NULL);
    int c[2] = {0, 0};
    t1_p = threadLaunch(c, t1);
    t2_p = threadLaunch(c, t2);
    while (!FLAG_THREAD_INTERFERENCE) {
    }
    killThread(t1_p);
    killThread(t2_p);
    return 0;
}

pthread_t* threadLaunch(int* obj, void* (*f)(void*)) {
    //pthread_attr_t attr;
    //pthread_attr_init(&attr);
    //int rval = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    //if (rval != 0) { return NULL; }

    pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t));
    int rval = pthread_create(thread, NULL, f, obj);
    if (rval != 0) {
        printf("pthread_create failed: %d", rval);
        return NULL;
    }
    return thread;
}

void killThread(pthread_t *p)
{
    void *status;
    pthread_kill(*p, SIGUSR1);
    pthread_join(*p, &status);
    free(p);
}

void* t1(void* obj)
{
    ACOUNT a1 = (ACOUNT)obj; 
    ACOUNT a2 = ((ACOUNT)obj)+1; 
    bool goNext = false;
    int count = 0;
    do {
        pthread_mutex_lock(&mMutex);
        int tmp1 = *a1;
        int tmp2 = *a2;
        int r = random();
        *a1 = tmp1 +r ;
        *a2 = tmp2 -r;
        goNext = (*a1 + *a2 == 0);
        pthread_mutex_unlock(&mMutex);
        count++;
    } while (goNext);

    printf("thread1 run %d times, then encounter thread interference\n", count);
    FLAG_THREAD_INTERFERENCE = true;
    return (void*)NULL;
}

void* t2(void* obj)
{
    ACOUNT a1 = (ACOUNT)obj; 
    ACOUNT a2 = ((ACOUNT)obj)+1; 
    bool goNext = false;
    int count = 0;
    do {
        pthread_mutex_lock(&mMutex);
        int tmp1 = *a1;
        int tmp2 = *a2;
        int r = random();
        *a1 = tmp1 +r ;
        *a2 = tmp2 -r;
        goNext = (*a1 + *a2 == 0);
        pthread_mutex_unlock(&mMutex);
        count++;
    } while (goNext);

    printf("thread2 run %d times, then encounter thread interference\n", count);
    FLAG_THREAD_INTERFERENCE = true;
    return (void*)NULL;
}
