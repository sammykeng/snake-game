#pragma once
#include "constants.h"

struct sSnakeSegment {
    int x;
    int y;
};

struct sObstacle {
    int x;
    int y;
    int length;
    bool isHorizontal;
};
