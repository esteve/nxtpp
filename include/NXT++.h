// File: NXT++.h
//
// Copyright (c) 2007 by XXX
// -------------------------------------------------------------------------
// DESCRIPTION
// NXT++.h is a header file containing an interface for programming Lego NXT 
// Mindstorms. 
//
// HISTORY
// Date         By who         Description
// -------------------------------------------------------------------------
// 04/26/2007   maddock     OS specific includes, comment headers
// 05/16/2007   phyizal     Removed fantom function call 

#ifndef _NXTPP_H_
#define _NXTPP_H_

#if ( defined( _MSC_VER ) && ( defined( _M_IX86 ) || defined( _M_I86 )))

#include "comm.h"

#elif defined(linux) && (defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(__x86_64__))   

#include "linuxcomm.h"

#endif

#include <time.h>
#include <math.h>
#include <string>
#include <iostream>

//! Defines different types of sensors.
enum SensorType
{
	Touch, /*!< A touch sensor. GetValue() returns a boolean.*/
	Sound, /*!< A sound sensor. GetValue() returns a decibel value.*/
	Light, /*!< A light sensor with the infrared light off. GetValue() returns a percent.*/
	ActiveLight, /*!< A light sensor with the infrared light on. GetValue() returns a percent.*/
	Sonar /*!< An ultrasonic sensor. GetValue() returns a distance in centimeters.*/
};
//! Initiates the NXT connection.
/*! Returns true if it completed without error. Returns false if it failed. You should not continue to
use the interface functions if this happens. Refer to example.cpp for info on how to properly handle this.*/
bool Open();
//! Closes the NXT connection.
void Close();
//! Plays a tone.
/*! \param frequency The frequency in hertz. \param duration The duration in milliseconds.*/
void PlayTone(int frequency, int duration);
//! Returns the rotation of a motor.
/*! \param port The port that you wish to retrieve the rotation. Can be the numbers 0-2 or OUT_A, OUT_B,
or OUT_C. \return The rotation of the motor in degrees.*/
int GetRotationCount(int port);
//! Returns the current battery level.
/*! \return The current battery level in millivolts.*/
int BatteryLevel();
//! Sets a sensor to a specified SensorType.
/*! \param port The port that you wish perform this operation on. Can be the numbers 0-3 or IN_1, IN_2,
IN_3, or IN_4. \param type The SensorType that you wish to set the sensor to.*/
void Set(int port, SensorType type);
//! Retrieves the value of a sensor.
/*! \param port The port that you wish to retrieve it's sensor's value from. Can be the numbers 0-3 or
IN_1, IN_2, IN_3, or IN_4. \return The sensor's current value.*/
int GetValue(int port);
//! Resets the specified motor's rotation count.
/*! \param port The port that you wish to reset the rotation. Can be the numbers 0-2 or OUT_A, OUT_B,
or OUT_C. \param relative If true it resets the rotation relative to the last movement. If false it
resets the rotation to its absolute position.*/
void ResetRotationCount(int port, bool relative);
//! Sets a motor to turn forward at a specified power.
/*! \param port The port that you wish to set. Can be the numbers 0-2 or OUT_A, OUT_B, or OUT_C. 
\param power The power level (0-100) that you want the motor to recieve.*/
void SetForward(int port, int power);
//! Sets a motor to turn backward at a specified power.
/*! \param port The port that you wish to set. Can be the numbers 0-2 or OUT_A, OUT_B, or OUT_C.
\param power The power level (0-100) that you want the motor to recieve.*/
void SetReverse(int port, int power);
//! Tells a motor to go to a certian angle and then stop.
/*! \param port The port that you wish to set. Can be the numbers 0-2 or OUT_A, OUT_B, or OUT_C.
\param power The power level (0-100) that you want the motor to recieve. \param tacho The rotation
count (in degrees) that you want the motor to turn to. \param brake Whether or not you want the motor
to brake or float when it is done turning.*/
void GoTo(int port, int power, int tacho, bool brake);
//! Tells a motor to stop turning.
/*! \param port The port that you wish to turn off. Can be the numbers 0-2 or OUT_A, OUT_B, or OUT_C.
\param brake Whether or not you want the motor to brake or float.*/
void Stop(int port, bool brake);
//! Sets a sensor in a specified port to a touch sensor.
/*! \param port The port that you wish to set as a touch sensor. Can be the numbers 0-3 or IN_1, IN_2,
IN_3, or IN_4.*/
void SetTouch(int port);
//! Sets a sensor in a specified port to a sound sensor.
/*! \param port The port that you wish to set as a sound sensor. Can be the numbers 0-3 or IN_1, IN_2,
IN_3, or IN_4.*/
void SetSound(int port);
//! Sets a sensor in a specified port to a light sensor.
/*! \param port The port that you wish to set as a light sensor. Can be the numbers 0-3 or IN_1, IN_2,
IN_3, or IN_4. \param active Whether or not you want the infrared light to be on.*/;
void SetLight(int port, bool active);
//! Sets a sensor in a specified port to a sonar sensor.
/*! \param port The port that you wish to set as a sonar sensor. Can be the numbers 0-3 or IN_1,
IN_2, IN_3, or IN_4.*/;
void SetSonar(int port);
//! Waits a specified time.
/*! \param time The time to wait in milliseconds.*/;
void Wait(int time);
//! Tells a motor to turn the brake on.
/*! \param port The port that you wish to turn the brake on. Can be the numbers 0-2 or OUT_A, OUT_B, or OUT_C.*/
void BrakeOn(int port);
//! Tells a motor to turn the brake off.
/*! \param port The port that you wish to turn the brake off. Can be the numbers 0-2 or OUT_A, OUT_B, or OUT_C.*/
void BrakeOff(int port);
//! Gets the name of the NXT.
/*! \return The name of the NXT.*/
std::string GetName();
//! Gets the amount of availiable flash memory on the NXT.
/*! Does not work with linux yet. \return The amount of free flash memory in kilobytes.*/
double GetAvailableFlash();
//! Stops the current program if one is running.
void StopProgram();
//! Gets the protocol version of the NXT.
/*! Does not work with linux yet. \return The protocol version of the NXT.*/
double GetProtocolVersion();
//! Gets the firmware version of the NXT.
/*! Does not work with linux yet. \return The firmware version of the NXT.*/
double GetFirmwareVersion();
//! Initiates the NXT connection through Bluetooth.
/*! Returns true if it completed without error. Returns false if it failed. You should not continue to
use the interface functions if this happens. Refer to example.cpp for info on how to properly handle this.
Does not work with linux yet.*/
bool OpenBT();
//! Gets the status of the lowspeed port.
/*! Most users should not need to use this. The function does not get the value of the sensor. Not to be 
confused with GetSonarValue(). \param port The port that you wish to get the status of. Can be the numbers
0-3 or IN_1, IN_2, IN_3, or IN_4. \return The status of the specified port.*/
int LSGetStatus(int port);
//! Retrieves the value for sonar sensors.
/*! \param port The port that contains the sonar sensor. Can be the numbers 0-3 or IN_1, IN_2, IN_3, or IN_4.
\return The sonar sensor's current value in centimeters.*/
int GetSonarValue(int port);
//! Tells the sonar sensor to stop sending pulses.
/*! \param port The port that contains the sonar sensor. Can be the numbers 0-3 or IN_1, IN_2, IN_3, or IN_4.*/
void SetSonarOff(int port);
//! Tells the sonar sensor to only send a pulse when you ask it what it's value is.
/*! \param port The port that contains the sonar sensor. Can be the numbers 0-3 or IN_1, IN_2, IN_3, or IN_4.*/
void SetSonarSingleShot(int port);
//! Tells the sonar sensor to continuously send pulses.
/*! \param port The port that contains the sonar sensor. Can be the numbers 0-3 or IN_1, IN_2, IN_3, or IN_4.*/
void SetSonarContinuous(int port);
//! Sets the interval at which the sonar sensor sends a pulse.
/*! \param port The port that contains the sonar sensor. Can be the numbers 0-3 or
IN_1, IN_2, IN_3, or IN_4. \param interval The interval at which the sonar sensor should send a pulse in
miliseconds.*/
void SetSonarContinuousInterval(int port, int interval);
//! Starts a program on the NXT with a specified name.
/*! \param name The filename of the program you wish to execute. Don't forget to include the '.rxe'!*/
void StartProgram(std::string name);
//! Plays a sound file on the NXT with a specified name.
/*! \param name The filename of the sound you wish to play. Don't forget to include the '.rso'!*/
void PlaySoundFile(std::string name, bool loop);
//! Sets a sensor to return a raw value (not formatted in any way).
/*! \param port The port that you wish to set to return a raw value. Can be the numbers 0-3 or IN_1,
IN_2, IN_3, or IN_4.*/
void SetRaw(int port);
//! Sends a direct command to the NXT.
/*! This low-level function is used to send commands to the NXT - just like all the other functions do.
The only difference is that this function is harder to use, but in turn, provides much more control of the
NXT. For a reference about direct commands, look in the PDF titled Appendix 2. \param response Whether or not
you want to recieve a response from the direct command. \param dc_buf The buffer that contains the direct 
command. \param dc_buf_size The number of bytes in the direct command buffer. \param re_buf The buffer that
will contain the response to the direct command if specified. \param re_buf_size The number of bytes in the
response buffer.*/
void SendDirectCommand(ViBoolean response, ViByte * dc_buf, int dc_buf_size, ViByte *re_buf, int re_buf_size);
//! Resets the internal sleep timer.
/*! This function is used so the NXT will not automatically turn off while the computer is still
controlling it. Note: an alternative to this is to set the shutoff time to never in the NXT settings
menu.*/
void KeepAlive();
//! Stops the currently playing sound.
void StopSound();

