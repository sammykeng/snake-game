#include "game.h"
#include <ncurses.h>
#include <chrono>
#include <thread>
#include <locale.h>
#include "obstacle.h"

// Thêm hàm để lấy ký tự tương ứng với hướng di chuyển
wchar_t getDirectionChar(Direction dir, bool isHead) {
    if (isHead) {
        switch (dir) {
            case UP: return L'o';
            case RIGHT: return L'o';
            case DOWN: return L'o';
            case LEFT: return L'o';
            default: return L'O';
        }
    } else {
        return L'o';  // Thân rắn
    }
}

Game::Game(GameLevel level) : 
    nScore(0),
    nSnakeDirection(3),
    bDead(false),
    nFoodX(30),
    nFoodY(15),
    currentLevel(level),
    previousScore(0) {
    
    // Khởi tạo rắn với độ dài cố định là 8
    for(int i = 60; i < 68; i++) {
        snake.push_back({i, 15});
    }
}

void Game::run() {
    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) {
        screen[i] = ' ';
    }

    while (!bDead) {
        auto t1 = std::chrono::system_clock::now();
        int currentSpeed = calculateSpeed();
        
        if (nScore > previousScore) {
            createObstacle(currentLevel, nScore, snake, obstacles);
            previousScore = nScore;
        }

        // Xử lý input với timeout
        timeout(currentSpeed);
        int ch = getch();
        switch (ch) {
            case 'q':
                bDead = true;
                break;
            case KEY_LEFT:
                if (nSnakeDirection != 1) nSnakeDirection = 3;
                break;
            case KEY_RIGHT:
                if (nSnakeDirection != 3) nSnakeDirection = 1;
                break;
            case KEY_UP:
                if (nSnakeDirection != 2) nSnakeDirection = 0;
                break;
            case KEY_DOWN:
                if (nSnakeDirection != 0) nSnakeDirection = 2;
                break;
        }

        // Di chuyển rắn
        sSnakeSegment newHead = snake.front();
        
        switch (nSnakeDirection) {
            case 0: newHead.y--; break; // UP
            case 1: newHead.x++; break; // RIGHT
            case 2: newHead.y++; break; // DOWN
            case 3: newHead.x--; break; // LEFT
        }

        // Wrap around screen
        if (newHead.x < 0) newHead.x = nScreenWidth - 2;
        if (newHead.x >= nScreenWidth) newHead.x = 1;
        if (newHead.y < 3) newHead.y = nScreenHeight - 2;
        if (newHead.y >= nScreenHeight) newHead.y = 3;

        // Kiểm tra va chạm với thân rắn
        for (const auto& segment : snake) {
            if (segment.x == newHead.x && segment.y == newHead.y) {
                bDead = true;
                break;
            }
        }

        // Kiểm tra va chạm với chướng ngại vật
        if (isObstacle(newHead.x, newHead.y, obstacles)) {
            bDead = true;
            break;
        }

        if (!bDead) {
            // Thêm đầu mới
            snake.push_front(newHead);

            // Kiểm tra ăn thức ăn
            if (newHead.x == nFoodX && newHead.y == nFoodY) {
                nScore++;
                generateFood();
                // Thêm 1 đoạn thân mới khi ăn được điểm
                snake.push_back(snake.back());
            } else {
                // Xóa đuôi nếu không ăn thức ăn
                snake.pop_back();
            }
        }

        // Vẽ game
        drawGame(screen);

        // Delay
        std::this_thread::sleep_until(t1 + std::chrono::milliseconds(currentSpeed));
    }

    delete[] screen;
}

