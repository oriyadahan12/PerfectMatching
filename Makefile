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

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean run
