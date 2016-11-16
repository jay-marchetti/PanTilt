//============================================================================
// Name        : PwmTest.cpp
// Author      : Jay Marchetti
// Copyright   : SEI
// Description : Unit tests for BBB PWM drive to Pan & Tilt motors
//============================================================================
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "GPIO.h"
#include "PWM.h"      //placed in the current directory for ease of use

using namespace exploringBB;
using namespace std;

int main(int argc, char* argv[]) {
	unsigned int del = 1000000; //microseconds to sleep
	PWM pwm1("pwm_test_P9_22.17");
	PWM pwm2("pwm_test_P9_16.18");
	pwm1.setPeriod(20000000u);		// Set the period at 20 msec, i.e. 50Hz
	pwm1.setPolarity(PWM::ACTIVE_HIGH);	// using active high PWM
	pwm1.run();                     	// start the PWM output
	if (argc == 2) // if position duty cycle specified just go there, else position sequencer demo
	{
		std::stringstream s_str(argv[1]);
		int pos;	// positional duty cycle in usec
		s_str >> pos;
		if (pos < 0) //if push button slewing
		{
			pos = -pos;
			pwm1.setDutyCycle(unsigned(pos * 1000));
			GPIO inGPIO(115);
			inGPIO.setDirection(GPIO::INPUT);
			while (1)
			{
				if (inGPIO.getValue() == GPIO::HIGH)
				{
					pos -= 10;
					pwm1.setDutyCycle(unsigned(pos * 1000));
					cout << "Set duty on P9_22.17 to " << pos << " usec" << endl;
					usleep(100000);

				}

			}
		}

		pwm1.setDutyCycle(unsigned(pos * 1000));
		cout << "Set duty on P9_22.17 to " << pos << " usec" << endl;
		exit(0);
	}
	cout << "Initializing 20 msec with 400 usec duty on P9_22.17 of BBB..." << endl;
	pwm1.setDutyCycle(400000u);  // Set initial position as the duty cycle in microsecs in [600, 2000]
	pwm1.setPolarity(PWM::ACTIVE_HIGH);	// using active high PWM
	pwm1.run();                     	// start the PWM output
	usleep(del);	//delay
	for (int duty = 400; duty <= 1200; duty += 100)
	{
		pwm1.setDutyCycle(unsigned(duty * 1000));
		usleep(del);
		cout << "At duty = " << duty << " microsec" << endl;
	}

	for (int duty = 1200; duty > 400; duty -= 1)
	{
		pwm1.setDutyCycle(unsigned(duty * 1000));
		del = 10000;
		usleep(del);
		cout << "At duty = " << duty << " microsec" << endl;
	}




	cout << "End of PwmTest application..." << endl;
}
