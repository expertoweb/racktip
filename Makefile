all: racktip gpio.o

racktip: racktip.cpp gpio.o graphics.o
	g++ `sdl-config --cflags --libs` -Wwrite-strings -std=c++0x -lSDL_image -o racktip racktip.cpp gpio.o graphics.o

gpio.o: gpio.cpp gpio.hpp
	g++ -c gpio.cpp

graphics.o: graphics.cpp graphics.hpp
	g++ -c graphics.cpp

clean:
	rm -f gpio.o racktip
