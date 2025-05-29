#pragma once

// Kích thước màn hình
const int nScreenWidth = 120;
const int nScreenHeight = 30;

// Các level game
enum GameLevel {
    EASY,
    HARD,
    SUPER_HARD
};

// Các menu item
enum MenuItem {
    PLAY,
    RECORD,
    QUIT
};

// Hướng di chuyển
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};