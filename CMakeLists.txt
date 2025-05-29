cmake_minimum_required(VERSION 3.10)
project(snake_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add OpenCV paths for macOS
include_directories(/opt/homebrew/opt/opencv/include/opencv4)
link_directories(/opt/homebrew/opt/opencv/lib)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Add source files
set(SOURCES
    snakegame/main.cpp
    snakegame/game.cpp
    snakegame/menu.cpp
    snakegame/obstacle_manager.cpp
)

# Create executable
add_executable(snake_game ${SOURCES})

# Link OpenCV libraries
target_link_libraries(snake_game ${OpenCV_LIBS}) 