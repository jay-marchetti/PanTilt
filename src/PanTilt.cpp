//============================================================================
// Name        : PanTilt.cpp
// Author      : Jay Marchetti
// Version     :
// Copyright   : SEI
// Description : Hello World in C++, Ansi-style
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
	PWM* pwm;
	unsigned int del = 1000000; //microseconds to sleep
	PWM pwm2("pwm_test_P9_22.15");
	PWM pwm1("pwm_test_P9_16.16");
	pwm2.setPeriod(20000000u);		// Set the period at 20 msec, i.e. 50Hz
	pwm2.setPolarity(PWM::ACTIVE_HIGH);	// using active high PWM
	pwm2.run();                     	// start the PWM output
	pwm1.setPeriod(20000000u);		// Set the period at 20 msec, i.e. 50Hz
	pwm1.setPolarity(PWM::ACTIVE_HIGH);	// using active high PWM
	pwm1.run();                     	// start the PWM output
	pwm = &pwm1;		// set default axis choice = Inner
	if (argc >= 2) // If usage is: PwmTest axisnum OR  PwmTest axisnum position
	{
		int ax;
		std::stringstream ax_str(argv[1]);
		ax_str >> ax;
		switch (ax)
		{
			case 1: pwm = &pwm1; break;
			case 2: pwm = &pwm2; break;
			default: exit(0);
		}
	}
	if (argc == 3) // If usage is: PwmTest axisnum position
	{
		int pos;	// positional duty cycle in usec
		std::stringstream pos_str(argv[2]);
		pos_str >> pos;
		pwm->setDutyCycle(unsigned(pos * 1000));
		usleep(del);
		cout << "At duty = " << pos << " microsec" << endl;
		exit(0);
	}
	pwm->setDutyCycle(400000u);  // Set initial position as the duty cycle in microsecs in [600, 2000]
	for (int duty = 400; duty <= 1200; duty += 100)
	{
		pwm->setDutyCycle(unsigned(duty * 1000));
		usleep(del);
		cout << "At duty = " << duty << " microsec" << endl;
	}

	for (int duty = 1200; duty > 400; duty -= 1)
	{
		pwm->setDutyCycle(unsigned(duty * 1000));
		del = 10000;
		usleep(del);
		cout << "At duty = " << duty << " microsec" << endl;
	}
}

