#include <fcntl.h>
#include <fstream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HIGH  1
#define LOW   0

#define IN    0
#define OUT   1

int GPIOExport(int pin);
int GPIOUnexport(int pin);
int GPIODirection(int pin, int dir);
int GPIORead(int pin);
int GPIOWrite(int pin, int value);

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
extern int PIXELS_PER_STEP;
extern int START_POSITION;
extern int FIRST_LINE;
extern int NEXT_LINE;
extern int LINE_END;
extern int TEXT_X_BEGIN;
extern int TEXT_Y_BEGIN;
