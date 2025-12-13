#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Node;
Node* head = NULL;
Node* tail = NULL;

// Function prototypes
void push_front(string , float, int);
void printLL();
void delete_at_index(int);
int search(string);
void push_back(string, float, int);
void saveFile();
void loadFile();
void shameBoard();

int main(){
    int exp; float percent;
    string name;
    bool run = true, runOut = true;
    int choice, num = 0;
    loadFile();

    do {
        cout<<"Please the required action you want to perform related to: "<<endl;
        cout<<"1. Members 2. Tasks 3. Shame Board 4. Weekly leader 5. exit"<<endl;
        cin>>choice;
        switch (choice) {
            case 1:
               do {
                    cout<<"Please enter the required action: "<<endl;
                    cout<<"1. Add members 2. Remove members 3. View members 4. To exit Members mode."<<endl;
                    cin>>choice;
                    switch (choice) {
                        case 1:
                            cout<<"Enter the name of member: "<<endl;
                            cin>>name;
                            cout<<"Enter the percentage of member: "<<endl;
                            cin>>percent;
                            cout<<"Enter the exp of member: "<<endl;
                            cin>>exp;
                            push_front(name,percent, exp);
                            saveFile();
                            break;
                        case 2:
                            cout<<"Enter the name of member: \n";
                            cin>>name;
                            delete_at_index(search(name));
                            break;
                        case 3:
                            printLL();
                            break;
                        case 4:
                            run = false;
                            break;
                    }
                }while (run);
            case 2:
                break;
            case 3:
                shameBoard();
                break;
            case 4:
                break;
            case 5:
                runOut = false;
        }

    }while (runOut);

    return 0;
}
struct Node {
    string name;
    float percent;
    int exp;
    Node* next;
    Node(string val, float p, int e)
        : name(val), percent(p), exp(e), next(NULL) {}

};
void shameBoard() {
    Node* temp = head;
    bool check = true;
    while (temp!= NULL) {
        if (temp-> percent< 30) {
            cout<<"Name: "<<temp -> name<<" percentage : "<<temp->percent<<"% Exp is: "<<temp->exp <<endl;
            check = false;
        }
        temp = temp -> next;
    }
    if (check) cout<<"No member in shame board."<<endl;
    cout<< endl;
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
int search(string key) {
    Node* temp = head;
    int idx = 0;

    while (temp!=NULL) {
        if (temp -> name == key) {
            return idx;
        }
        temp = temp-> next;
        idx++;
    }
    cout<<"Not found.";
    return -1;
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
    cout<<"The member was removed successfully"<<endl;
}
void printLL() {
    Node* temp = head;
    while (temp!= NULL) {
        cout<<"Name: "<<temp -> name<<" percentage : "<<temp->percent<<"% Exp is: "<<temp->exp <<endl;
        temp = temp -> next;
    }
    cout<< endl;
}
void push_front(string val, float p, int e) {
    Node* newNode= new Node(val, p, e);
    if (head == NULL) {
        head = tail= newNode;
        return;
    }
    newNode -> next = head;
    head = newNode;
}