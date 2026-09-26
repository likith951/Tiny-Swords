CXX = g++
CXXFLAGS = -std=c++17 -Wall $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = tiny-swords

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
