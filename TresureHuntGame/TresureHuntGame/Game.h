#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Obstacle.h"
#include "Player.h"

class Game {
public:
    Game(int playerStartX, int playerStartY, int targetX, int targetY, const std::vector<Obstacle>& obs);
    void drawBorder();
    void run();

private:
    Player player;
    int targetX, targetY;
    std::vector<Obstacle> obstacles;
    bool gameOver;
};

#endif // GAME_H
