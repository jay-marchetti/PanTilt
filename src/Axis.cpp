/*
 * Axis.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: jaym
 */
#include "Axis.hpp"
#include<string>
#include<sstream>
#include<cstdio>
#include<unistd.h>
#include<pthread.h>
#include<math.h>
using namespace std;

namespace exploringBB {

Axis::Axis()
{

}
Axis::Axis(string name, string descriptor)
{
	Name = name;
	PinDescriptor = descriptor;
}
Axis::Axis(string name, string descriptor, int zeroPos)
{
	Name = name;
	PinDescriptor = descriptor;
    ZeroPos = zeroPos;
    pwm = new PWM(descriptor);
}

int Axis::Initialize(void)
{
	pwm->setPeriod(20000000u);				// Set the period at 20 msec, i.e. 50Hz
	pwm->setPolarity(PWM::ACTIVE_HIGH);		// using active high PWM
	pwm->setDutyCycle(ZeroPos * 1000u);		// set initial position to 0.00 degrees
	pwm->run();                     		// start the PWM output
	MicrosecPerDeg = 8.5;					// Set axis scale factor
	Period = 10.0;
	Cycles = 1;
	Ampl = 30.0;
	Mode = SIN;
	// Create thread for this axis used for sine & step modes
	pthread_create(&SineThread, NULL, Axis::thread_helper, this);
	return 1;
}

int Axis::setDuty(unsigned microsecs)
{
	if (pwm == NULL)
		return -1;
	pwm->setDutyCycle(microsecs * 1000);
	return 0;
}

void Axis::Zero(void)
{
	pwm->setDutyCycle(ZeroPos * 1000);
}

void Axis::Rezero(void)
{
	unsigned k = pwm->getDutyCycle() / 1000;
	ZeroPos = k;
	pwm->setDutyCycle(ZeroPos * 1000);
}

unsigned Axis::getDuty()
{
	unsigned k = pwm->getDutyCycle() / 1000;
	return k;
}


void Axis::sinethread(void)
{
	const float PI = 3.1415927;
	const float TWO_PI = 2.0 * PI;
	int DCduty, ACduty, cycles, delay, duty;
	unsigned cpos;
	float ang, inc;
	bool step;

	while (1)
	{
		if (bTrig == true)
		{
			if ((Mode == SIN) || (Mode == COS))
			{
				if (bufTrig == false) // If just entering sine motion
				{
					DCduty = pwm->getDutyCycle(); // Save current axis DC position
					cycles = Cycles;
					ang = 0.0;
					inc = 1.0 / (50.0 * Period);
					bufTrig = true;
				}
				else
				{
					if (cycles > 0)
					{
						if (Mode == SIN)
						{
							ACduty = Ampl * MicrosecPerDeg * sin(ang * TWO_PI) * 1000.0;
						}
						else
						{
							ACduty = Ampl * MicrosecPerDeg * cos(ang * TWO_PI) * 1000.0;
						}
						ang += inc;
						if (ang > 1.0)
						{
							ang = 0.0;
							cycles--;
						}
						pwm->setDutyCycle((unsigned)(DCduty + ACduty));
					}
					else
					{
						bTrig = false;
					}
				}
			}
			else // Else mode is RANGE
			{
				if (bufTrig == false) // If just entering range mode motion
				{
					delay = 50;
					duty = 400;
					cpos = pwm->getDutyCycle(); // Save current axis position
					pwm->setDutyCycle(unsigned(duty * 1000));
					bufTrig = true;
					step = true;
				}
				delay--;
				if ((duty < 1200) && (step == true))
				{
					if (delay == 0)
					{
						duty += 100;
						pwm->setDutyCycle(unsigned(duty * 1000));
						delay = 50;
					}
				}
				else
				{
					if (duty >= 1200)
					{
						step = false;
						duty = 1200 - 1;
					}
					pwm->setDutyCycle(unsigned(duty-- * 1000));
					if (duty == 400)
					{
						bTrig = false;
						pwm->setDutyCycle(cpos);
					}
				}
			}
		}
		else
		{
			bufTrig = false;
		}
		usleep(20000);		// 20 millisec (50Hz) timing thread
	}
}

void * Axis::thread_helper(void * c)
{
	((Axis *) c)->sinethread();
	return NULL;
}


} // namespace exploringBB

