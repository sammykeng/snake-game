#include "obstacle.h"
#include <cstdlib>

void createObstacle(GameLevel level, int score, const std::list<sSnakeSegment>& snake, std::vector<sObstacle>& obstacles) {
    // Xác định điểm bắt đầu spawn chướng ngại vật cho mỗi level
    int startScore;
    switch (level) {
        case EASY:
            startScore = 7;
            break;
        case HARD:
            startScore = 5;
            break;
        case SUPER_HARD:
            startScore = 3;
            break;
        default:
            startScore = 7;
    }

    // Nếu điểm số chưa đạt ngưỡng, không tạo chướng ngại vật
    if (score < startScore) {
        return;
    }

    // Tính số lượng chướng ngại vật cần tạo
    int numObstacles = score - startScore + 1;
    
    // Tạo chướng ngại vật mới
    sObstacle newObs;
    int minLength, maxLength;
    
    // Xác định độ dài của chướng ngại vật dựa trên level
    switch (level) {
        case EASY:
            minLength = 3;
            maxLength = 5;
            break;
        case HARD:
            minLength = 4;
            maxLength = 6;
            break;
        case SUPER_HARD:
            minLength = 5;
            maxLength = 8;
            break;
    }
    
    newObs.isHorizontal = (rand() % 2 == 0);
    // Nếu là chướng ngại vật ngang, đặt độ dài cố định là 4
    if (newObs.isHorizontal) {
        newObs.length = 4;
    } else {
        newObs.length = minLength + (rand() % (maxLength - minLength + 1));
    }
    
    // Tạo vị trí ngẫu nhiên cho chướng ngại vật
    if (newObs.isHorizontal) {
        newObs.x = 2 + (rand() % (nScreenWidth - newObs.length - 4));
        newObs.y = 3 + (rand() % (nScreenHeight - 5));
    } else {
        newObs.x = 2 + (rand() % (nScreenWidth - 4));
        newObs.y = 3 + (rand() % (nScreenHeight - newObs.length - 3));
    }
    
    // Kiểm tra xem chướng ngại vật có trùng với thức ăn hoặc rắn không
    bool isValid = true;
    for (int i = 0; i < newObs.length; i++) {
        int checkX = newObs.isHorizontal ? newObs.x + i : newObs.x;
        int checkY = newObs.isHorizontal ? newObs.y : newObs.y + i;
        
        for (const auto& segment : snake) {
            if (checkX == segment.x && checkY == segment.y) {
                isValid = false;
                break;
            }
        }
    }
    
    if (isValid) {
        obstacles.push_back(newObs);
    }
}

bool isObstacle(int x, int y, const std::vector<sObstacle>& obstacles) {
    for (const auto& obs : obstacles) {
        if (obs.isHorizontal) {
            if (y == obs.y && x >= obs.x && x < obs.x + obs.length) {
                return true;
            }
        } else {
            if (x == obs.x && y >= obs.y && y < obs.y + obs.length) {
                return true;
            }
        }
    }
    return false;
}