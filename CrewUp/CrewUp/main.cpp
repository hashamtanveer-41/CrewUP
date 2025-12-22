#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

// Structs declaration
struct MemberNode {
    string name;
    float percent;
    int exp;
    MemberNode* next;
    MemberNode(string val, float p, int e) : name(val), percent(p), exp(e), next(NULL) {}
};
struct TaskNode {
    string title;
    string assignedTo;
    string date;
    int xp;
    bool isVerified;
    TaskNode* next;
};
struct LeaderState {
    string leaderName = "None";
    time_t electionTime  = 0;
    bool hasData = false;
};

// Global Variables
MemberNode* memberHead = NULL;
MemberNode* memberTail = NULL;
TaskNode* todo_head = NULL;
TaskNode* in_progress_head = NULL;
TaskNode* done_head = NULL;
string currentUser = "Guest";
int leaderMode = 0;

// Function prototyping
void push_member(string, float, int);
void remove_member(string);
int totalTasksXP();
void updatePercentages();
void add_xp(string,int );
void sort_members();
void loadMembers();
void saveMembers();
void add_task_to_list(TaskNode*&, string, int, string);
void move_node_logic(TaskNode*&, TaskNode*&, string, string);
void delete_task_globally(string);
void saveTasks();
void loadTasks();
string getShameMember();
void drawTaskCard(sf::RenderWindow& , sf::Font& , TaskNode* , float, float , int );
void drawMembersPanel(sf::RenderWindow&, sf::Font& );
void drawRightPanel(sf::RenderWindow& , sf::Font& , string , bool );

