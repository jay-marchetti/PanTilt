//============================================================================
// Name        : PanTilt.cpp
// Author      : Jay Marchetti
// Date        : Dec 20, 2016
// Version     : 1.50
// Copyright   : SEI, Carnegie Mellon University
// Description : PanTilt application for BeagleBone Black driving 2-axis mount
//============================================================================

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <vector>

#include "GPIO.h"
#include "PWM.h"      // placed in the current directory for ease of use
#include "Thing.hpp"  // C++ test class code
#include "Axis.hpp"

using namespace std;

vector<string> split(const string &s, char delim);


using namespace exploringBB;
using namespace std;

//Thing mything;
//Thing th1("Susan");
//Thing *th[2];

Axis* axis[3];
void  ShowUsage(char *s);
string ModeString[] =
{
		"sin",
		"cos",
		"range"
};

int main(int argc, char* argv[]) {
	string version = "v1.50";
	bool bDone = false;
	string cmd;
	unsigned Ax;
	int args;

//	cout << mything.getname() << endl;
//	th[0] = new Thing("Kate", 1);
//	th[1] = new Thing("Sam", 2);

	axis[0] = new Axis("Unused", "");
	axis[1] = new Axis("Tilt", "pwm_test_P9_16.16", 805);
	axis[2] = new Axis("Pan", "pwm_test_P9_22.15", 765);

	axis[1]->Initialize();
	axis[2]->Initialize();
	Ax = 1;		// Set the current working axis to 1

	while (bDone == false)
	{
		cout << ">>";
		getline(cin, cmd);
		//cout << "Recv'd: " << cmd << endl;
		// Now tokenize the input string using [space] as delimiter
		vector<std::string> x = split(cmd, ' ');
		args = x.size();
		// Now execute cmd
	    if (x[0] == "quit")
	    {
	    	bDone = true;
	    }
	    else if ((x[0] == "axis") && (args == 2))
	    {
	    	int k = strtol(x[1].c_str(), NULL, 0);
	    	if ((k > 0) && (k < 3)) // Range check the selected axis [1, 2]
			{
				Ax = k;
			}
	    	else
	    	{
	    		cout << "Range error" << endl;
	    	}
	    }
	    else if (x[0] == "axis?")
	    {
	    	cout << Ax << endl;
	    }
	    else if (x[0] == "pos")
	    {
	    	float p = strtod(x[1].c_str(), NULL);
	    	int k = (int) (p * axis[Ax]->MicrosecPerDeg);
	    	unsigned u = axis[Ax]->ZeroPos + k;
	    	axis[Ax]->setDuty(u);
	    }
	    else if (x[0] == "pos?")
	    {
	    	unsigned u = axis[Ax]->getDuty();
	    	int k = u - axis[Ax]->ZeroPos;
	    	float p = k / axis[Ax]->MicrosecPerDeg;
	    	cout << p << endl;
	    }

	    else if ((x[0] == "duty") && (args == 2))
	    {
	    	unsigned k = strtol(x[1].c_str(), NULL, 0);
	    	axis[Ax]->setDuty(k);
	    }
	    else if (x[0] == "duty?")
	    {
	    	cout << axis[Ax]->getDuty() << endl;
	    }
	    else if (x[0] == "zero")
	    {
	    	axis[Ax]->Zero();
	    }
	    else if (x[0] == "rezero")
	    {
	    	axis[Ax]->Rezero();
	    }
	    else if (x[0] == "trig")
	    {
	    	int k = strtol(x[1].c_str(), NULL, 0);
	    	axis[Ax]->bTrig = (k != 0) ? true : false;
	    }
	    else if (x[0] == "trigall")
	    {
	    	int k = strtol(x[1].c_str(), NULL, 0);
	    	axis[1]->bTrig = (k != 0) ? true : false;
	    	axis[2]->bTrig = (k != 0) ? true : false;
	    }
	    else if (x[0] == "cycles")
	    {
	    	int k = strtol(x[1].c_str(), NULL, 0);
	    	axis[Ax]->Cycles = k;
	    }
	    else if (x[0] == "cycles?")
	    {
	    	cout << axis[Ax]->Cycles << endl;
	    }
	    else if (x[0] == "amp")
	    {
	    	float a = strtod(x[1].c_str(), NULL);
	    	axis[Ax]->Ampl = a;
	    }
	    else if (x[0] == "amp?")
	    {
	    	cout << axis[Ax]->Ampl << endl;
	    }
	    else if (x[0] == "period")
	    {
	    	float p = strtod(x[1].c_str(), NULL);
	    	axis[Ax]->Period = p;
	    }
	    else if (x[0] == "period?")
	    {
	    	cout << axis[Ax]->Period << endl;
	    }
	    else if (x[0] == "mode")
	    {
	    	if (x[1] == "sin")
	    	{
	    		axis[Ax]->Mode = SIN;
	    	}
	    	else if (x[1] == "cos")
	    	{
	    		axis[Ax]->Mode = COS;
	    	}
	    	else
	    	{
	    		axis[Ax]->Mode = RANGE;
	    	}
	    }
	    else if (x[0] == "mode?")
	    {
	    	cout << ModeString[axis[Ax]->Mode] << endl;
	    }
	    else if (x[0] == "ver?")
	    {
	    	cout << version << endl;
	    }
	    else if (x[0] == "help")
	    {
	        ShowUsage(argv[0]);
	    }
	    else
	    {
	    	cout << "Invalid command. Type 'help' for usage." << endl;
	    }
    }
	cout << "Exiting program PanTilt" << endl;
}

