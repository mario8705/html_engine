SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
CPPFLAGS=-Iinclude -I/usr/local/Cellar/sdl/1.2.15/include/SDL -std=c++11 -g
LDFLAGS=-lSDL -lcairo -g -I/usr/local/Cellar/sdl/1.2.15/lib -lSDLmain -framework Cocoa

all: $(OBJECTS)
	$(CXX) -o html_engine $(OBJECTS) $(LDFLAGS)

clean:
	rm -rf html_engine $(OBJECTS)

run: all
	# ./html_engine tests/test9.html
	./html_engine

.PHONY: all clean run
