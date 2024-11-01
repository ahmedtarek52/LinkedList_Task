#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player(int startX, int startY) : x(startX), y(startY) {}

    int getX() const { return x; }
    int getY() const { return y; }
    void move(int newX, int newY, bool isValidMove) {
        if (isValidMove) {
            x = newX;
            y = newY;
        }
    }

private:
    int x, y;
};

#endif // PLAYER_H
