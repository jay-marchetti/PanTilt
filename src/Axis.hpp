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
using std::string;

namespace exploringBB {

class Axis {
// Member variables
public:

private:
	string Name;				// Name of the axis
	bool   IsLimitedRotation;	// Whether axis is freely rotational or limited rotation
	string PinDescriptor;		// BBB pin descriptor e.g. pwm_test_P9_22.15
	float  MinAngleDegrees;		// Minimum allowable angle (degrees) if limited rotation
	float  MaxAngleDegrees;		// Maximum allowable angle (degrees) if limited rotation
	float  Position;			// Current position in degrees
	float  SineAmplitude;       // Peak amplitude of sine motion in degrees (e.g. 5.0)
	float  SineFrequencyHz;		// Frequency of sine motion in Hz (e.g. 0.1)
	float  SinePhaseDegrees;	// Phase in [0.0, 360.0] of sine motion in degrees (e.g. 90.0)
	int    SineNumberOfCycles;	// Number of sine motion cycles to execute (e.g. 8)
	float  SinePercentComplete;	// Readout of percent done in [0.0, 100.0] for sine motion
	bool   SineMotionTrigger;   // Value to start / stop (on write) or query for active (read) sine motion

// Methods
public:
	// Constructor
	Axis   (string name, bool isLimitedRotation, string pinDescriptor, float min, float max);
	// Initializes PWM channels and places axis to known (midpoint) position
	int    Initialize ();
	// Commands the axis to move to a position
	int    setPosition (float positionDegrees);
	// Returns the current (commanded) position in degrees
	float  getPostion ();
	// Sets the sinusoidal motion parameters if sine motion is currently inactive
	int    setSineMotion (float ampl, float freq, float phas, int cycles);
	// Returns the percent complete of sinusoidal motion over all commanded cycles
	float  getSinePercentComplete ();
	// Starts or stops sinusoidal motion immediately
	int    setSineMotionTrigger (bool activate);
	// Returns whether sinusoidal motion is currently active. Clears upon completion.
	bool   getSineMotionTrigger ();
	// Destructor
	~Axis  ();

private:

};

} /* namespace exploringBB */
#endif /* AXIS_HPP_ */
