#include "game.h"
#include "menu.h"
#include <ncurses.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    initscr();
    
    // Khởi tạo ncurses
    setlocale(LC_ALL, "");
    set_escdelay(25);
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    // Khởi tạo màu sắc
    use_default_colors();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);    // Màu cho rắn
    init_pair(2, COLOR_RED, COLOR_BLACK);      // Màu cho thức ăn
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);   // Màu cho chướng ngại vật
    init_pair(4, COLOR_CYAN, COLOR_BLACK);     // Màu cho viền
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);  // Màu cho điểm số
    
    // Thêm dòng này để đảm bảo terminal hiển thị đúng màu
    bkgd(COLOR_PAIR(0));
    
    Menu menu;
    
    while (true) {
        GameLevel level = menu.showMainMenu();
        Game game(level);
        game.run();
        
        menu.updateHighScore(level, game.getScore());
    }
    
    endwin();
    return 0;
}