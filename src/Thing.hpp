/*
 * Thing.hpp
 *
 *  Created on: Dec 14, 2016
 *      Author: jaym
 */

#ifndef THING_HPP_
#define THING_HPP_

#include<string>
using std::string;

namespace exploringBB {

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

} // namespace

#endif /* THING_HPP_ */
