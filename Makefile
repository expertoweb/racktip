all: racktip.cpp
	gcc -std=c++0x -lboost_timer -lboost_filesystem -o racktip racktip.cpp
#gcc `sdl-config --cflags --libs` -std=c++0x -lSDL_image -lboost_timer -lboost_filesystem -o racktip racktip.cpp