/* TEST FUNCTIONS
void MessageSend(std::string message, int inbox);
std::string MessageRecieve(int reminbox, int locinbox, bool remove);
std::string GetCurrentProgramName();
*/

//! Motor port A on the NXT
const int OUT_A = 0;
//! Motor port B on the NXT
const int OUT_B = 1;
//! Motor port C on the NXT
const int OUT_C = 2;
//! Sensor port 1 on the NXT
const int IN_1 = 0;
//! Sensor port 2 on the NXT
const int IN_2 = 1;
//! Sensor port 3 on the NXT
const int IN_3 = 2;
//! Sensor port 4 on the NXT
const int IN_4 = 3;

//! Contains all the functions that have to do with the NXT.
namespace NXT
{
	bool Open();

	bool OpenBT();
	
	void Close();
	
	void PlayTone(int frequency, int duration);
	
	int BatteryLevel();

	std::string GetName();

	double GetAvailableFlash();

	void StopProgram();

	double GetProtocolVersion();

	double GetFirmwareVersion();

	void StartProgram(std::string name);

	void PlaySoundFile(std::string name, bool loop);

	void StopSound();

	/* DOESN'T WORK YET - DON'T TRY - JUST TEST CODE
	void MessageSend(std::string message, int inbox)
	{
			//ViUInt8 directCommandBuffer[] = { 0x0A, port, relative };
			//Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
		int length = message.size();
		const char *charmessage = message.c_str();
		//ViUInt8 *directCommandBuffer  = (ViUInt8*) malloc(3+length);
		ViUInt8* directCommandBuffer = NULL;
		directCommandBuffer  = new ViUInt8[3+length+1];
		directCommandBuffer[0] = 0x09;
		directCommandBuffer[1] = inbox-1;
		directCommandBuffer[2] = length+1;
		for(int i = 3; i < 3+length; i++)
			directCommandBuffer[i] = charmessage[i-3];
		directCommandBuffer[length+3] = NULL;
		for(int j = 0; j < 4+length; j++)
			std::cout << (int)directCommandBuffer[j] << "\n";
		//ViUInt8 directCommandBuffer[] = { 0x01 };
		Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
	}

	std::string MessageRecieve(int mailbox, bool remove)
	{
		ViUInt8 directCommandBuffer[] = { 0x13, mailbox, 0x00, remove };
		ViUInt8 responseBuffer[64];
		for(int i = 0; i < 64; i++)
			responseBuffer[i] = 0x00;
	
		// Send the direct command to the NXT.
		Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
			reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));
		for(int j = 0; j < 64; j++)
			std::cout << (int)responseBuffer[j] << "\n";
	
		return "";
	}
	*/

