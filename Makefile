all:
	gcc `sdl-config --cflags --libs` -std=c++0x -lboost_timer -o examplecpp examplecpp.cpp
