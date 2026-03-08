#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <vector>
#include <cstdlib>
using namespace std;

const int width = 40;
const int height = 20;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

struct Bullet {
    int x, y;
    bool fromPlayer;
    Bullet(int x, int y, bool fromPlayer) : x(x), y(y), fromPlayer(fromPlayer) {}
};

struct Enemy {
    int x, y;
    int shootCooldown;
    Enemy(int x, int y, int cooldown) : x(x), y(y), shootCooldown(cooldown) {}
};

int playerX, prevPlayerX;
int playerLives = 2;
int score = 0;
int level = 1;
bool gameOver = false;
bool gameWin = false;

bool spaceHeld = false;
int playerBulletCooldown = 0;
const int playerBulletDelay = 5;

vector<Bullet> bullets;
vector<Enemy> enemies;

int enemyMoveDelay = 20; // enemy movement 
int enemyMoveTimer = 0;
int enemyDirection = 1; // 1 = right, -1 = left

void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void gotoxy(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void drawBorders() {
    setColor(3);
    char wallChar = (char)219; //solid-wall
    for (int i = 0; i <= width; i++) {
        gotoxy(i, 0); cout << wallChar;
        gotoxy(i, height); cout << wallChar;
    }
    for (int i = 0; i <= height; i++) {
        gotoxy(0, i); cout << wallChar;
        gotoxy(width, i); cout << wallChar;
    }
    setColor(7);
}

void drawPlayer() {
    setColor(11);
    gotoxy(playerX - 1, height - 3); cout << " /\\ ";
    gotoxy(playerX - 1, height - 2); cout << "<||>";
    setColor(7);
}

void erasePlayer() {
    for (int i = 0; i < 2; i++) {
        for (int j = -1; j <= 2; j++) {
            int xPos = prevPlayerX + j;
            int yPos = height - 3 + i;
            if (xPos == 0 || xPos == width) continue;
            gotoxy(xPos, yPos); cout << " ";
        }
    }
}

void drawEnemy(int x, int y) {
    setColor(10);
    gotoxy(x - 1, y); cout << " ^ ";
    gotoxy(x - 1, y + 1); cout << "[o]";
    setColor(7);
}

void eraseEnemy(int x, int y) {
    gotoxy(x - 1, y); cout << "   ";
    gotoxy(x - 1, y + 1); cout << "   ";
}

void drawEnemies() {
    for (int i = 0; i < (int)enemies.size(); i++)
        drawEnemy(enemies[i].x, enemies[i].y);
}

void eraseEnemies() {
    for (int i = 0; i < (int)enemies.size(); i++)
        eraseEnemy(enemies[i].x, enemies[i].y);
}

void spawnEnemies(int count) {
    enemies.clear();
    vector<int> positions;
    int minSpacing = 5;
    int maxAttempts = 100;

    int attempts = 0;
    while ((int)positions.size() < count && attempts < maxAttempts) {
        int pos = 3 + rand() % (width - 6);
        bool tooClose = false;
        for (int i = 0; i < (int)positions.size(); i++) {
            if (abs(positions[i] - pos) < minSpacing) {
                tooClose = true;
                break;
            }
        }
        if (!tooClose)
            positions.push_back(pos);
        attempts++;
    }

    if ((int)positions.size() < count) {
        int step = width / (count + 1);
        positions.clear();
        for (int i = 1; i <= count; i++) {
            positions.push_back(i * step);
        }
    }

    for (int i = 0; i < count; i++) {
        int yPos = (i % 2 == 0) ? 2 : 4;
        enemies.push_back(Enemy(positions[i], yPos, rand() % 50 + 50)); 
    }
}

void input() {
    if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && playerX > 2)
        playerX--;
    if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && playerX < width - 4)
        playerX++;
    spaceHeld = (GetAsyncKeyState(VK_SPACE) & 0x8000);
}

void handlePlayerShooting() {
    if (spaceHeld && playerBulletCooldown == 0) {
        bullets.push_back(Bullet(playerX, height - 4, true));
        playerBulletCooldown = playerBulletDelay;
    }
    if (playerBulletCooldown > 0)
        playerBulletCooldown--;
}

void drawBullets() {
    for (int i = 0; i < (int)bullets.size(); i++) {
        if (bullets[i].fromPlayer)
            setColor(12);
        else
            setColor(14);
        gotoxy(bullets[i].x, bullets[i].y); cout << "|";
    }
    setColor(7);
}

void eraseBullets() {
    for (int i = 0; i < (int)bullets.size(); i++) {
        gotoxy(bullets[i].x, bullets[i].y); cout << " ";
    }
}

void updateBullets() {
    for (int i = 0; i < (int)bullets.size(); i++) {
        if (bullets[i].fromPlayer)
            bullets[i].y--;
        else
            bullets[i].y++;

        if (bullets[i].y <= 0 || bullets[i].y >= height) {
            bullets.erase(bullets.begin() + i);
            i--;
            continue;
        }

        if (bullets[i].fromPlayer) {
            for (int j = 0; j < (int)enemies.size(); j++) {
                Enemy& e = enemies[j];
                if (bullets[i].y >= e.y && bullets[i].y <= e.y + 1 &&
                    bullets[i].x >= e.x - 1 && bullets[i].x <= e.x + 1) {
                    eraseEnemy(e.x, e.y);
                    enemies.erase(enemies.begin() + j);
                    bullets.erase(bullets.begin() + i);
                    i--;
                    score++;
                    break;
                }
            }
        } else {
            if ((bullets[i].y == height - 3 || bullets[i].y == height - 2) &&
                bullets[i].x >= playerX - 1 && bullets[i].x <= playerX + 2) {
                bullets.erase(bullets.begin() + i);
                playerLives--;
                i--;
                if (playerLives <= 0)
                    gameOver = true;
            }
        }
    }
}

