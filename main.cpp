#include <iostream>
#include <string>
#include <fstream>
#include "List.h"

using namespace std;

int main(){
    int exp; float percent;
    string name;
    List ll;
    bool run = true;
    int choice, num = 0;
    ll.loadFile();

    do {
        cout<<"Please the required action: "<<endl;
        cout<<"1. Add members 2. Remove members 3. View members 4. exit"<<endl;
        cin>>choice;
        switch (choice) {
            case 1:
                cout<<"Enter the name of member: "<<endl;
                cin>>name;
                cout<<"Enter the percentage of member: "<<endl;
                cin>>percent;
                cout<<"Enter the exp of member: "<<endl;
                cin>>exp;
                ll.push_front(name,percent, exp);
                ll.saveFile();
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
