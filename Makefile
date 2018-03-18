SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
CPPFLAGS=-Iinclude -I/usr/include/SDL -std=c++11 -g
LDFLAGS=-lSDL -lcairo -g

all: $(OBJECTS)
	$(CXX) -o html_engine $(OBJECTS) $(LDFLAGS)

clean:
	rm -rf html_engine $(OBJECTS)

run: all
	./html_engine tests/test9.html

.PHONY: all clean run
