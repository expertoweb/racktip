CFLAGS = -std=c++0x
SDLFLAGS = `sdl-config --cflags --libs`

all: racktip gpio.o

racktip: racktip.cpp racktip.hpp gpio.o commands.o
	g++ $(SDLFLAGS) $(CFLAGS) -lSDL_image -lSDL_ttf -o racktip racktip.cpp gpio.o commands.o

gpio.o: gpio.cpp gpio.hpp
	g++ $(CFLAGS) -c gpio.cpp

commands.o: commands.cpp commands.hpp
	g++ $(CFLAGS) -c commands.cpp

clean:
	rm -f gpio.o commands.o racktip.o racktip 
