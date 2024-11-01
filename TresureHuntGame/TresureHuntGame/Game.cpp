#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Game.h"

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 10;

Game::Game(int playerStartX, int playerStartY, int targetX, int targetY, const std::vector<Obstacle>& obs)
    : player(playerStartX, playerStartY), targetX(targetX), targetY(targetY), obstacles(obs), gameOver(false) {}

void Game::drawBorder() {
    system("CLS");
    for (int i = 0; i <= HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j++) {
            if (i == 0 || i == HEIGHT || j == 0 || j == WIDTH) {
                cout << "*";
            }
            else if (i == player.getY() && j == player.getX()) {
                cout << "X";
            }
            else if (i == targetY && j == targetX) {
                cout << "Y";
            }
            else {
                bool isObstacle = false;
                for (const auto& obs : obstacles) {
                    if (obs.isHorizontal() && i == obs.getStartY() && j >= obs.getStartX() && j < obs.getStartX() + obs.getLength()) {
                        cout << "-";
                        isObstacle = true;
                        break;
                    }
                }
                if (!isObstacle) cout << " ";
            }
        }
        cout << endl;
    }
}

void Game::run() {
    drawBorder();
    while (!gameOver) {
        if (_kbhit()) {
            int newX = player.getX(), newY = player.getY();
            switch (_getch()) {
            case 72: newY--; break;  // Up arrow
            case 80: newY++; break;  // Down arrow
            case 75: newX--; break;  // Left arrow
            case 77: newX++; break;  // Right arrow
            }

            bool isValidMove = newX > 0 && newX < WIDTH && newY > 0 && newY < HEIGHT;
            for (const auto& obs : obstacles) {
                if (obs.isHorizontal() && newY == obs.getStartY() && newX >= obs.getStartX() && newX < obs.getStartX() + obs.getLength()) {
                    isValidMove = false;
                    break;
                }
            }
            player.move(newX, newY, isValidMove);
            drawBorder();

            if (player.getX() == targetX && player.getY() == targetY) {
                gameOver = true;
                cout << "X has reached Y! Game over.\n";
            }
            Sleep(100);
        }
    }
}