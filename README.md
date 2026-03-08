#  Space Shooter Game (C++)

A **console-based Space Shooter game** developed in **C++** using the Windows Console API.
The player controls a spaceship, shoots enemies, and survives through multiple levels while avoiding enemy bullets.

---

##  Game Features

*  Player spaceship movement
*  Bullet shooting system
*  Enemy AI with random shooting
*  Multiple difficulty levels
*  Player lives system
*  Score tracking
*  Enemy movement similar to classic arcade shooters
*  Colored console graphics

---

##  Controls

| Key           | Action               |
| ------------- | -------------------- |
|  Left Arrow  | Move spaceship left  |
|  Right Arrow | Move spaceship right |
| Space         | Shoot bullets        |

---


##  Game Mechanics

* The player starts with **3 lives**.
* Enemies move horizontally and descend when they hit the wall.
* Enemies shoot bullets at random intervals.
* Destroy all enemies to advance to the next level.
* Difficulty increases each level:

  * Enemies move faster
  * Enemy fire rate increases
  * More enemies spawn
* The game ends when:

  * The player loses all lives, or
  * Enemies reach the player's position.

---

##  Technologies Used

* **C++**
* **Windows Console API**
* **Vectors and Structures**
* **Game Loop**
* **Basic Collision Detection**

Libraries used:

* `<iostream>`
* `<vector>`
* `<windows.h>`
* `<conio.h>`
* `<ctime>`

---

##  Project Structure

```
space-shooter-game
│
├── space_shooter.cpp
└── README.md
```

---

##  How to Run

1. Clone or download the repository.
2. Open the project in **Visual Studio** or any C++ compiler.
3. Compile and run the `.cpp` file.

Example using g++:

```
g++ space_shooter.cpp -o shooter
shooter.exe
```

---

##  Concepts Demonstrated

This project demonstrates several important programming concepts:

* Object-Oriented Programming (Structures)
* Game loop design
* Real-time keyboard input
* Dynamic arrays using vectors
* Basic game physics
* Collision detection

---

##  Future Improvements

Possible enhancements for the game:

* Boss enemy
* Sound effects
* Power-ups
* Pause menu
* High score system
* Better graphics

---

##  Author

**Musa Khurram**

Computer Science Student
C++ Projects Portfolio

---

⭐ If you like this project, feel free to **star the repository**.
