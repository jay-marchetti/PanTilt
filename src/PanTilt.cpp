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

int main(int argc, char* argv[]) {
	int n, ax = 1;
	unsigned duty;
	string version = "v1.30";
	bool bDone = false;
	string cmd;
	unsigned Ax;
	int args;

	bool bGoPos = false;
	bool bSineTest = false;
	bool bRangeTest = false;
	unsigned int dc, per;
	float Amp = 100.0;
	float Freq = 0.2;
	float Phase = 0.0;
	int Cycles = 1;
	pthread_t thread1, thread2;

//	cout << mything.getname() << endl;
//	th[0] = new Thing("Kate", 1);
//	th[1] = new Thing("Sam", 2);

	axis[0] = new Axis("Unused", "");
	axis[1] = new Axis("Tilt", "pwm_test_P9_16.16", 805);
	axis[2] = new Axis("Pan", "pwm_test_P9_22.15", 765);

	axis[1]->Initialize();
	axis[2]->Initialize();
	Ax = 1;		// Set the current working axis to 1

	unsigned int del, sec = 1000000; //microseconds to sleep

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
	cout<<"Usage:   sudo "<< s <<"   Where valid commands are:" << endl;
	cout<<"Command <space> <argument> "<<endl;
	cout<<"help      Show (this) help information"<<endl;
	cout<<"axis <n>  Specify axis number [1 or 2]. Retained until changed"<<endl;
	cout<<"axis?     Read present axis number [1 or 2]"<<endl;
	cout<<"duty <n>  Statically position axis to duty cycle in microsec [400, 1200]"<<endl;
	cout<<"duty?     Read present Axis position as duty cycle in microsec [400, 1200]"<<endl;
	cout<<"zero      Statically position axis at its zero position"<<endl;
	cout<<"rezero    Redefine axis zero position to present position"<<endl;
	cout<<"-A  specify Amplitude of sine motion in duty cycle microsecs [100, 500]"<<endl;
	cout<<"-N  specify Number of sine cycles to complete [1, n]"<<endl;
	cout<<"-t  Trigger (commence) sine test mode motion"<<endl;
	cout<<"-R  commence Range test mode for an axis"<<endl;
	cout<<"-T  run simple POSIX Thread test"<<endl;
	cout<<"example1: sudo "<<s<<" -a1 -d850"<<endl;
	cout<<"example2: sudo "<<s<<" -a2 -r"<<endl;
	cout<<"example3: sudo "<<s<<" -a2 -A400.0 -N3 -t"<<endl;
	cout<<"example4: sudo "<<s<<" -a1 -R"<<endl;
	cout<<"example5: sudo "<<s<<" -h"<<endl;
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


#if 0
int Oldmain(int argc, char* argv[]) {
	int n, ax = 1;
	unsigned duty;
	string version = "v1.30";
	bool bDone = false;
	bool bGoPos = false;
	bool bSineTest = false;
	bool bRangeTest = false;
	unsigned int dc, per;
	float Amp = 100.0;
	float Freq = 0.2;
	float Phase = 0.0;
	int Cycles = 1;
	pthread_t thread1, thread2;

	unsigned int del, sec = 1000000; //microseconds to sleep

	while (bDone == false)
	  {




	    switch (n)
	    {
	      /*option h show the help infomation*/
	      case 'h':
	        ShowUsage(argv[0]);
	        exit(1);
	        break;
	      case 'a':
	        ax = atoi(optarg);
	    	switch (ax)
	    	{
	        	case 1: pwm = &pwm1; break;
	    		case 2: pwm = &pwm2; break;
	    		default: exit(1);
	    	}
	        break;
	      case 'd': // Duty cycle positioning	    	  if (pthread_create(&thread1, NULL, &threadFunc1, NULL))
	    	for (int i=0; i<16; i++)
	    	{
	    		usleep(1000000);	// 1 sec delay
	    	}
	    	exit (0);

	    	duty = (unsigned)atoi(optarg);
	    	bGoPos = true;
	    	break;
	      case 'r': // Read current position
	    	dc = pwm->getDutyCycle();
	    	per = pwm->getPeriod();
	    	cout << "Duty cycle = " << dc << endl;
	    	cout << "Period = " << per << endl;
		    exit(0);
		    break;
	      case 'v':
	    	cout << argv[0] << " version: " << version << endl;
	    	exit(0);
	    	break;
	      case 'A':
	    	Amp = atof(optarg);
	    	break;
	      case 'F':
	    	Freq = atof(optarg);
	    	break;
	      case 'P':
	    	Phase = atof(optarg);
	    	break;
	      case 'N':
	    	Cycles = atoi(optarg);
	    	break;
	      case 't': // Trigger Sine test
	    	bSineTest = true;
	    	break;
	      case 'R': // Range test
	    	bRangeTest = true;
	    	break;
	      case 'T': // Simple POSIX thread test
	    	break;
	      default:
	    	break;
	    }
	}

	if (bGoPos == true) // If just go to static position
	{
		pwm->setDutyCycle(unsigned(duty * 1000));
		cout << "Axis " << ax << " at duty = " << duty << " microsec" << endl;
		exit(0);
	}
	if (bSineTest == true) // If base-level scorsby test mode
	{
		cout << "Sine Test" << endl;
		const float PI = 3.1415927;
		const float TWO_PI = 2.0 * PI;
		del = 25000;
		int DCduty = pwm->getDutyCycle(); // Save current axis DC position
		for (int i=0; i<Cycles; i++)
		{
			cout << "cycle #"<< i+1 << endl;
			for (float x=0.0; x<1.0; x+=0.0025) // Simple fixed frequency and phase sine test
			{
				int ACduty = Amp * sin(x * TWO_PI) * 1000.0;
				//cout << DCduty + ACduty << endl;
				pwm->setDutyCycle((unsigned)(DCduty + ACduty));
				usleep(del);
			}
		}
		pwm->setDutyCycle((unsigned)DCduty);
		exit(0);
	}
	if (bRangeTest == true) // If axis motion range test mode
	{
		cout << "Axis" << ax << " Range Test" << endl;
		unsigned int cPos = pwm->getDutyCycle(); // Save current axis position
		pwm->setDutyCycle(400000u);  // Set initial position as the duty cycle in microsecs in [600, 2000]
		for (int duty = 400; duty <= 1200; duty += 100)
		{
			pwm->setDutyCycle(unsigned(duty * 1000));
			usleep(sec);
			cout << "Axis " << ax << " at duty = " << duty << " microsec" << endl;
		}
		del = 8000;
		for (int duty = 1200; duty > 400; duty -= 1)
		{
			pwm->setDutyCycle(unsigned(duty * 1000));
			usleep(del);
			cout << "Axis " << ax << " at duty = " << duty << " microsec" << endl;
		}
		pwm->setDutyCycle(cPos);
		exit(0);
	}

}
#endif



