#pragma once
#include <vector>
#include <list>
#include "structures.h"
#include "constants.h"

void createObstacle(GameLevel level, int score, const std::list<sSnakeSegment>& snake, std::vector<sObstacle>& obstacles);
bool isObstacle(int x, int y, const std::vector<sObstacle>& obstacles);