void enemyShoot() {
    for (int i = 0; i < (int)enemies.size(); i++) {
        enemies[i].shootCooldown--;
        if (enemies[i].shootCooldown <= 0) {
            bullets.push_back(Bullet(enemies[i].x, enemies[i].y + 2, false));
            // Fire rate increases with level (shorter cooldown)
            int minCooldown = 20 - (level * 3);
            if (minCooldown < 5) minCooldown = 5;
            enemies[i].shootCooldown = rand() % 30 + minCooldown;
        }
    }
}

void enemyMoveDown() {
    enemyMoveTimer++;
    if (enemyMoveTimer >= enemyMoveDelay) {
        enemyMoveTimer = 0;

        eraseEnemies();

        // Check if enemies hit left or right wall
        bool changeDirection = false;
        for (int i = 0; i < (int)enemies.size(); i++) {
            int nextX = enemies[i].x + enemyDirection;
            if (nextX - 1 <= 1 || nextX + 1 >= width - 1) {
                changeDirection = true;
                break;
            }
        }

        if (changeDirection) {
            enemyDirection = -enemyDirection;
            // Move down on direction change
            for (int i = 0; i < (int)enemies.size(); i++) {
                enemies[i].y++;
                if (enemies[i].y + 1 >= height - 3)
                    gameOver = true;
            }
        } else {
            for (int i = 0; i < (int)enemies.size(); i++) {
                enemies[i].x += enemyDirection;
            }
        }

        drawEnemies();
    }
}

void drawScoreLives() {
    setColor(15);
    gotoxy(2, height + 1);
    cout << "Score: ";
    setColor(14);
    cout << score;

    setColor(15);
    gotoxy(width / 2 - 7, height + 1);
    cout << "Lives: ";
    setColor(12);
    cout << playerLives;

    setColor(15);
    gotoxy(width - 15, height + 1);
    cout << "Level: ";
    setColor(13);
    cout << level;

    setColor(7);

    gotoxy(2, height + 2);
    setColor(9);
    cout << "[Left Arrow][Right Arrow] Move   [SPACE] Shoot";
    setColor(7);
}

void showInstructions() {
    system("cls");
    hideCursor();
    setColor(3);
    gotoxy(10, 3); cout << "+==========================+";
    gotoxy(10, 4); cout << "|      INSTRUCTIONS        |";
    gotoxy(10, 5); cout << "+==========================+";
    setColor(15);
    gotoxy(12, 7); cout << "[Left Arrow][Right Arrow] : Move ship";
    gotoxy(12, 8); cout << "SPACE : Fire bullet";
    gotoxy(12, 9); cout << "Destroy all enemies";
    gotoxy(12,10); cout << "Avoid enemy fire";
    gotoxy(12,11); cout << "3 lives to survive";
    setColor(14);
    gotoxy(10,13); cout << "Press ENTER to start...";
    setColor(7);
    while (_getch() != 13);
    while (_kbhit()) _getch();
}

void showTitleScreen() {
    system("cls");
    hideCursor();
    setColor(3);
    gotoxy(9, 4); cout << "+=======================+";
    gotoxy(9, 5); cout << "|     SPACE SHOOTER     |";
    gotoxy(9, 6); cout << "+=======================+";
    setColor(14);
    gotoxy(9, 8); cout << "Press ENTER to continue";
    setColor(7);
    while (_getch() != 13);
    while (_kbhit()) _getch();
}

int main() {
    srand(time(NULL));
    showTitleScreen();
    showInstructions();
    hideCursor();
    system("cls");
    playerX = width / 2;
    prevPlayerX = playerX;
    playerLives = 3;
    score = 0;
    level = 1;
    gameOver = false;
    gameWin = false;
    bullets.clear();
    drawBorders();
    spawnEnemies(6);

    while (!gameOver && !gameWin) {
        if (playerX != prevPlayerX)
            erasePlayer();
        prevPlayerX = playerX;

        input();
        handlePlayerShooting();

        drawPlayer();
        eraseBullets();
        updateBullets();
        drawBullets();

        enemyShoot();
        enemyMoveDown();

        drawEnemies();
        drawScoreLives();

        if ((int)enemies.size() == 0) {
            if (level == 3) {
                gameWin = true;
            } else {
                level++;
                // Increase difficulty
                if (enemyMoveDelay > 6) enemyMoveDelay -= 4;
                playerBulletCooldown = 0;

                spawnEnemies(6 + level * 2);
                // Give player a small reward: restore 1 life if less than max 3
                if (playerLives < 3)
                    playerLives++;

                bullets.clear();
                system("cls");
                drawBorders();
            }
        }

        Sleep(30);
    }

    system("cls");
    hideCursor();

    string message = gameWin ? " YOU WIN! " : " GAME OVER! ";
    string border(message.length() + 4, '=');
    int msgX = width / 2 - (int(message.length()) / 2);
    int msgY = height / 2;

    setColor(gameWin ? 10 : 12);

    gotoxy(msgX - 2, msgY - 2); cout << border;
    gotoxy(msgX - 2, msgY - 1); cout << "|"; setColor(15); cout << message; setColor(gameWin ? 10 : 12); cout << "|";
    gotoxy(msgX - 2, msgY);     cout << border;

    setColor(14);
    gotoxy(width / 2 - 8, msgY + 2);
    cout << "Final Score: " << score;

    gotoxy(width / 2 - 12, msgY + 4);
    cout << "Press any key to exit...";
    setColor(7);

    _getch();
    return 0;
}

