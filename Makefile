CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -Iraylib/src
LDFLAGS = -Lraylib/src
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

TARGET = muli.exe

SRC = src/main.cpp src/music_manager.cpp src/texture_manager.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) $(LIBS) -o $(TARGET)

run: all
		./$(TARGET)

clean:
		rm -rf $(TARGET)