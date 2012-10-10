#include "NXT++.h"
#include <iostream>
using namespace std;

void BackUp();

int main()
{
	srand(NULL);
	if(NXT::OpenBT()) //initialize the NXT and continue if it succeeds
	{
		NXT::Sensor::SetTouch(IN_1);
		NXT::Sensor::SetSound(IN_2);
		NXT::Sensor::SetLight(IN_3, false);
		NXT::Sensor::SetSonar(IN_4);
		NXT::Motor::SetForward(OUT_A, 50);
		NXT::Motor::SetForward(OUT_C, 50);
		while(1) 
		{
			if(NXT::Sensor::GetSonarValue(IN_4) < 35)
			{
				int port = rand() % 2;
				if(port == 1)
					port = 2;
				NXT::Motor::SetReverse(port, 50);
				while(NXT::Sensor::GetSonarValue(IN_4) < 27){}
				NXT::Motor::SetForward(port, 50);
			}
			if(NXT::Sensor::GetValue(IN_1))
				BackUp();
		}
	}
	NXT::Close(); //close the NXT
	return 0;
}

void BackUp()
{
	NXT::Motor::SetReverse(OUT_A, 50);
	NXT::Motor::SetReverse(OUT_C, 50);
	Wait(1500);
	if(rand() % 2 == 0)
	{
		NXT::Motor::SetForward(OUT_A, 50);
		Wait(500);
		NXT::Motor::SetForward(OUT_C, 50);
	}
	else
	{
		NXT::Motor::SetForward(OUT_C, 50);
		Wait(500);
		NXT::Motor::SetForward(OUT_A, 50);
	}
}