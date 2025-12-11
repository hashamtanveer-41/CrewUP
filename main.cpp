#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Functions Prototypes
void nameOfMembers(string arr[], int len);
void percentageOfMembers(int percentage);
struct Members{
    string name[5];
    int percentage;
    int exp;
};

class Node {
public:
    string name;
    Node* next;

    Node(string val) {
        name = val;
        next = NULL;
    }
};
class List {
    Node* head;
    Node*  tail;
public:
    List() {
        head = tail = NULL;
    }
    void push_front(string val) {
        Node* newNode= new Node(val);
        if (head == NULL) {
            head = tail= newNode;
            return;
        }else {
            newNode -> next = head;
            head = newNode;
        }
    }
    void pop_front() {
        if (head == NULL) {
            cout<<"No data found.";
        }

        Node* temp = head;
        head = head-> next;
        temp -> next = NULL;

        delete temp;
    }
    void insert(string val, int pos) {
        if (pos<0) {
            cout<<"Invalid Position"<<endl;
        }

        if (pos == 0) {
            push_front(val);
            return;
        }
        Node* temp = head;
        for (int i = 0; i<pos -1; i++) {
            temp = temp -> next;
        }

        Node* newNode = new Node(val);
        newNode -> next = temp -> next;
        temp -> next = newNode;
    }
    void printLL() {
         Node* temp = head;
        while (temp!= NULL) {
            cout<<temp -> name <<endl;
            temp = temp -> next;
        }
        cout<< endl;
    }
    void pop_back() {
        if (head == NULL) {
            cout<<"NO name.";
            return;
        }
        Node* temp = head;
        while (temp -> next != tail) {
            temp = temp -> next;
        }
        temp -> next = NULL;
        delete tail;
        tail = temp;
    }
    void search(string key) {
        Node* temp = head;
        int idx = 0;

        while (temp!=NULL) {
            if (temp -> name == key) {
            cout<<"matched at "<<idx;
                return;
            }
            temp = temp-> next;
            idx++;
        }
        cout<<"Not found.";
    }

    void push_back(string val) {
        Node* newNode = new Node(val);

        if (head == NULL) {
            head = tail = newNode;
        }else {
            tail -> next=  newNode;
        }
    }
};
int main(){
    ofstream outputFile("Output.txt");
    List ll;
    ll.push_front("3");
    ll.push_front("2");
    ll.push_front("1");
    ll.insert("4", 0);
    ll.printLL();

    ll.search("4");cout<<endl;
    bool run = true;
    int choice, num = 0;
    Members members;
    cout<<"Please the required action: "<<endl;
    cin>>choice;
    while (run) {
        switch (choice) {
            case 1:
                cout<<"Enter the number of members: ";
                cin>>num;
                nameOfMembers(members.name ,num);
                break;
            case 2:
                run = false;
        }
    }
    return 0;
}
void nameOfMembers(string arr[], int len) {
    Members members;
    arr = members.name;
    //int len =arr->length();
    for (int i= 0; i< len; i++) {
        cout<<"Enter the name of the member "<<i+1<<": "<<endl;
        cin>>arr[i];
    }
}
