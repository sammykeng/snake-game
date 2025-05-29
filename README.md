# Snake Game

A modern implementation of the classic Snake game using C++ and OpenCV.

## Features

- 🐍 Smooth snake movement with realistic graphics
- 🎮 Three difficulty levels: Easy, Hard, and Super Hard
- 🏆 High score tracking system
- 🧱 Dynamic obstacle generation
- 🎯 Increasing difficulty as you progress
- ⏸️ Pause functionality
- 🎨 Beautiful visual effects

## Requirements

- C++11 or higher
- OpenCV 4.x
- Make
- pkg-config

## Installation

1. Clone the repository:
```bash
git clone https://github.com/sammykeng/snake-game.git
cd snake-game
```

2. Install dependencies:
```bash
# For macOS
brew install opencv pkg-config

# For Ubuntu/Debian
sudo apt-get install libopencv-dev pkg-config
```

3. Build the game:
```bash
make
```

## How to Play

1. Run the game:
```bash
./snake
```

2. Controls:
- Arrow keys or WASD to move the snake
- Space to pause/resume
- ESC to quit
- Enter to restart after game over
- M to return to menu

## Game Rules

- Eat food to grow longer and earn points
- Avoid hitting walls, obstacles, or yourself
- Different difficulty levels have different speeds and obstacle patterns:
  - Easy: First wall appears at 7 points, then every 4 points
  - Hard: First wall appears at 5 points, then every 3 points
  - Super Hard: First wall appears at 3 points, then every 2 points

## Project Structure

```
snake-game/ 
├── snakegame/
│   ├── main.cpp
│   ├── game.cpp
│   ├── game.h
│   ├── menu.cpp
│   ├── menu.h
│   ├── obstacle.cpp
│   ├── obstacle.h
│   └── constants.h
├── Makefile
└── README.md
```

## Contributing

Feel free to submit issues and enhancement requests!

## License

This project is open source and available under the MIT License.
