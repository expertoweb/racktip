#include "commands.hpp"

void forward(int steps){

   std::vector<int>::iterator pin; 
   std::vector<int>::iterator val; 

	   for (pin = gpios_out.begin(), val = gpios_forward.begin(); pin != gpios_out.end(); ++pin, ++val)
	   {
		   GPIOWrite(*pin, *val);
	   }
   for (int step = 0; step < steps; step++){
	   for (pin = gpios_out.begin(), val = gpios_step.begin(); val != gpios_step.end(); ++pin, ++val)
	   {
		   GPIOWrite(*pin, *val);
	   }
	   usleep(MOVE_DELAY);
   }
}

void right(int steps){

   std::vector<int>::iterator pin;
   std::vector<int>::iterator val;

   for (pin = gpios_out.begin(), val = gpios_right.begin(); pin != gpios_out.end(); ++pin, ++val)
   {
	   GPIOWrite(*pin, *val);
   }
   for (int step = 0; step < steps; step++){
	   for (pin = gpios_out.begin(), val = gpios_step.begin(); val != gpios_step.end(); ++pin, ++val)
	   {
		   GPIOWrite(*pin, *val);
	   }
	   usleep(MOVE_DELAY);
   }
}

void left(int steps){

   std::vector<int>::iterator pin;
   std::vector<int>::iterator val;

   for (pin = gpios_out.begin(), val = gpios_left.begin(); pin != gpios_out.end(); ++pin, ++val)
   {
	   GPIOWrite(*pin, *val);
   }
   for (int step = 0; step < steps; step++){
	   for (pin = gpios_out.begin(), val = gpios_step.begin(); val != gpios_step.end(); ++pin, ++val)
	   {
		   GPIOWrite(*pin, *val);
	   }
	   usleep(MOVE_DELAY);
   }
}

void backwards(int steps){
   std::vector<int>::iterator pin; 
   std::vector<int>::iterator val; 

   for (pin = gpios_out.begin(), val = gpios_backwards.begin(); pin != gpios_out.end(); ++pin, ++val)
   {
   		GPIOWrite(*pin, *val);
   }

   for (int step = 0; step < steps; step++){
   	for (pin = gpios_out.begin(), val = gpios_step.begin(); val != gpios_step.end(); ++pin, ++val)
   	{
   		GPIOWrite(*pin, *val);
    	}
   	usleep(MOVE_DELAY);
   }

}

void stop(){
   std::vector<int>::iterator pin; 
   std::vector<int>::iterator val; 

  for (pin = gpios_out.begin(), val = gpios_stop.begin(); val != gpios_stop.end(); ++pin, ++val)
  {
  	GPIOWrite(*pin, *val);
  }
}
 
void drop(){
  std::vector<int>::iterator pin; 
  std::vector<int>::iterator val; 

  for (pin = gpios_out.begin(), val = gpios_drop.begin(); val != gpios_drop.end(); ++pin, ++val)
  {
      GPIOWrite(*pin, *val);
  } 
  usleep(DROP_DELAY);
  for (pin = gpios_out.begin(), val = gpios_stop.begin(); val != gpios_stop.end(); ++pin, ++val)
  {
  	  GPIOWrite(*pin, *val);
  }
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
      GPIOWrite(*pin, 0);
  }

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
	      return GPIORead(*pin);
	}
}
