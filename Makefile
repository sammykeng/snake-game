# Compiler và flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS = -lncurses


# Tên file thực thi
TARGET = snake

# Danh sách các file nguồn
SRCS = main.cpp game.cpp menu.cpp obstacle.cpp

# Tạo danh sách các file object
OBJS = $(SRCS:.cpp=.o)

# Rule mặc định
all: $(TARGET)

# Rule để build file thực thi
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule để build các file object
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Rule để clean
clean:
	rm -f $(OBJS) $(TARGET)

# Rule để rebuild
rebuild: clean all

.PHONY: all clean rebuild