int main() {
    sf::RenderWindow window(sf::VideoMode(1360, 800), "CrewUp", sf::Style::Titlebar | sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/Roboto-Bold.ttf")) {
        return -1;
    }
    loadMembers(); // Load members
    loadTasks(); // Load saved tasks
    updatePercentages(); // Updating the percentages
    // Default data if empty
    if (memberHead == NULL) {
        push_member("Hamza", 0, 0);
        push_member("Hasham", 0, 0);
        currentUser = "Hamza";
    }
    else {
        currentUser = memberHead->name;
    }

    string inputString = "";
    bool isTyping = false;

    sf::RectangleShape p1(sf::Vector2f(280, 780)); p1.setPosition(10, 10); p1.setFillColor(sf::Color(55, 60, 70));
    sf::RectangleShape p2(sf::Vector2f(740, 780)); p2.setPosition(300, 10); p2.setFillColor(sf::Color(55, 60, 70));
    sf::RectangleShape p3(sf::Vector2f(300, 780)); p3.setPosition(1050, 10); p3.setFillColor(sf::Color(55, 60, 70));
        // Runing the GUI frames in the loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                saveMembers();
                saveTasks(); // Save on exit
                window.close();
            }

            if (event.type == sf::Event::TextEntered && isTyping) {
                if (event.text.unicode == 8) {
                    if (!inputString.empty()) inputString.pop_back();
                }
                else if (event.text.unicode < 128 && event.text.unicode > 31) {
                    inputString += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // --- TOP RIGHT: USER SWITCHER ---
                if (mousePos.x > 1070 && mousePos.x < 1300 && mousePos.y > 0 && mousePos.y < 50) {
                    MemberNode* temp = memberHead;
                    while (temp != NULL) {
                        if (temp->name == currentUser) {
                            if (temp->next != NULL) currentUser = temp->next->name;
                            else if (memberHead != NULL) currentUser = memberHead->name;
                            break;
                        }
                        temp = temp->next;
                    }
                }

                // --- LEADER MODE BUTTONS ---
                if (mousePos.y > 440 && mousePos.y < 470) {
                    if (mousePos.x > 1070 && mousePos.x < 1130) leaderMode = 0;
                    if (mousePos.x > 1135 && mousePos.x < 1195) leaderMode = 1;
                    if (mousePos.x > 1200 && mousePos.x < 1260) leaderMode = 2;
                    if (mousePos.x > 1265 && mousePos.x < 1325) leaderMode = 3;
                }

                // --- INPUT BOX ---
                if (mousePos.x > 1070 && mousePos.x < 1330 && mousePos.y > 490 && mousePos.y < 530) isTyping = true;
                else isTyping = false;

                // --- EXECUTE BUTTON ---
                if (mousePos.x > 1070 && mousePos.x < 1330 && mousePos.y > 540 && mousePos.y < 580) {
                    if (!inputString.empty()) {
                        if (leaderMode == 0) { // Add Task
                            size_t commaPos = inputString.find(",");
                            string tName = inputString;
                            int tXp = 100;
                            if (commaPos != string::npos) {
                                tName = inputString.substr(0, commaPos);
                                try { tXp = stoi(inputString.substr(commaPos + 1)); }
                                catch (...) { tXp = 100; }
                            }
                            add_task_to_list(todo_head, tName, tXp, "");
                        }
                        else if (leaderMode == 1) { push_member(inputString, 0, 0); updatePercentages(); } // Add Mem
                        else if (leaderMode == 2) remove_member(inputString); // Del Mem

                        inputString = "";
                    }
                }

                //THE LOGIC SWITCH (TASK INTERACTIONS)
                auto checkListClick = [&](TaskNode* head, int type) {
                    TaskNode* temp = head;
                    int y = 60;
                    while (temp != NULL) {
                        int xStart = (type == 0) ? 320 : (type == 1 ? 560 : 810);

                        // Check if click hit a card
                        if (mousePos.x > xStart && mousePos.x < xStart + 220 && mousePos.y > y && mousePos.y < y + 100) {

                            // 1. DELETE MODE
                            if (leaderMode == 3) {
                                delete_task_globally(temp->title);
                                return true;
                            }

                            // 2. NORMAL MODES
                            else {
                                if (type == 0) { // ToDo -> InProgress
                                    move_node_logic(todo_head, in_progress_head, temp->title, currentUser);
                                }
                                else if (type == 1) { // InProgress -> Done
                                    move_node_logic(in_progress_head, done_head, temp->title, "");
                                }
                                else if (type == 2) { // Verify Logic
                                    if (!temp->isVerified && temp->assignedTo != currentUser) {
                                        add_xp(temp->assignedTo, temp->xp);
                                        add_xp(currentUser, 50);
                                        temp->isVerified = true;
                                    }
                                }
                                return true;
                            }
                        }
                        y += 110;
                        temp = temp->next;
                    }
                    return false;
                    };

                // Run the checks
                if (!checkListClick(todo_head, 0)) {
                    if (!checkListClick(in_progress_head, 1)) {
                        checkListClick(done_head, 2);
                    }
                }
            }
        }

        window.clear(sf::Color(40, 44, 52));
        window.draw(p1); window.draw(p2); window.draw(p3);

        sf::Text h1("TO DO", font, 20); h1.setPosition(320, 20); window.draw(h1);
        sf::Text h2("IN PROGRESS", font, 20); h2.setPosition(570, 20); window.draw(h2);
        sf::Text h3("DONE", font, 20); h3.setPosition(820, 20); window.draw(h3);

        float y = 60;
        TaskNode* t = todo_head;
        while (t != NULL) { drawTaskCard(window, font, t, 320, y, 0); y += 110; t = t->next; }

        y = 60;
        t = in_progress_head;
        while (t != NULL) { drawTaskCard(window, font, t, 560, y, 1); y += 110; t = t->next; }

        y = 60;
        t = done_head;
        while (t != NULL) { drawTaskCard(window, font, t, 810, y, 2); y += 110; t = t->next; }

        drawMembersPanel(window, font);
        drawRightPanel(window, font, inputString, isTyping);

        window.display();
    }
    return 0;
}

// Function to add member in the list
void push_member(string val, float p, int e) {
    // Creating node of variable newNode
    MemberNode* newNode = new MemberNode(val, p, e);
    // Checking for the edge cases
    if (memberHead == NULL) {
        memberHead = memberTail = newNode;
    }
    else {
        // Adding the node after checking the edge case
        memberTail->next = newNode;
        memberTail = newNode;
    }
}

// Function to remove the member from the list
void remove_member(string name) {
    // Creating the Node of variabe current
    MemberNode* current = memberHead;
    MemberNode* prev = NULL;
    // Checking at each node if the required node is found
    while (current != NULL && current->name != name) {
        prev = current;
        current = current->next;
    }
    // Checking for the edge cases
    if (current == NULL) return;
    // Deleting the node if found in the list
    if (prev == NULL) memberHead = current->next;
    else prev->next = current->next;
    if (current == memberTail) memberTail = prev;
    delete current;
}

// Function to add the total Exp of all the tasks
int totalTasksXP() {
    // Setting the total variable to zero
    int total = 0;
    // Creating a sample node to traverse through the list
    TaskNode* t = todo_head;
    // Checking for the edge cases
    while (t != NULL) {
        total += t->xp;
        t = t->next;
    }
    t = in_progress_head;
    while (t != NULL) {
        total += t->xp;
        t = t->next;
    }
    t = done_head;
    while (t != NULL) {
        total += t->xp;
        t = t->next;
    }
    return total;
}

