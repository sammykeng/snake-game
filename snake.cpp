#include <iostream>
#include <list>
#include <thread>
#include <chrono>
#include <ncurses.h>
#include <locale.h>
#include <vector>
#include <cstdio>


int nScreenWidth = 120;
int nScreenHeight = 30;

struct sSnakeSegment
{
    int x;
    int y;
};

// Thêm enum cho các level
enum GameLevel {
    EASY,
    HARD,
    SUPER_HARD
};

// Thêm biến lưu điểm cao nhất cho mỗi level
int highScores[3] = {0, 0, 0}; // [EASY, HARD, SUPER_HARD]

// Thêm enum cho các menu item
enum MenuItem {
    PLAY,
    RECORD,
    QUIT
};

// Thêm enum cho hướng di chuyển
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// Thêm struct cho chướng ngại vật
struct sObstacle {
    int x;
    int y;
    int length;
    bool isHorizontal;
};

// Thêm vector để lưu trữ các chướng ngại vật
std::vector<sObstacle> obstacles;

// Biến toàn cục cho thức ăn
int nFoodX = 30;
int nFoodY = 15;

// Hàm kiểm tra xem một vị trí có phải là chướng ngại vật không
bool isObstacle(int x, int y) {
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

// Hàm tạo chướng ngại vật mới
void createObstacle(GameLevel level, int score, const std::list<sSnakeSegment>& snake) {
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
        
        if (checkX == nFoodX && checkY == nFoodY) {
            isValid = false;
            break;
        }
        
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

// Thêm hàm để lấy ký tự tương ứng với hướng di chuyển
wchar_t getDirectionChar(Direction dir, bool isHead) {
    if (isHead) {
        return L'O';  // Đầu rắn
    } else {
        return L'o';  // Thân rắn
    }
}

// Hàm hiển thị bảng điểm cao
void showHighScores() {
    clear();
    mvprintw(10, 40, "HIGH SCORES");
    mvprintw(12, 35, "Easy Level: %d", highScores[EASY]);
    mvprintw(13, 35, "Hard Level: %d", highScores[HARD]);
    mvprintw(14, 35, "Super Hard Level: %d", highScores[SUPER_HARD]);
    mvprintw(16, 35, "Press M to return to menu");
    refresh();

    while (true) {
        int ch = getch();
        if (ch == 'm' || ch == 'M') {
            return;
        }
    }
}

// Hàm hiển thị menu
GameLevel showMenu() {
    MenuItem currentSelection = PLAY;
    
    while (true) {
        clear(); // Xóa màn hình trước khi vẽ lại menu
        
        // Vẽ tiêu đề game
        attron(COLOR_PAIR(4));  // Màu cyan cho menu
        mvprintw(nScreenHeight/2 - 2, nScreenWidth/2 - 5, "SNAKE GAME");
        
        // Vẽ các menu item
        mvprintw(nScreenHeight/2 + 1, nScreenWidth/2 - 2, "PLAY");
        mvprintw(nScreenHeight/2 + 3, nScreenWidth/2 - 2, "RECORD");
        mvprintw(nScreenHeight/2 + 5, nScreenWidth/2 - 2, "QUIT");
        
        // Vẽ dấu > và đường viền cho item được chọn
        switch (currentSelection) {
            case PLAY:
                mvprintw(nScreenHeight/2 + 1, nScreenWidth/2 + 3, "<");
                for (int i = 0; i < 4; i++) {
                    mvprintw(nScreenHeight/2 + 2, nScreenWidth/2 - 2 + i, "-");
                }
                break;
            case RECORD:
                mvprintw(nScreenHeight/2 + 3, nScreenWidth/2 + 5, "<");
                for (int i = 0; i < 6; i++) {
                    mvprintw(nScreenHeight/2 + 4, nScreenWidth/2 - 2 + i, "-");
                }
                break;
            case QUIT:
                mvprintw(nScreenHeight/2 + 5, nScreenWidth/2 + 3, "<");
                for (int i = 0; i < 4; i++) {
                    mvprintw(nScreenHeight/2 + 6, nScreenWidth/2 - 2 + i, "-");
                }
                break;
        }
        attroff(COLOR_PAIR(4));
        
        refresh();

        // Xử lý input
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (currentSelection > PLAY) {
                    currentSelection = (MenuItem)(currentSelection - 1);
                }
                break;
            case KEY_DOWN:
                if (currentSelection < QUIT) {
                    currentSelection = (MenuItem)(currentSelection + 1);
                }
                break;
            case '\n': // Enter
                switch (currentSelection) {
                    case PLAY:
                        clear();
                        // Hiển thị menu chọn level
                        mvprintw(10, 40, "SELECT LEVEL");
                        mvprintw(12, 35, "1. Easy Level");
                        mvprintw(13, 35, "2. Hard Level");
                        mvprintw(14, 35, "3. Super Hard Level");
                        mvprintw(15, 35, "M. Menu");
                        refresh();

                        while (true) {
                            int choice = getch();
                            switch (choice) {
                                case '1': return EASY;
                                case '2': return HARD;
                                case '3': return SUPER_HARD;
                                case 'm':
                                case 'M': 
                                    return showMenu();
                            }
                        }
                        break;
                    case RECORD:
                        showHighScores();
                        return showMenu();
                    case QUIT:
                        endwin();
                        exit(0);
                }
                break;
        }
    }
}

// Hàm tính toán tốc độ dựa trên level và điểm số
int calculateSpeed(GameLevel level, int score) {
    int baseSpeed;
    int minSpeed;
    int speedDecrease;
    
    switch (level) {
        case EASY:
            baseSpeed = 80;   // Tốc độ ban đầu
            minSpeed = 40;    // Tốc độ tối thiểu
            speedDecrease = 2; // Giảm 2ms mỗi điểm
            break;
        case HARD:
            baseSpeed = 50;   // Tốc độ ban đầu
            minSpeed = 15;    // Tốc độ tối thiểu
            speedDecrease = 2; // Giảm 2ms mỗi điểm
            break;
        case SUPER_HARD:
            baseSpeed = 30;   // Tốc độ ban đầu
            minSpeed = 5;     // Tốc độ tối thiểu
            speedDecrease = 3; // Giảm 3ms mỗi điểm
            break;
        default:
            baseSpeed = 80;
            minSpeed = 40;
            speedDecrease = 2;
    }
    
    // Tính toán tốc độ mới dựa trên điểm số
    int newSpeed = baseSpeed - (score * speedDecrease);
    if (newSpeed < minSpeed) {
        newSpeed = minSpeed;
    }
    
    timeout(newSpeed);
    return newSpeed;
}

// Thêm hàm để lưu điểm cao vào file
void saveHighScores() {
    FILE* file = fopen("../highscores.dat", "wb");  // Sử dụng đường dẫn tương đối từ thư mục snakegame
    if (file != nullptr) {
        fwrite(highScores, sizeof(int), 3, file);
        fclose(file);
    }
}

// Thêm hàm để đọc điểm cao từ file
void loadHighScores() {
    FILE* file = fopen("../highscores.dat", "rb");  // Sử dụng đường dẫn tương đối từ thư mục snakegame
    if (file != nullptr) {
        fread(highScores, sizeof(int), 3, file);
        fclose(file);
    }
}

int main()
{
    setlocale(LC_ALL, "en_US.UTF-8");
    initscr();
    
    // Thêm dòng này để bật chế độ Unicode
    setlocale(LC_ALL, "");
    
    // Thêm dòng này để bật chế độ wide character
    set_escdelay(25);
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    // Thêm các cài đặt này
    use_default_colors();
    start_color();
    
    // Khởi tạo màu sắc với màu nền rõ ràng
    init_pair(1, COLOR_GREEN, COLOR_BLACK);    // Màu cho rắn
    init_pair(2, COLOR_RED, COLOR_BLACK);      // Màu cho thức ăn
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);   // Màu cho viền
    init_pair(4, COLOR_CYAN, COLOR_BLACK);     // Màu cho menu
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);  // Màu cho điểm số

    // Thêm dòng này để đảm bảo terminal hiển thị đúng màu
    bkgd(COLOR_PAIR(0));

    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
    {
        screen[i] = L' ';
    }

    // Thêm dòng này sau khi khởi tạo highScores
    loadHighScores();  // Đọc điểm cao từ file khi khởi động
    
    while (1)
    {
        // Hiển thị menu và lấy level được chọn
        GameLevel currentLevel = showMenu();
        
        std::list<sSnakeSegment> snake = {{60,15},{61,15},{62,15},{63,15},{64,15},{65,15},{66,15},{67,15},{68,15},{69,15}};
        int nScore = 0;
        int nSnakeDirection = 3;
        bool bDead = false;
        obstacles.clear(); // Xóa tất cả chướng ngại vật khi bắt đầu game mới
        int previousScore = 0;  // Thêm biến này vào đầu vòng lặp game

        while (!bDead)
        {
            auto t1 = std::chrono::system_clock::now();
            int currentSpeed = calculateSpeed(currentLevel, nScore);
            
            // Kiểm tra và tạo chướng ngại vật mới dựa trên điểm số
            if (nScore > previousScore) {
                createObstacle(currentLevel, nScore, snake);
                previousScore = nScore;
            }
            
            while ((std::chrono::system_clock::now() - t1) < std::chrono::milliseconds(currentSpeed))
            {
                int ch = getch();
                switch (ch)
                {
                    case 'q':
                        endwin();
                        return 0;
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

                switch (nSnakeDirection)
                {
                case 0: 
                    {
                        sSnakeSegment newSegment;
                        newSegment.x = snake.front().x;
                        newSegment.y = snake.front().y - 1;
                        snake.push_front(newSegment);
                    }
                    break;
                case 1: 
                    {
                        sSnakeSegment newSegment;
                        newSegment.x = snake.front().x + 1;
                        newSegment.y = snake.front().y;
                        snake.push_front(newSegment);
                    }
                    break;
                case 2: 
                    {
                        sSnakeSegment newSegment;
                        newSegment.x = snake.front().x;
                        newSegment.y = snake.front().y + 1;
                        snake.push_front(newSegment);
                    }
                    break;
                case 3: 
                    {
                        sSnakeSegment newSegment;
                        newSegment.x = snake.front().x - 1;
                        newSegment.y = snake.front().y;
                        snake.push_front(newSegment);
                    }
                    break;
                default:
                    break;
                }

                if (snake.front().x == nFoodX && snake.front().y == nFoodY)
                {
                    nScore++;
                    while (screen[nFoodY * nScreenWidth + nFoodX] != ' ')
                    {
                        nFoodX = rand() % nScreenWidth;
                        nFoodY = (rand() % (nScreenHeight - 3)) + 3;
                    }

                    for (int i = 0; i < 5; i++)
                    {
                        snake.push_back({snake.back().x, snake.back().y});
                    }
                }
            
                if (snake.front().x < 0) {
                    snake.front().x = nScreenWidth - 2;
                }
                else if (snake.front().x >= nScreenWidth) {
                    snake.front().x = 1;
                }
                
                if (snake.front().y < 3) {
                    snake.front().y = nScreenHeight - 2;
                }
                else if (snake.front().y >= nScreenHeight) {
                    snake.front().y = 3;
                }

                // Kiểm tra va chạm với chướng ngại vật
                if (isObstacle(snake.front().x, snake.front().y)) {
                    bDead = true;
                }

                for (std::list<sSnakeSegment>::iterator i = snake.begin(); i != snake.end(); i++)
                {
                    if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
                    {
                        bDead = true;
                    }
                }

                snake.pop_back();

                for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
                {
                    screen[i] = L' ';
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
                for (size_t i = 0; i < scoreStr.size(); i++)
                {
                    screen[1 * nScreenWidth + 5 + i] = scoreStr[i];
                }

                // Vẽ viền dưới dòng chữ Snake Game và Score
                for (int i = 0; i < nScreenWidth; i++) {
                    screen[2 * nScreenWidth + i] = L'-';  // Viền dưới dòng chữ
                }

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
                
                screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'×' : L'◌';

                screen[nFoodY * nScreenWidth + nFoodX] = L'*';  // Thay $ bằng *

                // Vẽ chướng ngại vật
                for (const auto& obs : obstacles) {
                    for (int i = 0; i < obs.length; i++) {
                        int x = obs.isHorizontal ? obs.x + i : obs.x;
                        int y = obs.isHorizontal ? obs.y : obs.y + i;
                        screen[y * nScreenWidth + x] = L'#';
                    }
                }

                if (bDead)
                {
                    std::string gameOverStr = "PRESS 'SPACE' TO PLAY AGAIN";
                    for (size_t i = 0; i < gameOverStr.size(); i++)
                    {
                        screen[(nScreenHeight / 2) * nScreenWidth + (nScreenWidth - gameOverStr.size()) / 2 + i] = gameOverStr[i];
                    }
                }

                for (int y = 0; y < nScreenHeight; y++)
                {
                    for (int x = 0; x < nScreenWidth; x++)
                    {
                        wchar_t ch = screen[y * nScreenWidth + x];
                        if (ch == L'◌' || ch == L'o') {
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
                            attron(COLOR_PAIR(2)); // Sử dụng màu đỏ cho chướng ngại vật
                            mvaddch(y, x, ch);
                            attroff(COLOR_PAIR(2));
                        } else if (y == 1 && x >= 5 && x < 5 + scoreStr.size()) {
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
        }

        // Hiển thị màn hình game over
        clear();
        mvprintw(nScreenHeight/2, nScreenWidth/2 - 10, "GAME OVER!");
        mvprintw(nScreenHeight/2 + 1, nScreenWidth/2 - 15, "Final Score: %d", nScore);
        mvprintw(nScreenHeight/2 + 2, nScreenWidth/2 - 20, "Press SPACE to return to menu");
        refresh();
        
        while (getch() != ' ');

        // Thêm dòng này trước khi cập nhật điểm cao
        if (nScore > highScores[currentLevel]) {
            highScores[currentLevel] = nScore;
            saveHighScores();  // Lưu điểm cao vào file mỗi khi có điểm mới
        }
    }

    // Thêm dòng này trước khi kết thúc chương trình
    saveHighScores();  // Lưu điểm cao vào file khi tắt chương trình
    endwin();
    return 0;
}

