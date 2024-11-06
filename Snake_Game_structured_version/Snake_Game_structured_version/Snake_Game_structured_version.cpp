#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 20;
const int INITIAL_SPEED = 5;
const int SPEED_INCREMENT = 2;
const int HEALTH_THRESHOLD = 10;
enum Direction { Up, Down, Left, Right };

struct SnakeSegment {
    sf::RectangleShape shape;
    SnakeSegment(float x, float y) {
        shape.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(x, y);
    }
};

struct Obstacle {
    sf::RectangleShape shape;
    bool isMoving;
    sf::Vector2f velocity;

    Obstacle(float x, float y, float width, float height, bool moving = false, sf::Vector2f vel = { 0, 0 })
        : isMoving(moving), velocity(vel) {
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color::Magenta);
        shape.setPosition(x, y);
    }

    void update() {
        if (isMoving) {
            shape.move(velocity);
            if (shape.getPosition().x < 0 || shape.getPosition().x + shape.getSize().x > WINDOW_WIDTH)
                velocity.x = -velocity.x;
            if (shape.getPosition().y < 0 || shape.getPosition().y + shape.getSize().y > WINDOW_HEIGHT)
                velocity.y = -velocity.y;
        }
    }
};

// Text for displaying score and speed
sf::Font font;
sf::Text speedText;
sf::Text scoreText;

std::vector<SnakeSegment> snake;
std::vector<Obstacle> obstacles;
Direction snakeDirection = Right;
int snakeHealth = 0;
int INITIAL_HEALTH = 0;
int foodCounter = 0;
bool blueFoodActive = false;
bool yellowFoodActive = false;
unsigned int speed = INITIAL_SPEED;
int lastHealthThreshold = HEALTH_THRESHOLD;

sf::CircleShape food;
sf::CircleShape yellowFood;

void resetFood(sf::CircleShape& foodItem, sf::Color color, int size) {
    foodItem.setRadius(size);
    foodItem.setFillColor(color);

    bool validPosition = false;
    while (!validPosition) {
        foodItem.setPosition((rand() % (WINDOW_WIDTH / GRID_SIZE)) * GRID_SIZE,
            (rand() % (WINDOW_HEIGHT / GRID_SIZE)) * GRID_SIZE);
        validPosition = true;
        for (const auto& obstacle : obstacles) {
            if (obstacle.shape.getGlobalBounds().intersects(foodItem.getGlobalBounds())) {
                validPosition = false;
                break;
            }
        }
    }
}

void loadObstaclesFromMap(const std::string& filename) {
    obstacles.clear();
    std::ifstream file(filename);
    std::string line;
    int y = 0;

    while (std::getline(file, line)) {
        for (int x = 0; x < line.size(); ++x) {
            char cell = line[x];
            float posX = x * GRID_SIZE;
            float posY = y * GRID_SIZE;

            if (cell == '#') {
                obstacles.push_back(Obstacle(posX, posY, GRID_SIZE, GRID_SIZE));
                obstacles.back().shape.setFillColor(sf::Color::Magenta);
            }
            else if (cell == 'O') {
                obstacles.push_back(Obstacle(posX, posY, GRID_SIZE, GRID_SIZE));
                obstacles.back().shape.setFillColor(sf::Color::Magenta);
            }
        }
        y++;
    }

    obstacles.push_back(Obstacle(200, 200, GRID_SIZE, GRID_SIZE, true, { 2, 0 }));
    obstacles.push_back(Obstacle(500, 400, GRID_SIZE, GRID_SIZE, true, { 0, 2 }));
}


// Overload for two RectangleShape objects
bool checkCollision(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2) {
    return rect1.getGlobalBounds().intersects(rect2.getGlobalBounds());
}

// Overload for a RectangleShape and a CircleShape
bool checkCollision(const sf::RectangleShape& rect, const sf::CircleShape& circle) {
    return rect.getGlobalBounds().intersects(circle.getGlobalBounds());
}


