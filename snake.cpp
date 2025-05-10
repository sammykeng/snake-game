#include <iostream>
#include <list>
#include <thread>
#include <chrono>
#include <ncurses.h>

int nScreenWidth = 120;
int nScreenHeight = 30;

struct sSnakeSegment
{
    int x;
    int y;
};

int main()
{
    initscr();
    keypad(stdscr, TRUE);
    noecho(); 
    curs_set(0);

    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
    {
        screen[i] = L' ';
    }

    while (1)
    {
        std::list<sSnakeSegment> snake = {{60,15},{61,15},{62,15},{63,15},{64,15},{65,15},{66,15},{67,15},{68,15},{69,15}};
        int nFoodX = 30;
        int nFoodY = 15;
        int nScore = 0;
        int nSnakeDirection = 3;
        bool bDead = false;
        timeout(100);

        while (!bDead)
        {
            auto t1 = std::chrono::system_clock::now();
            while ((std::chrono::system_clock::now() - t1) < ((nSnakeDirection % 2 == 1) ? std::chrono::milliseconds(120) : std::chrono::milliseconds(200)))
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
                    snake.push_front({snake.front().x, snake.front().y - 1});
                    break;
                case 1: 
                snake.push_front({snake.front().x + 1, snake.front().y });
                    break;
                case 2: 
                snake.push_front({snake.front().x, snake.front().y + 1});
                    break;
                case 3: 
                snake.push_front({snake.front().x - 1, snake.front().y});
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
            
                if (snake.front().x < 0 || snake.front().x >= nScreenWidth)
                {
                    bDead = true;
                }
                if (snake.front().y < 3 || snake.front().y >= nScreenHeight)
                {
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

                for (int i = 0; i < nScreenWidth; i++)
                {
                    screen[i] = L'=';
                    screen[2 * nScreenWidth + i] = L'=';
                }

                std::string scoreStr = "Snake Game   SCORE: " + std::to_string(nScore);
                for (size_t i = 0; i < scoreStr.size(); i++)
                {
                    screen[1 * nScreenWidth + 5 + i] = scoreStr[i];
                }

                for(auto s : snake){
                    screen[s.y * nScreenWidth + s.x] = bDead ? L'+' : L'@';
                }
                
                screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'X' : L'@';

                screen[nFoodY * nScreenWidth + nFoodX] = L'%';

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
                        mvaddch(y, x, screen[y * nScreenWidth + x]);
                    }
                }
            
                refresh();
            }
        }

        while (getch() != ' ');
    }

    endwin();
    return 0;
}