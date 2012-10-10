// NXT++ test.cpp : Defines the entry point for the console application.
//

#include "NXT++.h"
#include <iostream>

using namespace std;

int main()
{
    cout << "NXT++ test project" << endl;

    if(NXT::Open()) //initialize the NXT and continue if it succeds
	{
	    NXT::Sensor::SetTouch(IN_1); //tell the NXT that the touch sensor is in port 1
		while(1) //main loop
		{
			if(NXT::Sensor::GetValue(IN_1) == true) //if the touch sensor is pressed down...
				NXT::Motor::SetForward(OUT_A, 50); //turn the motor in port 1 on 50% power
			else
				NXT::Motor::Stop(OUT_A, false); //if the touch sensor is not pressed down turn the motor off
		}
    }
	return 0;
}

