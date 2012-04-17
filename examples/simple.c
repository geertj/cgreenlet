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
#include <greenlet.h>


greenlet_t *gr1, *gr2;


void *func1(void *arg)
{
    printf("in func1 (greenlet 1)\n");
    greenlet_switch_to(gr2, NULL);
    printf("back in func1 (greenlet 1)\n");
    greenlet_switch_to(gr2, NULL);
    printf("back again in func1 (greenlet 1)\n");
    return NULL;
}

void *func2(void *arg)
{
    printf("in func2 (greenlet 2)\n");
    greenlet_switch_to(gr1, NULL);
    printf("back in func2 (greenlet 2)\n");
    return NULL;
}

int main(int argc, char **argv)
{
    gr1 = greenlet_new(func1, NULL, 0);
    gr2 = greenlet_new(func2, NULL, 0);

    if ((gr1 == NULL) || (gr2 == NULL))
    {
        fprintf(stderr, "error: could not allocate greenlets\n");
        exit(1);
    }

    printf("entering greenlet 1\n");
    if (greenlet_switch_to(gr1, NULL) == gr2)
        printf("back in main from: greenlet 2\n");
    printf("greenlet 1 dead: %d\n", greenlet_isdead(gr1));
    printf("greenlet 2 dead: %d\n", greenlet_isdead(gr2));
    printf("re-entering greenlet 1\n");
    if (greenlet_switch_to(gr1, NULL) == gr1)
        printf("back in main from: greenlet 1\n");
    printf("greenlet 1 dead: %d\n", greenlet_isdead(gr1));
    printf("greenlet 2 dead: %d\n", greenlet_isdead(gr2));

    greenlet_destroy(gr1);
    greenlet_destroy(gr2);

    return 0;
}
