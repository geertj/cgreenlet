/*
 * This file is part of cgreenlet. CGreenlet is free software available
 * under the terms of the MIT license. Consult the file LICENSE that was
 * shipped together with this source file for the exact licensing terms.
 *
 * Copyright (c) 2012 by the cgreenlet authors. See the file AUTHORS for a
 * full list.
 */

#include <stdio.h>
#include <sys/time.h>
#include <setjmp.h>

#include "greenlet.h"
#include "greenlet-int.h"


float timeit(void (*func)(int))
{
    int count, elapsed;
    struct timeval start, end;

    count = 1;
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

void time_setjmp(int count)
{
    int i;
    jmp_buf buf;

    for (i=0; i<count; i++) {
        if (setjmp(buf))
            continue;
        longjmp(buf, 1);
    }
}

#if defined(__linux__) || defined(__APPLE__)
void time_sigsetjmp(int count)
{
    int i;
    sigjmp_buf buf;

    for (i=0; i<count; i++) {
        if (sigsetjmp(buf, 1))
            continue;
        siglongjmp(buf, 1);
    }
}
#endif

#if defined(__linux__)

#include <ucontext.h>

void time_getcontext(int count)
{
    int i;
    volatile int ret;
    ucontext_t uc;

    for (i=0; i<count; i++) {
        ret = 0;
        getcontext(&uc);
        if (ret == 1)
            continue;
        ret = 1;
        setcontext(&uc);
    }
}
#endif

void time_greenlet_switchcontext(int count)
{
    int i;
    void *frame[8];

    for (i=0; i<count; i++) {
        if (_greenlet_savecontext(frame))
            continue;
        _greenlet_switchcontext(frame, NULL, NULL);
    }
}

int main(int argc, char **argv)
{
    float mps;

    mps = timeit(time_setjmp) / 1000000.0;
    printf("setjmp/longjmp: %.2f million saves+restores/sec\n", mps);
#if defined(__linux__) || defined(__APPLE__)
    mps = timeit(time_sigsetjmp) / 1000000.0;
    printf("sigsetjmp/siglongjmp: %.2f million saves+restores/sec\n", mps);
#endif
#if defined(__linux__)
    mps = timeit(time_getcontext) / 1000000.0;
    printf("getcontext/setcontext: %.2f million saves+restores/sec\n", mps);
#endif
    mps = timeit(time_greenlet_switchcontext) / 1000000.0;
    printf("_greenlet_savecontext/_greenlet_switchcontext: "
           "%.2f million saves+restores/sec\n", mps);

    return 0;
}
