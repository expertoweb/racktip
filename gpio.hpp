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
