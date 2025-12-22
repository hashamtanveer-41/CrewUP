# CrewUp 🚀

CrewUp is a gamified task manager App built using C++ and SFML.

In the Group projects the members either dont know their tasks to do or procrastinate them. We wanted a way to manage group projects in a way that are no more procrastinated and every member exacly know what they have to do. The idea is simple: The group Leader add tasks, then team members can then claim any task, and when they get done, that person gets XP after it is verified by an another memberof the group. If someone isn't contributing to then project properly, they end up on the "Shame Board."


It uses a custom GUI (SFML).

![CrewUp_SS](https://github.com/user-attachments/assets/9d1f5b60-cdd6-4512-877f-04ef2c2016a5)

## Features

* **Board:** Three columns for To-Do, In Progress, and Done.
* **XP & Leveling:** Completing tasks grants XP. The more hard work one do, the higher his contribution percentage.
* **The Shame Board:** If a member's contribution drops below 30%, their name moves to red shame board.
* **Leader Controls:**
    * Add new tasks with custom XP values.
    * Add or kick members.
    * **Delete Mode:** A specific mode to click and destroy tasks instantly.
* **Persistence:** Everything saves to `.txt` files (`Tasks.txt` and `Output.txt`) automatically when the app is closed, so that the progress is not loose.

## How it Works


* **Tasks:** There are three separate linked lists (one for each column). When a task is clicked, the node is physically detached from one list and re-linked to the next.
* **Members:** Stored in a linked list and sorted by XP every time the frame updates.

## Controls

The UI is split into three panels:

1. **Left panel:**
   * Shows all the members of the group.

2. **Center Board:**
   * Click a card to move it forward (Todo -> Progress -> Done).
   * *Note:* In the "Done" column, clicking a task verifies it and awards the XP.

3. **Right Panel:**
   * **User Switcher:** Click the name at the top right to switch which user is "logged in."
   * **Leader Controls:** Select a mode (Gold button), type in the input box, and press Execute.
     * `New Task`: Format is "Task Name, XP" (e.g., `Fix Bug, 150`).
     * `Del Task`: Select this, then click any card on the board to delete it.

## Dependencies

You need **SFML** to compile this.

1. Download the SFML SDK.
2. Link the libraries (`sfml-graphics`, `sfml-window`, `sfml-system`).
3. Make sure the `assets/fonts/Roboto-Bold.ttf` file is in your working directory, or the text won't load.


*Built for our university FoCP project to demonstrate the Understanding in C++.*
