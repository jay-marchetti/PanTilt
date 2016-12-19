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
	while (1)
	{
		usleep(20000);		// 20 millisec (50Hz) timing thread
	}
}

void * Axis::thread_helper(void * c)
{
	((Axis *) c)->sinethread();
	return NULL;
}


} // namespace exploringBB

