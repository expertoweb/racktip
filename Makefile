all: racktip gpio.o

racktip: racktip.cpp gpio.o
	g++ `sdl-config --cflags --libs` -std=c++0x -lSDL_image -o racktip racktip.cpp gpio.o 

gpio.o: gpio.cpp gpio.hpp
	g++ -c gpio.cpp

clean:
	rm -f gpio.o racktip