void resetGame(sf::RenderWindow& window) {
    snake.clear();
    snake.push_back(SnakeSegment(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
    speed = INITIAL_SPEED;
    foodCounter = 0;
    yellowFoodActive = false;
    resetFood(food, sf::Color::Red, GRID_SIZE / 2);
    window.setFramerateLimit(speed);
}

void moveSnake() {
    for (int i = snake.size() - 1; i > 0; --i) {
        snake[i].shape.setPosition(snake[i - 1].shape.getPosition());
    }

    sf::Vector2f newHeadPosition = snake[0].shape.getPosition();
    switch (snakeDirection) {
    case Up:    newHeadPosition.y -= GRID_SIZE; break;
    case Down:  newHeadPosition.y += GRID_SIZE; break;
    case Left:  newHeadPosition.x -= GRID_SIZE; break;
    case Right: newHeadPosition.x += GRID_SIZE; break;
    }
    snake[0].shape.setPosition(newHeadPosition);
}

void handleFoodCollision() {
    if (checkCollision(snake[0].shape, food)) {
        snakeHealth += blueFoodActive ? 3 : 1;
        INITIAL_HEALTH += blueFoodActive ? 3 : 1;
        foodCounter += blueFoodActive ? 0 : 1;

        blueFoodActive = foodCounter >= 10;
        if (blueFoodActive) foodCounter = 0;

        if ((snakeHealth == 15 || snakeHealth == 40) && !yellowFoodActive) {
            yellowFoodActive = true;
            resetFood(yellowFood, sf::Color::Yellow, GRID_SIZE);
        }

        sf::Vector2f tailPosition = snake.back().shape.getPosition();
        snake.push_back(SnakeSegment(tailPosition.x, tailPosition.y));
        resetFood(food, blueFoodActive ? sf::Color::Blue : sf::Color::Red, blueFoodActive ? GRID_SIZE : GRID_SIZE / 2);
    }
}

void handleYellowFoodCollision() {
    if (yellowFoodActive && checkCollision(snake[0].shape, yellowFood)) {
        snakeHealth *= 1.5;
        yellowFoodActive = false;
    }
}

void increaseSpeed(sf::RenderWindow& window) {
    if (snakeHealth >= lastHealthThreshold) {
        speed += SPEED_INCREMENT;
        window.setFramerateLimit(speed);
        lastHealthThreshold += HEALTH_THRESHOLD;
    }
}

void displayEndMessage(sf::RenderWindow& window, const std::string& message) {
    sf::Text text;
    text.setFont(font);
    text.setString(message);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Green);
    text.setPosition(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 3);

    while (window.isOpen()) {
        window.clear();
        window.draw(text);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::C) return;
                if (event.key.code == sf::Keyboard::R) {
                    snakeHealth = 0; 
                    resetGame(window);
                    return;
                }
            }
        }
    }
}

void checkGameEndConditions(sf::RenderWindow& window) {
    if (INITIAL_HEALTH >= 53) {
        displayEndMessage(window, "Congratulations! You won! Press 'R' to retry.");
        resetGame(window);
    }
}

void handleObstacleCollision(sf::RenderWindow& window) {
    for (auto& obstacle : obstacles) {
        if (checkCollision(snake[0].shape, obstacle.shape)) {
            displayEndMessage(window, "Sorry, you lose! Press 'C' to continue or 'R' to retry.");
            return;
        }
    }
}



void handleWallCollision(sf::RenderWindow& window) {
    sf::Vector2f headPosition = snake[0].shape.getPosition();
    if (headPosition.x < 0 || headPosition.x >= WINDOW_WIDTH || headPosition.y < 0 || headPosition.y >= WINDOW_HEIGHT) {
        displayEndMessage(window, "Sorry, you lose! Press 'C' to continue or 'R' to retry.");
        switch (snakeDirection) {
        case Up:    snakeDirection = Down;   headPosition.y += GRID_SIZE; break;
        case Down:  snakeDirection = Up;     headPosition.y -= GRID_SIZE; break;
        case Left:  snakeDirection = Right;  headPosition.x += GRID_SIZE; break;
        case Right: snakeDirection = Left;   headPosition.x -= GRID_SIZE; break;
        }
        snake[0].shape.setPosition(headPosition);
    }
}
int main() {
    srand(static_cast<unsigned>(time(0)));
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game with Obstacles");
    window.setFramerateLimit(speed);

    // Load font
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    // Setup text for speed and score
    speedText.setFont(font);
    speedText.setCharacterSize(24);
    speedText.setFillColor(sf::Color::White);
    speedText.setPosition(20, 20);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 50);

    // Load obstacles
    loadObstaclesFromMap("map.txt");

    resetFood(food, sf::Color::Red, GRID_SIZE / 2);
    resetGame(window);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up && snakeDirection != Down) snakeDirection = Up;
                else if (event.key.code == sf::Keyboard::Down && snakeDirection != Up) snakeDirection = Down;
                else if (event.key.code == sf::Keyboard::Left && snakeDirection != Right) snakeDirection = Left;
                else if (event.key.code == sf::Keyboard::Right && snakeDirection != Left) snakeDirection = Right;
            }
        }

        if (clock.getElapsedTime().asMilliseconds() >= 1000 / speed) {
            clock.restart();

            moveSnake();
            handleFoodCollision();
            handleYellowFoodCollision();
            increaseSpeed(window);
            handleObstacleCollision(window);
            handleWallCollision(window);
            checkGameEndConditions(window);

            window.clear();
            for (const auto& segment : snake) window.draw(segment.shape);
            for (auto& obstacle : obstacles) {
                obstacle.update();
                window.draw(obstacle.shape);
            }
            window.draw(food);
            if (yellowFoodActive) window.draw(yellowFood);
            speedText.setString("Speed: " + std::to_string(speed));
            scoreText.setString("Score: " + std::to_string(snakeHealth));
            window.draw(speedText);
            window.draw(scoreText);
            window.display();
        }
    }
    return 0;
}