	void SendDirectCommand(ViBoolean response, ViByte * dc_buf, int dc_buf_size, ViByte *re_buf, int re_buf_size);

	/* DOESN'T WORK YET - DON'T TRY - JUST TEST CODE
	std::string GetCurrentProgramName()
	{
		ViUInt8 directCommandBuffer[] = { 0x11 };
		ViUInt8 responseBuffer[] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
	
		// Send the direct command to the NXT.
		Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
			reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));

		std::string name = "";
		for(int i = 3; i < 23; i++)
			name += (char)responseBuffer[i];
	
		return name;
	}
	*/

	void KeepAlive();
	
	//! Contains all the functions that have to do with a motor.
	namespace Motor
	{
		int GetRotationCount(int port);
	
		void ResetRotationCount(int port, bool relative);
		
		void SetForward(int port, int power);
		
		void SetReverse(int port, int power);
		
		void Stop(int port, bool brake);
		
		void BrakeOn(int port);
		
		void BrakeOff(int port);
		
		void GoTo(int port, int power, int tacho, bool brake);
	}
	
	//! Contains all the functions that have to do with a sensor.
	namespace Sensor
	{
		void SetTouch(int port);
		
		void SetSound(int port);
		
		void SetLight(int port, bool active);

		void SetSonar(int port);

