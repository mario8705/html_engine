SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
CPPFLAGS=-Iinclude -I/usr/include/SDL
LDFLAGS=-lSDL -lcairo

all: $(OBJECTS)
	$(CXX) -o html_engine $(OBJECTS) $(LDFLAGS)

clean:
	rm -rf html_engine $(OBJECTS)

.PHONY: all clean