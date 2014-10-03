#include <vector>
#include "gpio.hpp"

void forward();
void right();
void left();
void backwards();
void drop();
void export_pins();
void unexport_pins();
int read_input();

extern std::vector<int> gpios_out; 
extern std::vector<int> gpios_in;
extern std::vector<int> gpios_first;
extern std::vector<int> gpios_second;
extern std::vector<int> gpios_drop;

extern int HEIGHT;
extern int WIDTH;
extern int DELAY;;
extern int PLATE_LINE_STEPS;;
extern int TOTAL_STEPS;;
extern int PLATES_X_BEGIN;
extern int PLATES_Y_BEGIN;
extern int PLATE_Y_MOVE;
extern int DROPPER_X_BEGIN;
extern int DROPPER_Y_BEGIN;
extern int DROPPER_X_MOVE;
extern int RADIUS5;
extern int STEPS_PER_PIXEL;
extern int START_POSITION;
extern int FIRST_LINE;
extern int NEXT_LINE;
extern int LINE_END;
extern int TEXT_X_BEGIN;
extern int TEXT_Y_BEGIN;
