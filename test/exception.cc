/*
 * This file is part of cgreenlet. CGreenlet is free software available
 * under the terms of the MIT license. Consult the file LICENSE that was
 * shipped together with this source file for the exact licensing terms.
 *
 * Copyright (c) 2012 by the cgreenlet authors. See the file AUTHORS for a
 * full list.
 */

#include <iostream>
#include <greenlet>

using namespace std;

greenlet *gr1, *gr2;

void *func1(void *arg)
{
    gr2->switch_to();
    return NULL;
}

void *func2(void *arg)
{
    throw 10;
}

int main(int argc, char **argv)
{
    gr1 = new greenlet(func1);
    gr2 = new greenlet(func2, gr1);

    try {
        gr1->switch_to();
    } catch (int i) {
        cout << "Exception correctly caught in main thread." << endl;
        cout << "Exception value: " << i << endl;
    } catch (greenlet_exit &e) {
        cout << "Exception caught but platform does not support rethrow." << endl;
    }
}
