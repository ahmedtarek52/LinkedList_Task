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
const int HEALTH_THRESHOLD = 20;

enum class Direction { Up, Down, Left, Right };

class SnakeSegment {
public:
    SnakeSegment(float x, float y) {
        shape.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(x, y);
    }

    sf::RectangleShape& getShape()  {
        return shape;
    }

private:
    sf::RectangleShape shape;
};

class Obstacle {
public:
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

    sf::RectangleShape& getShape() {
        return shape;
    }

private:

    sf::RectangleShape shape;
    bool isMoving;
    sf::Vector2f velocity;
};

class Game {
public:
    Game(sf::RenderWindow& window)
        : window(window), font(), speed(INITIAL_SPEED), snakeHealth(0), INITIAL_HEALTH(0), foodCounter(0),
        blueFoodActive(false), yellowFoodActive(false), lastHealthThreshold(HEALTH_THRESHOLD) {
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Error loading font\n";
        }

        setupText();
        loadObstaclesFromMap("map.txt");
        resetFood(food, sf::Color::Red, GRID_SIZE / 2);
        resetGame();
    }

    void run() {
        sf::Clock clock;
        while (window.isOpen()) {
            handleEvents();

            if (clock.getElapsedTime().asMilliseconds() >= 1000 / speed) {
                clock.restart();
                update();
                draw();
            }
        }
    }

