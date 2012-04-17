/*
 * This file is part of cgreenlet. CGreenlet is free software available
 * under the terms of the MIT license. Consult the file LICENSE that was
 * shipped together with this source file for the exact licensing terms.
 *
 * Copyright (c) 2012 by the cgreenlet authors. See the file AUTHORS for a
 * full list.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#include "greenlet.h"


float timeit(void (*func)(long))
{
    long count, elapsed;
    struct timeval start, end;

    count = 10;
    while (1) {
        gettimeofday(&start, NULL);
        func(count);
        gettimeofday(&end, NULL);
        elapsed = end.tv_usec - start.tv_usec;
        elapsed += 1000000 * (end.tv_sec - start.tv_sec);
        if (elapsed > 1000000)
            break;
        count *= 2;
    }
    return 1000000.0 * count / elapsed;
}

volatile long counter;
greenlet_t *gr1, *gr2;


void *_greenlet_func1(void *arg)
{
    while (counter > 0)
        greenlet_switch_to(gr2, NULL);
    return NULL;
}

void *_greenlet_func2(void *arg)
{
    while (1) {
        counter--;
        greenlet_switch_to(gr1, NULL);
    }
    return NULL;
}
    
void time_greenlet_switch_to(long count)
{
    gr1 = greenlet_new(_greenlet_func1, NULL, 0);
    gr2 = greenlet_new(_greenlet_func2, NULL, 0);
    if ((gr1 == NULL) || (gr2 == NULL)) {
        fprintf(stderr, "could not allocate greenlets\n");
        exit(1);
    }

    counter = count;
    greenlet_switch_to(gr1, NULL);

    greenlet_destroy(gr1);
    greenlet_destroy(gr2);
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *_pthread_func1(void *arg)
{
    while (counter > 0) {
        pthread_mutex_lock(&mutex);
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
    }
    pthread_cond_signal(&cond);
    return NULL;
}

void *_pthread_func2(void *arg)
{
    while (counter > 0) {
        pthread_mutex_lock(&mutex);
        counter--;
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void time_pthread_cond_wait(long count)
{
    int ret1, ret2;
    pthread_t pt1, pt2;
    void *ret;

    counter = count;
    ret1 = pthread_create(&pt1, NULL, _pthread_func1, NULL);
    ret2 = pthread_create(&pt2, NULL, _pthread_func2, NULL);
    if ((ret1 != 0) || (ret2 != 0)) {
        fprintf(stderr, "could not start pthreads\n");
        exit(1);
    }

    pthread_join(pt1, &ret);
    pthread_join(pt2, &ret);
}

int main(int argc, char **argv)
{
    float mps;

    mps = timeit(time_greenlet_switch_to) / 1000000.0;
    printf("greenlet_switch_to: %.2f million context switches/sec\n", mps);
    mps = timeit(time_pthread_cond_wait) / 1000000.0;
    printf("pthread_cond_wait: %.2f million context switches/sec\n", mps);

    return 0;
}
