CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2 -I./src
LDFLAGS = `sdl2-config --cflags --libs`

SRC = ./src/*.cpp ./src/*/*.cpp
BIN = ./ragdol

default:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)
