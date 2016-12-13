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
#include "GPIO.h"
#include "PWM.h"      //placed in the current directory for ease of use
#include "Axis.hpp"

using namespace exploringBB;
using namespace std;

class Thing
{
private:
	string name;
	int num;
public:
	Thing()
	{
		name = "Jay";
	}
	Thing(string nm)
	{
		name = nm;
	}
	Thing(string nm, int n)
	{
		name = nm;
		num = n;
	}
	string getname(void)
	{
		return name;
	}
};

Thing mything;
Thing th1("Susan");
Thing *th[2];

Axis* axis[3];
void  ShowUsage(char *s);
void* threadFunc1(void* arg);
void* threadFunc2(void *arg);

int main(int argc, char* argv[]) {
	int n, ax = 1;
	unsigned duty;
	string version = "v1.12";
	bool bGoPos = false;
	bool bSineTest = false;
	bool bRangeTest = false;
	unsigned int dc, per;
	float Amp = 100.0;
	float Freq = 0.2;
	float Phase = 0.0;
	int Cycles = 1;
	pthread_t thread1, thread2;

	cout << mything.getname() << endl;

	th[0] = new Thing("Kate", 1);
	th[1] = new Thing("Sam", 2);

	axis[0] = new Axis("Unused", "");
	axis[1] = new Axis("Tilt", "pwm_test_P9_16.16",10);
	axis[2] = new Axis("Pan", "pwm_test_P9_22.15",7);

	axis[1]->Initialize();
	axis[2]->Initialize();
	int jj = 0;
	while (1)
	{
		jj++;
	}


	unsigned int del, sec = 1000000; //microseconds to sleep
	PWM pwm2("pwm_test_P9_22.15");
	PWM pwm1("pwm_test_P9_16.16");
	pwm2.setPeriod(20000000u);		// Set the period at 20 msec, i.e. 50Hz
	pwm2.setPolarity(PWM::ACTIVE_HIGH);	// using active high PWM
	pwm2.run();                     	// start the PWM output
	pwm1.setPeriod(20000000u);		// Set the period at 20 msec, i.e. 50Hz
	pwm1.setPolarity(PWM::ACTIVE_HIGH);	// using active high PWM
	pwm1.run();                     	// start the PWM output
	PWM* pwm;
	pwm = &pwm1;

	while ((n = getopt(argc,argv,"ha:d:rvA:F:P:N:tRT")) != -1)
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
	      case 'd': // Duty cycle positioning
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
	    	if (pthread_create(&thread1, NULL, &threadFunc1, NULL))
	    	{
	    		cout << "Failed to create thread1" << endl;
	    		exit (1);
	    	}
	    	if (pthread_create(&thread2, NULL, &threadFunc2, NULL))
	    	{
	    		cout << "Failed to create thread2" << endl;
	    		exit (1);
	    	}
	    	for (int i=0; i<16; i++)
	    	{
	    		usleep(1000000);	// 1 sec delay
	    	}
	    	exit (0);
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


int Oldmain(int argc, char* argv[]) {
	int n, ax = 1;
	unsigned duty;
	string version = "v1.12";
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
	PWM pwm2("pwm_test_P9_22.15");
	PWM pwm1("pwm_test_P9_16.16");
	pwm2.setPeriod(20000000u);		// Set the period at 20 msec, i.e. 50Hz
	pwm2.setPolarity(PWM::ACTIVE_HIGH);	// using active high PWM
	pwm2.run();                     	// start the PWM output
	pwm1.setPeriod(20000000u);		// Set the period at 20 msec, i.e. 50Hz
	pwm1.setPolarity(PWM::ACTIVE_HIGH);	// using active high PWM
	pwm1.run();                     	// start the PWM output
	PWM* pwm;
	pwm = &pwm1;

	while ((n = getopt(argc,argv,"ha:d:rvA:F:P:N:tRT")) != -1)
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
	      case 'd': // Duty cycle positioning
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
	    	if (pthread_create(&thread1, NULL, &threadFunc1, NULL))
	    	{
	    		cout << "Failed to create thread1" << endl;
	    		exit (1);
	    	}
	    	if (pthread_create(&thread2, NULL, &threadFunc2, NULL))
	    	{
	    		cout << "Failed to create thread2" << endl;
	    		exit (1);
	    	}
	    	for (int i=0; i<16; i++)
	    	{
	    		usleep(1000000);	// 1 sec delay
	    	}
	    	exit (0);
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

void ShowUsage(char *s)
{
	cout<<"Usage:   "<<s<<" [-option] [argument]"<<endl;
	  cout<<"option:  "<<"-h  show (this) Help information"<<endl;
	  cout<<"         "<<"-a  specify Axis number [1 or 2]. Must preceed any other axis args."<<endl;
	  cout<<"         "<<"-d  position axis to Duty cycle in microsec [400, 1200]"<<endl;
	  cout<<"         "<<"-r  Read out axis current PWM parameters"<<endl;
	  cout<<"         "<<"-v  show program Version infomation"<<endl;
	  cout<<"         "<<"-A  specify Amplitude of sine motion in duty cycle microsecs [100, 500]"<<endl;
	  cout<<"         "<<"-N  specify Number of sine cycles to complete [1, n]"<<endl;
	  cout<<"         "<<"-t  Trigger (commence) sine test mode motion"<<endl;
	  cout<<"         "<<"-R  commence Range test mode for an axis"<<endl;
	  cout<<"         "<<"-T  run simple POSIX Thread test"<<endl;
	  cout<<"example1: sudo "<<s<<" -a1 -d850"<<endl;
	  cout<<"example2: sudo "<<s<<" -a2 -r"<<endl;
	  cout<<"example3: sudo "<<s<<" -a2 -A400.0 -N3 -t"<<endl;
	  cout<<"example4: sudo "<<s<<" -a1 -R"<<endl;
	  cout<<"example5: sudo "<<s<<" -h"<<endl;
}

void* threadFunc1(void* arg)
{
	while (1)
	{
		int i = 1000;		// 1 second thread
		while (i-- > 0)
		{
			usleep(1000);	// sleep for 1 millisec
		}
    	cout << "1 second thread" << endl;
	}
}

void* threadFunc2(void *arg)
{
	while (1)
	{
		int i = 3000;		// 3 second thread
		while (i-- > 0)
		{
			usleep(1000);	// sleep for 1 millisec
		}
    	cout << "3 second thread" << endl;
	}
}