		void SetRaw(int port);
		
		void Set(int port, SensorType type);
		
		int GetValue(int port);

		int LSGetStatus(int port);

		int GetSonarValue(int port);

		void SetSonarOff(int port);

		void SetSonarSingleShot(int port);

		void SetSonarContinuous(int port);

		void SetSonarContinuousInterval(int port, int interval);
	}

    
	//! NXT file functions.
	namespace File
	{		
	    //! Defines different types of resource files.
		enum FileType
		{
			ANY,      /*!< *.*, All files. */
			PROGRAM,  /*!< *.rcx, A user define program.  */
			SOUND,    /*!< *.rso A sound file. */
			GRAPHICS, /*!< *.ric. A graphics file. */
		    DATALOG,  /*! < *.rdt. */
		};
		
	    //! Defines different types of file modes.
		enum FileMode
		{
			READ,
			WRITE,
			APPEND,
			DATA_WRITE,			
			LINEAR_WRITE,
		};

		//! Retrieves a specific NXT file pointer by name.
		int getNXTFile(std::string filename, NXTFile& file);

		//! Retrieves NXT file pointers by name.
		int getNXTFilesByName(std::string pattern, NXTFileIterator& iter);

		//! Retrieves NXT file pointers by type.
		int getNXTFilesByType(int fileType, NXTFileIterator& iter);

		//! Destroys a file iterator.
		void destroyFileIterator(NXTFileIterator &iter);

		//! Destroys a file pointer
		void destroyFile(NXTFile& file);

	};

	//! NXT module functions.
	namespace Module
	{
		//! Retrieves NXT module pointers by name.
		int getNXTModules(std::string pattern, NXTModuleIterator &iter);

		//! Retrieves a specific module pointer by name.
		int getNXTModule(std::string pattern, int moduleID, int moduleSize,
			int ioMapSize, int &status, NXTModule &module);
	};
}

void Wait(int time);

#endif
