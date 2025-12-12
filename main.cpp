#include <iostream>
#include <string>
#include <fstream>

using namespace std;
struct Node {
    std::string name;
    float percent;
    int exp;
    Node* next;
    Node(string val, float p, int e)
        : name(val), percent(p), exp(e), next(NULL) {}

};
Node* head = NULL;
Node* tail = NULL;
void push_front(string val, float p, int e) {
    Node* newNode= new Node(val, p, e);
    if (head == NULL) {
        head = tail= newNode;
        return;
    }
    newNode -> next = head;
    head = newNode;
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
int main(){
    int exp; float percent;
    string name;
    bool run = true, runOut = true;
    int choice, num = 0;
    loadFile();

    do {  cout<<"Please the required action you want to perform related to: "<<endl;
        cout<<"1. Members 2. Tasks 3. Shame Board 5. Weekly leader 4. exit"<<endl;
        cin>>choice;
        switch (choice) {
            case 1:
                cout<<"Please the required action: "<<endl;
                cout<<"1. Add members 2. Remove members 3. View members 4. exit"<<endl;
                cin>>choice;do {
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
                    }
                }
                while (run);
        }
    }while (runOut);

    return 0;
}
