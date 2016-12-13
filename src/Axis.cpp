/*
 * Axis.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: jaym
 */
#include "Axis.hpp"
#include<iostream>
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
Axis::Axis(string name, string descriptor, int t)
{
	Name = name;
	PinDescriptor = descriptor;
    testvar = t;
}



int Axis::Initialize(void)
{
	pthread_create(&SineThread, NULL, Axis::thread_helper, this);
	return 1;
}

void Axis::sinethread(void)
{
	int k = testvar;
	while (k)
	{
		cout << Name << ": " << k-- << endl;
		usleep(1000000);
	}
}

void * Axis::thread_helper(void * c)
{
	((Axis *) c)->sinethread();
	return NULL;
}


} // namespace exploringBB
