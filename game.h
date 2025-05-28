#pragma once
#include <list>
#include <vector>
#include "structures.h"
#include "constants.h"

class Game {
private:
    std::list<sSnakeSegment> snake;
    std::vector<sObstacle> obstacles;
    int nScore;
    int nSnakeDirection;
    bool bDead;
    int nFoodX;
    int nFoodY;
    GameLevel currentLevel;
    int previousScore;

    void updateSnake();
    void checkCollision();
    void generateFood();
    void drawGame(wchar_t* screen);
    int calculateSpeed();

public:
    Game(GameLevel level);
    void run();
    int getScore() const { return nScore; }
    bool isDead() const { return bDead; }
};