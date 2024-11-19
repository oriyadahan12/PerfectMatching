CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = Geometry.cpp PerfectMatching.cpp GUILogic.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = main

run: $(EXEC)
	./$(EXEC)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Geometry.o: Geometry.cpp Geometry.hpp
PerfectMatching.o: PerfectMatching.cpp PerfectMatching.hpp
GUILogic.o: GUILogic.cpp GUILogic.hpp
main.o: main.cpp Geometry.hpp PerfectMatching.hpp GUILogic.hpp

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean run
