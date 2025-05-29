#pragma once
#include "constants.h"

class Menu {
private:
    int highScores[3];
    void saveHighScores();
    void loadHighScores();
    void showHighScores();

public:
    Menu();
    GameLevel showMainMenu();
    void updateHighScore(GameLevel level, int score);
    int getHighScore(GameLevel level) const;
};