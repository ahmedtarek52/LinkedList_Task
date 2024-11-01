#ifndef OBSTACLES_H
#define OBSTACLES_H

#include <vector>

class Obstacle {
public:
    Obstacle(int x, int y, int len, char dir)
        : startX(x), startY(y), length(len), direction(dir) {}

    int getStartX() const { return startX; }
    int getStartY() const { return startY; }
    int getLength() const { return length; }
    char getDirection() const { return direction; }

    bool isHorizontal() const { return direction == 'H'; }
    bool isVertical() const { return direction == 'V'; }

private:
    int startX;
    int startY;
    int length;
    char direction;
};

class ObstacleLoader {
public:
    static std::vector<Obstacle> loadObstacles() {
        std::vector<Obstacle> obstacles;
        obstacles.emplace_back(4, 3, 10, 'H');
        obstacles.emplace_back(8, 5, 5, 'H');
        obstacles.emplace_back(6, 7, 6, 'H');
        return obstacles;
    }
};

#endif // OBSTACLES_H