// Function to Update the Percentage of each member
void updatePercentages() {
    // Calculate total earned XP across all members (team total)
    int totalMembersXP = 0;
    MemberNode* temp = memberHead;
    while (temp != NULL) {
        totalMembersXP += temp->exp;
        temp = temp->next;
    }

    // If no member has earned XP, set everyone's percentage to 0
    temp = memberHead;
    if (totalMembersXP <= 0) {
        while (temp != NULL) {
            temp->percent = 0.0f;
            temp = temp->next;
        }
        return;
    }

    // Otherwise compute each member's contribution from the members' total
    while (temp != NULL) {
        temp->percent = (static_cast<float>(temp->exp) / static_cast<float>(totalMembersXP)) * 100.0f;
        if (temp->percent > 100.0f) temp->percent = 100.0f;
        temp = temp->next;
    }
}

// Function to add the exp of each member depending on the tasks choosen by the member
void add_xp(string name, int amount) {
    // Node to traverse through the list
    MemberNode* temp = memberHead;
    // Adding the exp of member
    while (temp != NULL) {
        if (temp->name == name) {
            temp->exp += amount;
            updatePercentages();
            return;
        }
        temp = temp->next;
    }
}

// Function to sort the member of list based on their exp
void sort_members() {
    // Checking for the edge cases
    if (memberHead == NULL || memberHead->next == NULL) return;
    bool swapped;
    MemberNode* ptr1;
    MemberNode* lptr = NULL;
    // Runing the loop until the members are sorted
    do {
        swapped = false;
        ptr1 = memberHead;
        while (ptr1->next != lptr) {
            if (ptr1->exp < ptr1->next->exp) {
                swap(ptr1->exp, ptr1->next->exp);
                swap(ptr1->percent, ptr1->next->percent);
                swap(ptr1->name, ptr1->next->name);
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Function to load the members
void loadMembers() {
    // Loading the file
    ifstream outputFile("Output.txt");
    // Checking for the file
    if (!outputFile) return;
    // Declaring the varaibles
    string name;
    int exp;
    float percentage;
    // Reading the members from the file
    while (outputFile >> name >> percentage >> exp) {
        push_member(name, percentage, exp);
    }
    // Closing the file
    outputFile.close();
}

// Function to save the members in the file
void saveMembers() {
    // Loading the file
    ofstream outputFile("Output.txt");
    // Creating Node
    MemberNode* temp = memberHead;
    // Runing the loop Until all the added members are added in the file
    while (temp != NULL) {
        outputFile << temp->name << " " << temp->percent << " " << temp->exp << endl;
        temp = temp->next;
    }
    // Closing the file
    outputFile.close();
}

// Function to add the task to the List
void add_task_to_list(TaskNode*& head, string title, int xp, string date) {
    // Creating the node to add the task in list
    TaskNode* newNode = new TaskNode;
    // Giving the data to the Node
    newNode->title = title;
    newNode->xp = xp;
    newNode->date = date;
    newNode->assignedTo = "Unassigned";
    newNode->isVerified = false;
    newNode->next = head;
    head = newNode;
}

// Function to move the node
void move_node_logic(TaskNode*& source_head, TaskNode*& dest_head, string task_name, string assigneeName) {
    // Node to traverse through the list
    TaskNode* current = source_head;
    TaskNode* prev = NULL;

    // Checking for the edge cases
    while (current != NULL && current->title != task_name) {
        prev = current;
        current = current->next;
    }
    // Checking if the current is null then return
    if (current == NULL) return;

    // Moving the source head node to currents next if previous is null
    if (prev == NULL) source_head = current->next;
    else prev->next = current->next;
    if (assigneeName != "") current->assignedTo = assigneeName;

    // Traversing through the list
    current->next = dest_head;
    dest_head = current;
}

// Function to delete the task globally
void delete_task_globally(string title) {
    // (TaskNode*& head) is passed by reference so we can modify the actual head pointer of the list.
    auto deleteFromList = [&](TaskNode*& head) -> bool {
        TaskNode* current = head;
        TaskNode* prev = NULL;

        // Traverse the linked list
        while (current != NULL) {
            // Check if the current node matches the title we want to delete
            if (current->title == title) {

                // Case 1: The node to delete is the head of the list
                if (prev == NULL) head = current->next;
                // Case 2: The node is in the middle or end
                else prev->next = current->next;

                delete current; // Free the memory
                return true;    // Return true to indicate successful deletion
            }
            // Move pointers forward
            prev = current;
            current = current->next;
        }
        return false; // Return false if the title was not found in this list
    };
    // If NOT found (!deleteFromList), move to the next list.
    if (!deleteFromList(todo_head)) {

        // If not in To-Do, check 'In-Progress'
        if (!deleteFromList(in_progress_head)) {

            // If not in In-Progress, finally check 'Done'
            deleteFromList(done_head);
        }
    }
}

// Function to save the tasks
void saveTasks() {
    // Opening the file tasks to write the data in it
    ofstream file("Tasks.txt");
    // Saving all the tasks added in the list
    auto saveList = [&](TaskNode* head, int type) {
        // Node to traverse through the list
        TaskNode* temp = head;
        while (temp != NULL) {
            file << type << endl << temp->xp << endl << temp->isVerified << endl;
            file << temp->title << endl << temp->assignedTo << endl << temp->date << endl;
            temp = temp->next;
        }
    };
    saveList(todo_head, 0); saveList(in_progress_head, 1); saveList(done_head, 2);
    // Closing the file
    file.close();
}


void loadTasks() {
    ifstream file("Tasks.txt"); // Open the text file for reading
    if (!file) return; // If the file doesn't exist, stop here

    int type, xp; bool isVerified;
    string title, assigned, date;

    // Keep reading as long as there is data in the file
    while (file >> type >> xp >> isVerified) {
        file.ignore(); // Skip the leftover newline after reading the numbers

        // Read the text lines (Title, Assigned person, Date)
        getline(file, title);
        getline(file, assigned);
        getline(file, date);

        // Create a new task and fill it with the data we just read
        TaskNode* newNode = new TaskNode;
        newNode->title = title;
        newNode->xp = xp;
        newNode->assignedTo = assigned;
        newNode->date = date;
        newNode->isVerified = isVerified;

        // check the 'type' to decide which list to put the task in
        if (type == 0) {
            newNode->next = todo_head; // Add to To-Do list
            todo_head = newNode;
        }
        else if (type == 1) {
            newNode->next = in_progress_head; // Add to In-Progress list
            in_progress_head = newNode;
        }
        else {
            newNode->next = done_head; // Add to Done list
            done_head = newNode;
        }
    }
    file.close(); // Close the file when finished
}

string getShameMember() {
    MemberNode* temp = memberHead; // start at the beginning of the list
    string shameName = "";
    int lowestScore = 101; // set initial score higher than max possible 100

    while (temp != NULL) { // loop through every member in the list
        if (temp->percent < lowestScore) {
            lowestScore = temp->percent;
            shameName = temp->name; // save the name of this member
        }
        temp = temp->next; // move to the next member
    }
    // only return the name if the score is really low (below 30)
    if (lowestScore < 30) return shameName;
    return ""; // return empty if no one failed that badly
}
void drawTaskCard(sf::RenderWindow& window, sf::Font& font, TaskNode* task, float x, float y, int listType) {
    sf::Color statusColor;

    // Modern Neon Colors
    if (listType == 0) statusColor = sf::Color(255, 80, 80);      // Red
    else if (listType == 1) statusColor = sf::Color(255, 220, 50); // Yellow
    else {
        if (task->isVerified) statusColor = sf::Color(50, 255, 150); // Green
        else statusColor = sf::Color(255, 140, 0); // Orange
    }

    // Dark Card Background
    sf::RectangleShape box(sf::Vector2f(220, 100));
    box.setPosition(x, y);
    box.setFillColor(sf::Color(50, 55, 65));
    box.setOutlineThickness(2);
    box.setOutlineColor(statusColor);

    sf::Text titleText(task->title, font, 16);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(x + 10, y + 10);

    sf::Text personText((listType == 0 ? "Unassigned" : task->assignedTo), font, 12);
    personText.setFillColor(sf::Color(180, 180, 180));
    personText.setPosition(x + 10, y + 55);

    string statusMsg = "Due: " + task->date;
    if (listType == 2 && !task->isVerified) statusMsg = "Wait Verify...";
    else if (listType == 2 && task->isVerified) statusMsg = "Verified!";

    sf::Text dateText(statusMsg, font, 12);
    dateText.setFillColor(statusColor);
    dateText.setPosition(x + 10, y + 75);

    sf::Text xpText(std::to_string(task->xp) + " XP", font, 14);
    xpText.setStyle(sf::Text::Bold);
    xpText.setFillColor(sf::Color(255, 215, 0));
    xpText.setPosition(x + 150, y + 10);

    window.draw(box);
    window.draw(titleText);
    window.draw(personText);
    window.draw(dateText);
    window.draw(xpText);
}
void drawMembersPanel(sf::RenderWindow& window, sf::Font& font) {
    sf::Text header("THE CREW", font, 22);
    header.setPosition(20, 20);
    header.setStyle(sf::Text::Bold);
    window.draw(header);

    sf::Text labels("Name        Contrib   XP", font, 14);
    labels.setFillColor(sf::Color(180, 180, 180));
    labels.setPosition(20, 60);
    window.draw(labels);

    int y = 90;
    sort_members();
    MemberNode* temp = memberHead;
    while (temp != NULL) {
        sf::Text name(temp->name, font, 18);
        name.setPosition(20, y);
        window.draw(name);

        sf::Text contrib(std::to_string((int)temp->percent) + "%", font, 18);
        contrib.setPosition(130, y);
        if (temp->percent < 30) contrib.setFillColor(sf::Color(255, 107, 107));
        else contrib.setFillColor(sf::Color(100, 221, 155));
        window.draw(contrib);

        sf::Text xp(std::to_string(temp->exp) + " XP", font, 18);
        xp.setPosition(210, y);
        xp.setFillColor(sf::Color(255, 215, 0));
        window.draw(xp);

        y += 40;
        temp = temp->next;
    }
}

void drawRightPanel(sf::RenderWindow& window, sf::Font& font, string currentInput, bool isTyping) {
    // Current User
    sf::Text userLabel("Logged in as: " + currentUser, font, 16);
    userLabel.setPosition(1070, 10);
    userLabel.setFillColor(sf::Color::Cyan);
    window.draw(userLabel);
    sf::Text hint("(Click to switch)", font, 12);
    hint.setPosition(1070, 30);
    hint.setFillColor(sf::Color::White);
    window.draw(hint);

    // Shame Board
    sf::Text shameTitle("SHAME BOARD", font, 22);
    shameTitle.setFillColor(sf::Color(255, 107, 107));
    shameTitle.setPosition(1070, 80);
    window.draw(shameTitle);

    string shameName = getShameMember();
    if (shameName != "") {
        sf::Text sName(shameName, font, 30);
        sName.setPosition(1080, 120);
        sName.setFillColor(sf::Color(255, 107, 107));
        window.draw(sName);
    }
    else {
        sf::Text good("All Good!", font, 16);
        good.setPosition(1080, 120);
        good.setFillColor(sf::Color(100, 221, 155));
        window.draw(good);
    }

    // Leader Controls
    float inputY = 400;
    sf::Text lTitle("LEADER CONTROLS", font, 20);
    lTitle.setPosition(1070, inputY);
    window.draw(lTitle);

    // 4 MODE BUTTONS
    string modes[4] = { "New Task", "Add Mem", "Del Mem", "Del Task" };
    for (int i = 0; i < 4; i++) {
        sf::RectangleShape btn(sf::Vector2f(60, 30));
        btn.setPosition(1070 + (i * 65), inputY + 40);

        if (leaderMode == i) btn.setFillColor(sf::Color(255, 215, 0));
        else btn.setFillColor(sf::Color(100, 100, 100));
        window.draw(btn);

        sf::Text mText(modes[i], font, 10);
        mText.setFillColor(sf::Color::Black);
        mText.setPosition(1072 + (i * 65), inputY + 48);
        window.draw(mText);
    }

    // Input Box
    sf::RectangleShape input1(sf::Vector2f(260, 40));
    input1.setPosition(1070, inputY + 90);
    input1.setOutlineColor(isTyping ? sf::Color::Yellow : sf::Color::White);
    input1.setOutlineThickness(1);
    input1.setFillColor(sf::Color(60, 65, 75));
    window.draw(input1);

    sf::Text userText;
    userText.setFont(font);
    if (currentInput == "") {
        if (leaderMode == 0) userText.setString("Task Name, XP");
        else if (leaderMode == 1) userText.setString("Member Name");
        else if (leaderMode == 2) userText.setString("Name to Remove");
        else userText.setString("Click Card to Delete");
    }
    else userText.setString(currentInput);

    userText.setCharacterSize(14);
    userText.setFillColor(isTyping ? sf::Color::White : sf::Color(150, 150, 150));
    userText.setPosition(1080, inputY + 100);
    window.draw(userText);

    // Execute Button
    sf::RectangleShape addBtn(sf::Vector2f(260, 40));
    addBtn.setPosition(1070, inputY + 140);
    addBtn.setFillColor(sf::Color(100, 221, 155));
    window.draw(addBtn);
    sf::Text btnTx("EXECUTE", font, 16);
    btnTx.setFillColor(sf::Color::Black);
    btnTx.setPosition(1160, inputY + 150);
    window.draw(btnTx);
}
