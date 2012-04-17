/*
 * This file is part of cgreenlet. CGreenlet is free software available
 * under the terms of the MIT license. Consult the file LICENSE that was
 * shipped together with this source file for the exact licensing terms.
 *
 * Copyright (c) 2012 by the cgreenlet authors. See the file AUTHORS for a
 * full list.
 */

#include <new>
#include <iostream>
#include <greenlet>

using namespace std;

greenlet *gr1;
greenlet *gr2;

class greenlet1: greenlet
{
    void *run(void *arg)
    {
        cout << "in greenlet1::run()\n";
        gr2->switch_to();
        cout << "back in greenlet1::run()\n";
        gr2->switch_to();
        cout << "back again in greenlet1::run()\n";
        return NULL;
    }
};

class greenlet2: greenlet
{
    void *run(void *arg)
    {
        cout << "in greenlet2::run()\n";
        gr1->switch_to();
        cout << "back in greenlet2::run()\n";
        return NULL;
    }
};


int main(int argc, char **argv)
{
    gr1 = (greenlet *) new greenlet1;
    gr2 = (greenlet *) new greenlet2;

    cout << "entering greenlet1" << endl;
    if (gr1->switch_to() == gr2)
        cout << "back in main from greenlet2" << endl;
    cout << "greenlet1 dead: " << gr1->isdead() << endl;
    cout << "greenlet2 dead: " << gr2->isdead() << endl;
    cout << "re-entering greenlet1" << endl;
    if (gr1->switch_to() == gr1)
        cout << "back in main from greenet1" << endl;
    cout << "greenlet1 dead: " << gr1->isdead() << endl;
    cout << "greenlet2 dead: " << gr2->isdead() << endl;

    delete gr1;
    delete gr2;
}
