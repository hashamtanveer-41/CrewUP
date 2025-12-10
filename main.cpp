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
int main(){
    ofstream outputFile("Output.txt");
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
