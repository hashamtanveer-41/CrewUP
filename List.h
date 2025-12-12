//
// Created by Tanveer on 12/12/2025.

#ifndef CREWUP_LIST_H
#define CREWUP_LIST_H
#include "Node.h"
#include<string>
using namespace std;
class List {
    Node* head;
    Node*  tail;
public:
    List() {
        head = tail = NULL;
    }
    void push_front(string val, float p, int e) {
        Node* newNode= new Node(val, p, e);
        if (head == NULL) {
            head = tail= newNode;
            return;
        }else {
            newNode -> next = head;
            head = newNode;
        }
    }

    void printLL() {
         Node* temp = head;
        while (temp!= NULL) {
            cout<<"Name: "<<temp -> name<<" percentage : "<<temp->percent<<"% Exp is: "<<temp->exp <<endl;
            temp = temp -> next;
        }
        cout<< endl;
    }
    void delete_at_index(int index) {
        if (head == NULL || index < 0) {
            cout << "List is empty or invalid index." << endl;
            return;
        }

        if (index == 0) {
            Node* temp = head;
            head = head->next;

            if (head == NULL) {
                tail = NULL;
            }

            delete temp;
            return;
        }

        Node* prev = head;
        for (int i = 0; i < index - 1; i++) {
            if (prev->next == NULL) {
                cout << "Index out of bounds." << endl;
                return;
            }
            prev = prev->next;
        }

        if (prev->next == NULL) {
            cout << "Index out of bounds." << endl;
            return;
        }

        Node* toDelete = prev->next;
        prev->next = toDelete->next;
        if (toDelete == tail) {
            tail = prev;
        }

        delete toDelete;
    }
    int search(string key) {
        Node* temp = head;
        int idx = 0;

        while (temp!=NULL) {
            if (temp -> name == key) {
                cout<<"matched at "<<idx;
                return idx;
            }
            temp = temp-> next;
            idx++;
        }
        cout<<"Not found.";
        return -1;
    }
    void push_back(string val, float p, int e) {
        Node* newNode = new Node(val, p, e);

        if (head == NULL) {
            head = tail = newNode;
        }else {
            tail -> next=  newNode;
        }
    }

    void saveFile() {
        ofstream outputFile("Output.txt");
        Node* temp = head;
        while (temp != NULL) {
            outputFile<< temp-> name<<" "<<temp-> percent<<" "<< temp-> exp<<endl;
            temp = temp-> next;
        }
        outputFile.close();
    }

    void loadFile() {
        ifstream outputFile("Output.txt");
        if (!outputFile) return;
        string name; int exp; float percentage;
        while (outputFile >> name>> percentage>> exp) {
            push_back(name, percentage, exp);
        }
        outputFile.close();
    }
};

#endif //CREWUP_LIST_H