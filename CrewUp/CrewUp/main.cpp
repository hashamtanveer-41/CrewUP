#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;

// ==========================================
// 1. DATA STRUCTURES
// ==========================================

struct MemberNode {
    string name;
    float percent;
    int exp;
    MemberNode* next;

    MemberNode(string val, float p, int e) : name(val), percent(p), exp(e), next(NULL) {}
};

MemberNode* memberHead = NULL;
MemberNode* memberTail = NULL;

struct TaskNode {
    string title;
    string assignedTo;
    string date;
    int xp;
    bool isVerified;
    TaskNode* next;
};

TaskNode* todo_head = NULL;
TaskNode* in_progress_head = NULL;
TaskNode* done_head = NULL;

// Global State
string currentUser = "Guest";
int leaderMode = 0; // 0=Task, 1=AddMem, 2=DelMem, 3=DelTask

// ==========================================
// 2. BACKEND LOGIC
// ==========================================

// --- Member Functions ---
void push_member(string val, float p, int e) {
    MemberNode* newNode = new MemberNode(val, p, e);
    if (memberHead == NULL) {
        memberHead = memberTail = newNode;
    }
    else {
        memberTail->next = newNode;
        memberTail = newNode;
    }
}

void remove_member(string name) {
    MemberNode* current = memberHead;
    MemberNode* prev = NULL;

    while (current != NULL && current->name != name) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) return;

    if (prev == NULL) memberHead = current->next;
    else prev->next = current->next;

    if (current == memberTail) memberTail = prev;
    delete current;
}

int totalTasksXP() {
    int total = 0;
    TaskNode* t = todo_head;
    while (t != NULL) { total += t->xp; t = t->next; }
    t = in_progress_head;
    while (t != NULL) { total += t->xp; t = t->next; }
    t = done_head;
    while (t != NULL) { total += t->xp; t = t->next; }
    return total;
}

void updatePercentages() {
    int total = totalTasksXP();
    MemberNode* temp = memberHead;
    if (total <= 0) {
        while (temp != NULL) { temp->percent = 0.0f; temp = temp->next; }
        return;
    }
    while (temp != NULL) {
        temp->percent = (static_cast<float>(temp->exp) / static_cast<float>(total)) * 100.0f;
        if (temp->percent > 100.0f) temp->percent = 100.0f;
        temp = temp->next;
    }
}

void add_xp(string name, int amount) {
    MemberNode* temp = memberHead;
    while (temp != NULL) {
        if (temp->name == name) {
            temp->exp += amount;
            updatePercentages();
            return;
        }
        temp = temp->next;
    }
}
//to sort the members based on XP in descending order
void sort_members() {
    if (memberHead == NULL || memberHead->next == NULL) return;
    bool swapped;
    MemberNode* ptr1;
    MemberNode* lptr = NULL;

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

void loadMembers() {
    ifstream outputFile("Output.txt");
    if (!outputFile) return;
    string name; int exp; float percentage;
    while (outputFile >> name >> percentage >> exp) {
        push_member(name, percentage, exp);
    }
    outputFile.close();
}

void saveMembers() {
    ofstream outputFile("Output.txt");
    MemberNode* temp = memberHead;
    while (temp != NULL) {
        outputFile << temp->name << " " << temp->percent << " " << temp->exp << endl;
        temp = temp->next;
    }
    outputFile.close();
}

// --- Task Functions ---
void add_task_to_list(TaskNode*& head, string title, int xp, string date) {
    TaskNode* newNode = new TaskNode;
    newNode->title = title;
    newNode->xp = xp;
    newNode->date = date;
    newNode->assignedTo = "Unassigned";
    newNode->isVerified = false;
    newNode->next = head;
    head = newNode;
}

void move_node_logic(TaskNode*& source_head, TaskNode*& dest_head, string task_name, string assigneeName) {
    TaskNode* current = source_head;
    TaskNode* prev = NULL;

    while (current != NULL && current->title != task_name) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) return;

    if (prev == NULL) source_head = current->next;
    else prev->next = current->next;

    if (assigneeName != "") current->assignedTo = assigneeName;

    current->next = dest_head;
    dest_head = current;
}

// NEW: Delete Task Logic (Search and Destroy)
void delete_task_globally(string title) {
    auto deleteFromList = [&](TaskNode*& head) -> bool {
        TaskNode* current = head;
        TaskNode* prev = NULL;
        while (current != NULL) {
            if (current->title == title) {
                if (prev == NULL) head = current->next;
                else prev->next = current->next;
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
        };

    if (!deleteFromList(todo_head)) {
        if (!deleteFromList(in_progress_head)) {
            deleteFromList(done_head);
        }
    }
}

// NEW: Save Tasks Logic
void saveTasks() {
    ofstream file("Tasks.txt");
    auto saveList = [&](TaskNode* head, int type) {
        TaskNode* temp = head;
        while (temp != NULL) {
            file << type << endl << temp->xp << endl << temp->isVerified << endl;
            file << temp->title << endl << temp->assignedTo << endl << temp->date << endl;
            temp = temp->next;
        }
        };
    saveList(todo_head, 0); saveList(in_progress_head, 1); saveList(done_head, 2);
    file.close();
}

// NEW: Load Tasks Logic
void loadTasks() {
    ifstream file("Tasks.txt");
    if (!file) return;
    int type, xp; bool isVerified;
    string title, assigned, date;
    while (file >> type >> xp >> isVerified) {
        file.ignore();
        getline(file, title); getline(file, assigned); getline(file, date);

        TaskNode* newNode = new TaskNode;
        newNode->title = title; newNode->xp = xp; newNode->assignedTo = assigned;
        newNode->date = date; newNode->isVerified = isVerified;

        if (type == 0) { newNode->next = todo_head; todo_head = newNode; }
        else if (type == 1) { newNode->next = in_progress_head; in_progress_head = newNode; }
        else { newNode->next = done_head; done_head = newNode; }
    }
    file.close();
}

string getShameMember() {
    MemberNode* temp = memberHead;
    string shameName = "";
    int lowestScore = 101;
    while (temp != NULL) {
        if (temp->percent < lowestScore) {
            lowestScore = temp->percent;
            shameName = temp->name;
        }
        temp = temp->next;
    }
    if (lowestScore < 30) return shameName;
    return "";
}

// ==========================================
// 3. GUI DRAWING
// ==========================================

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

// ==========================================
// 4. MAIN
// ==========================================

int main() {
    sf::RenderWindow window(sf::VideoMode(1360, 800), "CrewUp", sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/Roboto-Bold.ttf")) {
        return -1;
    }

    loadMembers();
    loadTasks(); // Load saved tasks
    updatePercentages();

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