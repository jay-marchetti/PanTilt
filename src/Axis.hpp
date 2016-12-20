/*
 * Axis.hpp
 *
 *  Created on:    Nov 16, 2016
 *      Author:    Jay Marchetti
 *      Project:   MBT/HIL Lab - System Under Test (SUT) Example
 *      Copyright: Software Engineering Institute, Carnegie Mellon University
 */

#ifndef AXIS_HPP_
#define AXIS_HPP_

#include<string>
#include "GPIO.h"
#include "PWM.h"

using std::string;

namespace exploringBB {

enum MODE{ SIN, COS, RANGE };

class Axis {
// Member variables
public:
	bool   bTrig;   			// Value to start / stop (on write) or query for active (read) sine motion
	bool   bufTrig;
	int    Cycles;
	float  Ampl;       			// Peak amplitude of sine motion in degrees (e.g. 5.0)
	float  Period;				// Sine period in seconds
	float  MicrosecPerDeg;		// Scale factor for translation of degrees to microsec duty cycle
	unsigned ZeroPos;			// Axis zero (0.00) degree position in PWM microsecs
	MODE   Mode;

private:
	string Name;				// Name of the axis
	string PinDescriptor;		// BBB pin descriptor e.g. pwm_test_P9_22.15
	pthread_t SineThread;		// Thread for driving sinusoidal axis motion
	PWM*   pwm;					// PWM output for this axis

// Methods
public:
	// Constructors
	Axis   ();
	Axis   (string name, string pinDescriptor);
	Axis   (string name, string descriptor, int zeroPositionPwm);
	// Initializes PWM channel and axis thread - call once before any other method
	int    Initialize ();
	// Re-zeroes the axis 0.00 degree position at the current position (non-persistently)
	void   Rezero ();
	// Statically position the axis
	void   Zero (void);
	int    setPosition (float positionDegrees);
	int    setDuty (unsigned microsecs);
	// Returns the current axis position
	float  getPostion ();
	unsigned getDuty ();
	// Sets the sinusoidal motion parameters if sine motion is currently inactive
	int    setSineMotion (float ampl, float freq, float phas, int cycles);
	// Returns the percent complete of sinusoidal motion over all commanded cycles
	float  getSinePercentComplete ();
	// Starts or stops sinusoidal motion immediately
	int    setSineMotionTrigger (bool activate);
	// Returns whether sinusoidal motion is currently active. Clears upon completion.
	bool   getSineMotionTrigger ();
	void   sinethread (void);
	static void * thread_helper (void * context);
	// Destructor
	~Axis  ();

private:

};

} /* namespace exploringBB */
#endif /* AXIS_HPP_ */
