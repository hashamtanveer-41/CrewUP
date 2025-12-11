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
    void delete_at_index(int index) {
        // 1. Check if list is empty or index is invalid
        if (head == NULL || index < 0) {
            cout << "List is empty or invalid index." << endl;
            return;
        }

        // 2. Handling removal of the HEAD (Index 0)
        if (index == 0) {
            Node* temp = head;
            head = head->next;

            // If list becomes empty, update tail too
            if (head == NULL) {
                tail = NULL;
            }

            delete temp;
            return;
        }

        // 3. Traverse to the node BEFORE the one we want to delete
        Node* prev = head;
        for (int i = 0; i < index - 1; i++) {
            // If we run out of nodes before reaching index, it's out of bounds
            if (prev->next == NULL) {
                cout << "Index out of bounds." << endl;
                return;
            }
            prev = prev->next;
        }

        // Check if the node to delete actually exists
        if (prev->next == NULL) {
            cout << "Index out of bounds." << endl;
            return;
        }

        // 4. Delete the node
        Node* toDelete = prev->next;
        prev->next = toDelete->next;

        // 5. Handling removal of the TAIL
        // If we deleted the last node, update the tail pointer
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
    void push_back(string val) {
        Node* newNode = new Node(val);

        if (head == NULL) {
            head = tail = newNode;
        }else {
            tail -> next=  newNode;
        }
    }
    void del(string key) {
        Node* temp = head;
        int idx = 0;

        while (temp!=NULL) {
            if (temp -> name == key) {
                cout<<"matched at "<<idx<<endl;
            }
            temp = temp-> next;
            idx++;
        }
        cout<<"Not found.";
    }

};
int main(){
    ofstream outputFile("Output.txt");
    string name;
    List ll;
    bool run = true;
    int choice, num = 0;
    Members members;

    do {
        cout<<"Please the required action: "<<endl;
        cout<<"1. Add members 2. Remove members 3. View members 4. exit"<<endl;
        cin>>choice;
        switch (choice) {
            case 1:
                cout<<"Enter the name of member: "<<endl;
                cin>>name;
                ll.push_front(name);

                break;
            case 2:
                cout<<"Enter the name of member: \n";
                cin>>name;
                ll.delete_at_index(ll.search(name));
                break;
            case 3:
                ll.printLL();
                break;
            case 4:
                run = false;
        }
    }
    while (run);

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