void ShowUsage(char *s)
{
	cout<<"Usage:      sudo "<< s <<"   Where valid commands are:" << endl;
	cout<<"Command <space> <argument> "<<endl;
	cout<<"help        Show (this) help information"<<endl;
	cout<<"quit        Exit the program"<<endl;
	cout<<"axis <n>    Specify axis number [1 or 2]. Retained until changed"<<endl;
	cout<<"axis?       Read present axis number"<<endl;
	cout<<"pos <f>     Statically position axis at a position in degrees [-40.0, 40.0]"<<endl;
	cout<<"pos?        Read present axis position in degrees"<<endl;
	cout<<"zero        Statically position the axis at zero degrees position"<<endl;
	cout<<"amp <f>     Specify sinusoidal motion peak amplitude in degrees [-40.0, 40.0]"<<endl;
	cout<<"amp?        Read present sinusoidal motion peak amplitude in degrees"<<endl;
	cout<<"period <f>  Specify sinusoidal motion period in seconds [1.0, 300.0]"<<endl;
	cout<<"period?     Read present sinusoidal motion period in seconds"<<endl;
	cout<<"cycles <n>  Specify sinusoidal motion number of cycles to complete [1, 100]"<<endl;
	cout<<"cycles?     Read present sinusoidal motion number of cycles to complete"<<endl;
	cout<<"mode <s>    Specify sinusoidal or range motion type [sin, cos, range]"<<endl;
	cout<<"mode?       Read present sinusoidal or range motion type"<<endl;
	cout<<"rezero      Redefine axis zero position to be the present position"<<endl;
	cout<<"duty <n>    Statically position axis at a position in duty cycle microsecs [400, 1200]"<<endl;
	cout<<"duty?       Read present axis position in duty cycle microsecs"<<endl;
	cout<<"trig <n>    Trigger axis to cease (0) or commence (1) motion per the mode [0, 1]"<<endl;
	cout<<"trigall <n> Trigger both axes to cease (0) or commence (1) motion per their mode [0, 1]"<<endl;
	cout<<"ver?        Read the version of this program"<<endl;
	cout<<"Example commands:"<<endl;
	cout<<">>axis 1"<<endl;
	cout<<">>pos?"<<endl;
	cout<<">>25.342"<<endl;
	cout<<">>pos 10.0"<<endl;
	cout<<">>mode sin"<<endl;
	cout<<">>trig 1"<<endl;
}

vector<string> split(const string &s, char delim)
{
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}


