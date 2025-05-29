# Compiler và flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Thêm đường dẫn đến OpenCV
OPENCV_CFLAGS = $(shell pkg-config --cflags opencv4)
OPENCV_LIBS = $(shell pkg-config --libs opencv4)
CXXFLAGS += $(OPENCV_CFLAGS)
LDFLAGS = $(OPENCV_LIBS)

# Tên file thực thi
TARGET = snake

# Danh sách các file nguồn
SRC_DIR = snakegame
OBJ_DIR = $(SRC_DIR)
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/game.cpp $(SRC_DIR)/menu.cpp

# Tạo danh sách các file object
OBJS = $(SRCS:.cpp=.o)

# Rule mặc định
all: $(TARGET)

# Rule để build file thực thi
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule để build các file object
$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule để clean
clean:
	rm -f $(OBJS) $(TARGET)

# Rule để rebuild
rebuild: clean all

.PHONY: all clean rebuild
