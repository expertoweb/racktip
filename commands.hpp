#include <vector>
#include "gpio.hpp"
#include "extern.hpp"

void forward(int steps);
void right(int steps);
void left(int steps);
void backwards(int steps);
void stop();
void drop();
void export_pins();
void unexport_pins();
int read_input();