private:

    void setupText() {
        speedText.setFont(font);
        speedText.setCharacterSize(24);
        speedText.setFillColor(sf::Color::White);
        speedText.setPosition(20, 20);

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(20, 50);
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
                    obstacles.back().getShape().setFillColor(sf::Color::Magenta);
                }
                else if (cell == 'O') {
                    obstacles.push_back(Obstacle(posX, posY, GRID_SIZE, GRID_SIZE));
                    obstacles.back().getShape().setFillColor(sf::Color::Magenta);
                }
            }
            y++;
        }


         obstacles.push_back(Obstacle(200, 200, GRID_SIZE, GRID_SIZE, true, { 2, 0 }));
         obstacles.push_back(Obstacle(500, 400, GRID_SIZE, GRID_SIZE, true, { 0, 2 }));
    }

    void resetGame() {
        snake.clear();
        snake.emplace_back(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        snakeDirection = Direction::Right;
        speed = INITIAL_SPEED;
        foodCounter = 0;
        yellowFoodActive = false;
        resetFood(food, sf::Color::Red, GRID_SIZE / 2);
        window.setFramerateLimit(speed);
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code);
            }
        }
    }

    void handleKeyPress(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Up && snakeDirection != Direction::Down) snakeDirection = Direction::Up;
        else if (key == sf::Keyboard::Down && snakeDirection != Direction::Up) snakeDirection = Direction::Down;
        else if (key == sf::Keyboard::Left && snakeDirection != Direction::Right) snakeDirection = Direction::Left;
        else if (key == sf::Keyboard::Right && snakeDirection != Direction::Left) snakeDirection = Direction::Right;
    }

    void update() {
        moveSnake();
        handleFoodCollision();
        handleYellowFoodCollision();
        increaseSpeed();
        handleObstacleCollision();
        handleWallCollision();
        checkGameEndConditions();
    }

    void moveSnake() {
        for (int i = snake.size() - 1; i > 0; --i) {
            snake[i].getShape().setPosition(snake[i - 1].getShape().getPosition());
        }
        sf::Vector2f newHeadPosition = snake[0].getShape().getPosition();
        switch (snakeDirection) {
        case Direction::Up:    newHeadPosition.y -= GRID_SIZE; break;
        case Direction::Down:  newHeadPosition.y += GRID_SIZE; break;
        case Direction::Left:  newHeadPosition.x -= GRID_SIZE; break;
        case Direction::Right: newHeadPosition.x += GRID_SIZE; break;
        }
        snake[0].getShape().setPosition(newHeadPosition);
    }

    void handleFoodCollision() {
        if (checkCollision(snake[0].getShape(), food)) {
            handleFoodEaten();
            resetFood(food, blueFoodActive ? sf::Color::Blue : sf::Color::Red, blueFoodActive ? GRID_SIZE : GRID_SIZE / 2);
        }
    }

    void handleYellowFoodCollision() {
        if (yellowFoodActive && checkCollision(snake[0].getShape(), yellowFood)) {
            snakeHealth *= 1.5;
            yellowFoodActive = false;
        }
    }

    void increaseSpeed() {
        if (snakeHealth >= lastHealthThreshold) {
            speed += SPEED_INCREMENT;
            window.setFramerateLimit(speed);
            lastHealthThreshold += HEALTH_THRESHOLD;
        }
    }

    void handleObstacleCollision() {
        for (auto& obstacle : obstacles) {
            if (checkCollision(snake[0].getShape(), obstacle.getShape())) {
                displayEndMessage("Sorry, you lose! Press 'C' to continue or 'R' to retry.");
                return;
            }
        }
    }

    void handleWallCollision() {
        sf::Vector2f headPosition = snake[0].getShape().getPosition();

        if (headPosition.x < 0) {
            headPosition.x = WINDOW_WIDTH - GRID_SIZE; 
        }
        else if (headPosition.x >= WINDOW_WIDTH) {
            headPosition.x = 0; 
        }
        if (headPosition.y < 0) {
            headPosition.y = WINDOW_HEIGHT - GRID_SIZE; 
        }
        else if (headPosition.y >= WINDOW_HEIGHT) {
            headPosition.y = 0; 
        }
        snake[0].getShape().setPosition(headPosition);
    }


    void checkGameEndConditions() {
        if (snakeHealth >= 50) {
            displayEndMessage("Congratulations! You won! Press 'R' to retry.");
            resetGame();
        }
    }

    void draw() {
        window.clear();
        for (auto& segment : snake) {
            window.draw(segment.getShape());
        }

        for (auto& obstacle : obstacles) {
            obstacle.update();
            window.draw(obstacle.getShape());
        }

        window.draw(food);
        if (yellowFoodActive) {
            window.draw(yellowFood);
        }

        // Display speed and score
        speedText.setString("Speed: " + std::to_string(speed));
        scoreText.setString("Score: " + std::to_string(snakeHealth));
        window.draw(speedText);
        window.draw(scoreText);

        window.display();
    }


    void displayEndMessage(const std::string& message) {
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
                        resetGame();
                        return;
                    }
                }
            }
        }
    }

    bool checkCollision(const sf::RectangleShape& rect, const sf::CircleShape& circle) {
        return rect.getGlobalBounds().intersects(circle.getGlobalBounds());
    }

    bool checkCollision(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2) {
        return rect1.getGlobalBounds().intersects(rect2.getGlobalBounds());
    }

    void handleFoodEaten() {
        foodCounter++;
        snakeHealth++;
        sf::Vector2f lastSegmentPosition = snake.back().getShape().getPosition();
        snake.emplace_back(lastSegmentPosition.x, lastSegmentPosition.y);
      

        if (blueFoodActive) {
            speed += SPEED_INCREMENT; 
            window.setFramerateLimit(speed);  
            blueFoodActive = false;  
        }

        if (foodCounter % 5 == 0) {
            blueFoodActive = true;
            resetFood(food, sf::Color::Blue, GRID_SIZE);
        }
        else {
            blueFoodActive = false;
        }

        if (foodCounter % 13 == 0) {
            yellowFoodActive = true;
            resetFood(yellowFood, sf::Color::Yellow, GRID_SIZE);
        }
    }

    void resetFood(sf::CircleShape& food, sf::Color color, float radius) {
        food.setRadius(radius);
        food.setFillColor(color);
        bool validPosition = false;

        while (!validPosition) {
            food.setPosition(rand() % (WINDOW_WIDTH - GRID_SIZE), rand() % (WINDOW_HEIGHT - GRID_SIZE));
            validPosition = true;

            for ( auto& segment : snake) {
                if (checkCollision(segment.getShape(), food)) {
                    validPosition = false;
                    break;
                }
            }

            for ( auto& obstacle : obstacles) {
                if (checkCollision(obstacle.getShape(), food)) {
                    validPosition = false;
                    break;
                }
            }
        }
    }

    sf::RenderWindow& window;
    sf::Font font;
    sf::Text speedText, scoreText;

    std::vector<SnakeSegment> snake;
    std::vector<Obstacle> obstacles;
    Direction snakeDirection;
    sf::CircleShape food, yellowFood;

    int speed, foodCounter, snakeHealth, INITIAL_HEALTH, lastHealthThreshold;
    bool blueFoodActive, yellowFoodActive;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Obstacle Map");
    Game game(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        game.run();

        window.display();
    }

    return 0;
}
