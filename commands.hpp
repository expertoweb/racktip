#include <vector>
#include "gpio.hpp"
#include "constants.hpp"

void forward();
void rewind();
void stop();
void export_pins();
void unexport_pins();
int read_input();

extern std::vector<int> gpios_out; 
extern std::vector<int> gpios_in;
extern std::vector<int> gpios_first;
extern std::vector<int> gpios_second;
extern std::vector<int> gpios_stop;
