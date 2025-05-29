#include "menu.h"
#include <ncurses.h>
#include <cstdio>
#include <cstdlib>

Menu::Menu() {
    loadHighScores();
}

void Menu::saveHighScores() {
    FILE* file = fopen("highscores.dat", "wb");
    if (file != nullptr) {
        // Khởi tạo điểm cao mặc định nếu chưa có
        if (highScores[EASY] == 0) highScores[EASY] = 0;
        if (highScores[HARD] == 0) highScores[HARD] = 0;
        if (highScores[SUPER_HARD] == 0) highScores[SUPER_HARD] = 0;
        
        fwrite(highScores, sizeof(int), 3, file);
        fclose(file);
    }
}

void Menu::loadHighScores() {
    // Khởi tạo điểm cao mặc định
    highScores[EASY] = 0;
    highScores[HARD] = 0;
    highScores[SUPER_HARD] = 0;

    FILE* file = fopen("highscores.dat", "rb");
    if (file != nullptr) {
        fread(highScores, sizeof(int), 3, file);
        fclose(file);
    }
}

void Menu::showHighScores() {
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

GameLevel Menu::showMainMenu() {
    MenuItem currentSelection = PLAY;
    
    while (true) {
        clear();
        
        // Vẽ tiêu đề game
        attron(COLOR_PAIR(4));
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
                                case '1': 
                                    clear();
                                    return EASY;
                                case '2': 
                                    clear();
                                    return HARD;
                                case '3': 
                                    clear();
                                    return SUPER_HARD;
                                case 'm':
                                case 'M': 
                                    return showMainMenu();
                            }
                        }
                        break;
                    case RECORD:
                        showHighScores();
                        return showMainMenu();
                    case QUIT:
                        endwin();
                        std::exit(0);
                }
                break;
        }
    }
    return EASY; // Default return value
}

void Menu::updateHighScore(GameLevel level, int score) {
    if (score > highScores[level]) {
        highScores[level] = score;
        saveHighScores();
    }
}

int Menu::getHighScore(GameLevel level) const {
    return highScores[level];
}