void Game::drawGame(wchar_t* screen) {
    // Clear screen
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) {
        screen[i] = ' ';
    }

    // Vẽ viền trên và dưới
    for (int i = 0; i < nScreenWidth; i++) {
        screen[i] = L'-';  // Viền trên
        screen[(nScreenHeight - 1) * nScreenWidth + i] = L'-';  // Viền dưới
    }

    // Vẽ viền trái và phải
    for (int i = 0; i < nScreenHeight; i++) {
        screen[i * nScreenWidth] = L'|';  // Viền trái
        screen[i * nScreenWidth + (nScreenWidth - 1)] = L'|';  // Viền phải
    }

    // Vẽ góc
    screen[0] = L'+';  // Góc trên trái
    screen[nScreenWidth - 1] = L'+';  // Góc trên phải
    screen[(nScreenHeight - 1) * nScreenWidth] = L'+';  // Góc dưới trái
    screen[(nScreenHeight - 1) * nScreenWidth + (nScreenWidth - 1)] = L'+';  // Góc dưới phải

    // Vẽ dòng chữ Snake Game và Score
    std::string scoreStr = "Snake Game   SCORE: " + std::to_string(nScore);
    for (size_t i = 0; i < scoreStr.size(); i++) {
        screen[1 * nScreenWidth + 5 + i] = scoreStr[i];
    }

    // Vẽ viền dưới dòng chữ Snake Game và Score
    for (int i = 0; i < nScreenWidth; i++) {
        screen[2 * nScreenWidth + i] = L'-';  // Viền dưới dòng chữ
    }

    // Vẽ rắn
    for(auto it = snake.begin(); it != snake.end(); ++it) {
        if (it == snake.begin()) {
            // Vẽ đầu rắn
            Direction headDir;
            if (nSnakeDirection == 0) headDir = UP;
            else if (nSnakeDirection == 1) headDir = RIGHT;
            else if (nSnakeDirection == 2) headDir = DOWN;
            else headDir = LEFT;
            
            screen[it->y * nScreenWidth + it->x] = bDead ? L'×' : getDirectionChar(headDir, true);
        } else {
            // Vẽ thân rắn
            auto prev = std::prev(it);
            Direction bodyDir;
            
            // Xác định hướng của thân dựa trên vị trí tương đối với phần trước
            if (prev->x == it->x) {
                bodyDir = (prev->y < it->y) ? UP : DOWN;
            } else {
                bodyDir = (prev->x < it->x) ? LEFT : RIGHT;
            }
            
            screen[it->y * nScreenWidth + it->x] = bDead ? L'×' : getDirectionChar(bodyDir, false);
        }
    }

    // Vẽ thức ăn
    screen[nFoodY * nScreenWidth + nFoodX] = L'*';

    // Vẽ chướng ngại vật
    for (const auto& obs : obstacles) {
        for (int i = 0; i < obs.length; i++) {
            int x = obs.isHorizontal ? obs.x + i : obs.x;
            int y = obs.isHorizontal ? obs.y : obs.y + i;
            screen[y * nScreenWidth + x] = L'#';
        }
    }

    // Hiển thị game over nếu thua
    if (bDead) {
        std::string gameOverStr = "PRESS 'SPACE' TO PLAY AGAIN";
        for (size_t i = 0; i < gameOverStr.size(); i++) {
            screen[(nScreenHeight / 2) * nScreenWidth + (nScreenWidth - gameOverStr.size()) / 2 + i] = gameOverStr[i];
        }
    }

    // Vẽ màn hình với màu sắc
    for (int y = 0; y < nScreenHeight; y++) {
        for (int x = 0; x < nScreenWidth; x++) {
            wchar_t ch = screen[y * nScreenWidth + x];
            if (ch == L'O' || ch == L'o') {
                attron(COLOR_PAIR(1));
                mvaddch(y, x, ch);
                attroff(COLOR_PAIR(1));
            } else if (ch == L'*') {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, ch);
                attroff(COLOR_PAIR(2));
            } else if (ch == L'-' || ch == L'|' || ch == L'+') {
                attron(COLOR_PAIR(3));
                mvaddch(y, x, ch);
                attroff(COLOR_PAIR(3));
            } else if (ch == L'#') {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, ch);
                attroff(COLOR_PAIR(2));
            } else if (y == 1 && x >= 5 && x < static_cast<int>(5 + scoreStr.size())) {
                attron(COLOR_PAIR(5));
                mvaddch(y, x, ch);
                attroff(COLOR_PAIR(5));
            } else {
                mvaddch(y, x, ch);
            }
        }
    }

    refresh();
}

void Game::generateFood() {
    bool validPosition;
    do {
        validPosition = true;
        nFoodX = rand() % (nScreenWidth - 2) + 1;
        nFoodY = rand() % (nScreenHeight - 4) + 3;

        // Kiểm tra không trùng với thân rắn
        for (const auto& segment : snake) {
            if (segment.x == nFoodX && segment.y == nFoodY) {
                validPosition = false;
                break;
            }
        }

        // Kiểm tra không trùng với chướng ngại vật
        if (validPosition) {
            for (const auto& obs : obstacles) {
                for (int i = 0; i < obs.length; i++) {
                    int x = obs.isHorizontal ? obs.x + i : obs.x;
                    int y = obs.isHorizontal ? obs.y : obs.y + i;
                    if (x == nFoodX && y == nFoodY) {
                        validPosition = false;
                        break;
                    }
                }
                if (!validPosition) break;
            }
        }
    } while (!validPosition);
}

int Game::calculateSpeed() {
    int baseSpeed = 100;
    switch (currentLevel) {
        case EASY:
            return baseSpeed;
        case HARD:
            return baseSpeed - 40;
        case SUPER_HARD:
            return baseSpeed - 60;
        default:
            return baseSpeed;
    }
}