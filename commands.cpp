#include "commands.hpp"

void forward(){

   std::vector<int>::iterator pin; 
   std::vector<int>::iterator val; 

   for (int step = 0; step < PLATE_LINE_STEPS; step++){
	   for (pin = gpios_out.begin(), val = gpios_first.begin(); pin != gpios_out.end(); ++pin, ++val)
	   {
		   GPIOWrite(*pin, *val);
	   }
	   usleep(DELAY);
	   for (pin = gpios_out.begin(), val = gpios_second.begin(); pin != gpios_out.end(); ++pin, ++val)
	   {
		   GPIOWrite(*pin, *val);
	   }
	   usleep(DELAY);
   }
}

void rewind(){

   std::vector<int>::iterator pin; 
   std::vector<int>::iterator val; 

   for (int step = 0; step < TOTAL_STEPS; step++){
   	for (pin = gpios_out.begin(), val = gpios_second.begin(); pin != gpios_out.end(); ++pin, ++val)
   	{
       	    GPIOWrite(*pin, *val);
   	}
   	usleep(DELAY);
   	for (pin = gpios_out.begin(), val = gpios_first.begin(); pin != gpios_out.end(); ++pin, ++val)
   	{
       	    GPIOWrite(*pin, *val);
   	}
   	usleep(DELAY);
   }
}
 
void stop(){
  std::vector<int>::iterator pin; 
  std::vector<int>::iterator val; 

  for (pin = gpios_out.begin(), val = gpios_stop.begin(); pin != gpios_out.end(); ++pin, ++val)
  {
      GPIOWrite(*pin, *val);
  } 
  usleep(DELAY);
}

void export_pins(){
  std::vector<int>::iterator pin; 

  for (pin = gpios_out.begin(); pin != gpios_out.end(); ++pin)
  {
      GPIOExport(*pin);
  }	 

  for (pin = gpios_out.begin(); pin != gpios_out.end(); ++pin)
  {
      GPIODirection(*pin,OUT);
  }	 

  for (pin = gpios_in.begin(); pin != gpios_in.end(); ++pin)
  {
      GPIOExport(*pin);
  }	 

  for (pin = gpios_in.begin(); pin != gpios_in.end(); ++pin)
  {
      GPIODirection(*pin,IN);
  }
}


void unexport_pins(){
  std::vector<int>::iterator pin; 

  for (pin = gpios_out.begin(); pin != gpios_out.end(); ++pin)
  {
      GPIOUnexport(*pin);
  }

  for (pin = gpios_in.begin(); pin != gpios_in.end(); ++pin)
  {
      GPIOUnexport(*pin);
  }
}

int read_input(){
	std::vector<int>::iterator pin;
	for (pin = gpios_in.begin(); pin != gpios_in.end(); ++pin)
	{
	      return GPIOExport(*pin);
	}
}
