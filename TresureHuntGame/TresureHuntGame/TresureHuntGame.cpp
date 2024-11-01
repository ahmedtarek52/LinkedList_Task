#include "Game.h"
#include "Obstacle.h"
#include <vector>

int main() {
    // Initial positions for player and target
    int playerStartX = 1, playerStartY = 1;
    int targetX = 18, targetY = 8;

    // Load obstacles using your existing function or directly add them
    std::vector<Obstacle> obstacles = ObstacleLoader::loadObstacles();

    // Initialize the Game object
    Game game(playerStartX, playerStartY, targetX, targetY, obstacles);

    // Run the game
    game.run();

    return 0;
}

