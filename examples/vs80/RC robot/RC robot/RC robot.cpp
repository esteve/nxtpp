#include "NXT++.h"
#include <windows.h>
#include <iostream>
using namespace std;

bool UP = false;
bool DOWN = false;
bool RIGHT = false;
bool LEFT = false;
int laststate = 0;
int a = 0;
int c = 0;
int lasta = 0;
int lastc = 0;

int main()
{
	if(NXT::OpenBT()) //initialize the NXT and continue if it succeeds
	{
		NXT::Sensor::SetSonar(IN_4);
		NXT::Sensor::SetTouch(IN_1);
		while(1)
		{
			UP = GetAsyncKeyState(VK_UP);
			DOWN = GetAsyncKeyState(VK_DOWN);
			RIGHT = GetAsyncKeyState(VK_RIGHT);
			LEFT = GetAsyncKeyState(VK_LEFT);

			if((UP) && (!DOWN) && (!RIGHT) && (!LEFT))
			{
				a = 50;
				c = 50;
			}
			else if((!UP) && (DOWN) && (!RIGHT) && (!LEFT))
			{
				a = -50;
				c = -50;
			}
			else if((!UP) && (!DOWN) && (RIGHT) && (!LEFT))
			{
				a = -50;
				c = 50;
			}
			else if((!UP) && (!DOWN) && (!RIGHT) && (LEFT))
			{
				a = 50;
				c = -50;
			}
			else if((UP) && (!DOWN) && (RIGHT) && (!LEFT))
			{
				a = 25;
				c = 50;
			}
			else if((UP) && (!DOWN) && (!RIGHT) && (LEFT))
			{
				a = 50;
				c = 25;
			}
			else if((!UP) && (DOWN) && (RIGHT) && (!LEFT))
			{
				a = -25;
				c = -50;
			}
			else if((!UP) && (DOWN) && (!RIGHT) && (LEFT))
			{
				a = -50;
				c = -25;
			}
			else
			{
				a = 0;
				c = 0;
			}

			if((a == 0) && (c == 0))
			{
				if(GetAsyncKeyState(VK_RETURN))
					NXT::PlayTone(400, 75);
			}
			else if((a > 0) && (c > 0))
			{
				if(NXT::Sensor::GetSonarValue(IN_4) < 30)
				{
					a = 0;
					c = 0;
				}
				else if(NXT::Sensor::GetValue(IN_1))
				{
					a = 0;
					c = 0;
				}
			}

			if(lasta != a)
			{
				if(a == 0)
					NXT::Motor::Stop(OUT_A, false);
				else
					NXT::Motor::SetForward(OUT_A, a);
				lasta = a;
			}
			if(lastc != c)
			{
				if(c == 0)
					NXT::Motor::Stop(OUT_C, false);
				else
					NXT::Motor::SetForward(OUT_C, c);
				lastc = c;
			}
		}
		NXT::Close(); //close the NXT
	}
	return 0;
}