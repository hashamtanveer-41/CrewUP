#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// this is hasham's part
struct Node {
    string name;
    float percent;
    int exp;
    Node* next;
    Node(string val, float p, int e)
        : name(val), percent(p), exp(e), next(NULL) {}
};

Node* head = NULL;
Node* tail = NULL;

// this is Asfand's part
struct node {
    string tasks;
    node* next;
};

node* todo_head = NULL;
node* in_progress_head = NULL;
node* done_head = NULL;

// functions of hasham
void push_front(string , float, int);
void printLL();
void delete_at_index(int);
int search(string);
void push_back(string, float, int);
void saveFile();
void loadFile();
void shameBoard();
// functions of asfand
void add_task_to_list(node*& head, string task_description); // this will add the task
void display_task_list(node* head); // display the task
void delete_task_from_list(node*& head, string task_to_delete); // this will delete the task
void move_task_to_progress(node*& todo_head, node*& in_progress_head, string task_name); // this will move task in the progress section by breaking and linking the node
void move_task_to_done(node*& in_progress_head, node*& done_head, string task_name); // this will move the task in done in the same way

// Helper function for moving tasks.
void move_node(node*& source_head, node*& dest_head, string task_name);

// Menu
void display_task_menu(); // display the menu ( mtlb ke user ko instruction de ga ke 1 press krne se kya ho ya phir 2 se kya ho ga

// Main program (Uses Parent's main structure)
int main(){
    int exp; float percent;
    string name;
    bool run = true, runOut = true;
    int choice, num = 0;
    loadFile(); // Loads member data

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
                run = true; // Reset run for next loop iteration
                break;
            case 2: // INTEGRATED TASK TRACKER
                do {
                    display_task_menu();
                    cin >> choice; // takes input for switch choice
                    // Input cleanup for 'getline'
                    cin.ignore(100, '\n');  // yr ye delimiter use kiya hai mtlb ye ke ye values ko mix ni hone deta jese user int enter krta hai phir string tou ye har input ko seperate rakhta hai /n ki help se

                    switch (choice) {
                        case 1: {
                            string task_desc; // hasham this stores the contents on the task
                            cout << "Task to add:"<<endl;
                            getline(cin, task_desc); // getline is used so that it can also include spaces in the string
                            add_task_to_list(todo_head, task_desc);
                            break;
                        }
                        case 2:
                            // Display all three lists
                            cout << "\n--- TO-DO List ---" << endl;
                            display_task_list(todo_head); // this function displays each task when callled (simple si bt hai)
                            cout << "\n--- IN PROGRESS List ---" << endl;
                            display_task_list(in_progress_head); // this will display functions that are in progress
                            cout << "\n--- DONE List ---" << endl;
                            display_task_list(done_head); // this will display function sthat have been done
                            break;
                        case 3: {
                            string task_to_delete;
                            display_task_list(todo_head); // ye function dobara call kiya ta ke tasks phir se display ho sken
                            cout << "write the name of task you want to delete:"<<endl;
                            getline(cin, task_to_delete);
                            delete_task_from_list(todo_head, task_to_delete); // Deletes from To-Do list
                            break;
                        }
                        case 4: { // Move Task to IN PROGRESS
                            string task_name;
                            display_task_list(todo_head);
                            cout << "Enter task name to move to IN PROGRESS:" << endl;
                            getline(cin, task_name);
                            move_task_to_progress(todo_head, in_progress_head, task_name);
                            break;
                        }
                        case 5: { // Move Task to DONE
                            string task_name;
                            display_task_list(in_progress_head);
                            cout << "Enter task name to move to DONE:" << endl;
                            getline(cin, task_name);
                            move_task_to_done(in_progress_head, done_head, task_name);
                            break;
                        }
                        case 6:
                            runOut = false; // Exits the Task mode (inner loop)
                            break;
                        default:
                            cout << "Invalid choice." << endl;
                    }

                } while (runOut);
                runOut = true; // Reset runOut for main loop
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

    // Final Memory Cleanup for Task Lists
    node* current_task = todo_head;
    while (current_task != NULL ) {
        node* next = current_task->next;
        delete current_task;
        current_task = next;
    }
    current_task = in_progress_head;
    while (current_task != NULL ) {
        node* next = current_task->next;
        delete current_task;
        current_task = next;
    }
    current_task = done_head;
    while (current_task != NULL ) {
        node* next = current_task->next;
        delete current_task;
        current_task = next;
    }
    return 0;
}
// shame baord made by hasham
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
// menu to add/remove and delete tasks
void display_task_menu() {
    cout << "// Menu to add/remove/delete the tasks //" << endl;
    cout << " 1. Add Task (To-Do) " << endl;
    cout << " 2. Display All Lists " << endl;
    cout << " 3. Delete Task (from To-Do) " << endl;
    cout << " 4. Move Task to IN PROGRESS " << endl;
    cout << " 5. Move Task to DONE " << endl;
    cout << " 6. Exit Task Menu " << endl;
    cout << "Enter a choice "<<endl;
}

void add_task_to_list(node*& head, string task_description) {
    node* newNode = new node;
    newNode->tasks = task_description;
    newNode->next = head;
    head = newNode;
    cout << "Added: " << task_description << endl;
}

void display_task_list(node* head) {
    if (head == NULL) {
        cout << "List is empty." << endl;
        return;
    }

    int index = 1;
    while (head != NULL) {
        cout << index << ". " << head->tasks << endl;
        head = head->next;
        index++;
    }
}

void delete_task_from_list(node*& head, string task_to_delete) {
    node* current = head;
    node* prev = NULL;

    // Case A: Deleting the Head Node
    if (current != NULL && current->tasks == task_to_delete) {
        head = current->next;
        delete current;
        cout << "Deleted: " << task_to_delete << endl;
        return;
    }
    // Case B: Searching and Deleting
    while (current != NULL && current->tasks != task_to_delete) {
        prev = current;
        current = current->next;
    }
    // Task not found
    if (current == NULL) {
        cout << "Not found: " << task_to_delete << endl;
        return;
    }
    // this will unlink the node
    prev->next = current->next;
    delete current;             // This line will free the memory hasham
    cout << "Deleted: " << task_to_delete << endl;
}

// yr ye hepler function hai code ko link aur unlink krne ke liye gemini se bnwaya
void move_node(node*& source_head, node*& dest_head, string task_name) {
    node* current = source_head;
    node* prev = NULL;

    // iss se node find ho ga , task ka name likho ge tou jis node main task ho ga wohi node a jaye ga
    while (current != NULL && current->tasks != task_name) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        cout << "Error: Task '" << task_name << "' not found in the source list." << endl;
        return;
    }
    // iss se node unlink ho ga
    if (prev == NULL) {
        source_head = current->next; // Node was the head
    } else {
        prev->next = current->next; // Node was middle/end
    }
    // is se node destination pe link hon ga
    current->next = dest_head;
    dest_head = current;


    cout << "Moved task '" << task_name << "' successfully." << endl;
}

void move_task_to_progress(node*& todo_head, node*& in_progress_head, string task_name) {
    move_node(todo_head, in_progress_head, task_name);
}
void move_task_to_done(node*& in_progress_head, node*& done_head, string task_name) {
    move_node(in_progress_head, done_head, task_name);
}