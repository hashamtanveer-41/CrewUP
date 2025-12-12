#ifndef CREWUP_NODE_H
#define CREWUP_NODE_H
#include <iostream>
using namespace std;

class Node {
public:
    string name;
    float percent;
    int exp;
    Node* next;

    Node(string val, float p, int e) {
        name = val;
        percent = p;
        exp = e;
        next = NULL;
    }
};


#endif