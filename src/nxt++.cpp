#include "NXT++.h"

using namespace NXT;
using namespace NXT::File;
using namespace NXT::Module;
using namespace NXT::Motor;
using namespace NXT::Sensor;

bool NXT::Open()
{
	return Comm::Open();
}

bool NXT::OpenBT()
{
	return Comm::OpenBT();
}

void NXT::Close()
{
	Comm::Close();
}

void NXT::PlayTone(int frequency, int duration)
{
	// This is a direct command to play a tone.
	ViUInt8 directCommandBuffer[] = { 0x03, frequency%256, (frequency-(frequency%256))/256, duration%256, (duration-(duration%256))/256 };

	// Send the direct command to the NXT.
	Comm::SendDirectCommand( false /* a response is not required for this direct command */,
		reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
		NULL /* no response buffer */, 0 /* no response buffer, specify 0 for size */);
}

int NXT::BatteryLevel()
{
	ViUInt8 directCommandBuffer[] = { 0x0B };
	ViUInt8 responseBuffer[] = { 0x01, 0x01, 0x01, 0x01 };

	// Send the direct command to the NXT.
	Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
		reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));

	return responseBuffer[3]*256+responseBuffer[2];
}

std::string NXT::GetName()
{
	return Comm::GetName();
}

double NXT::GetAvailableFlash()
{
	return Comm::GetAvailableFlash();
}

void NXT::StopProgram()
{
	ViUInt8 directCommandBuffer[] = { 0x01 };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

double NXT::GetProtocolVersion()
{
	return Comm::GetProtocolVersion();
}

double NXT::GetFirmwareVersion()
{
	return Comm::GetFirmwareVersion();
}

void NXT::StartProgram(std::string name)
{
	char fname[20];
	strcpy(fname, name.c_str());
	ViUInt8 directCommandBuffer[21];
	directCommandBuffer[0] = 0x00;
	for(int i = 0; i < 20; i++)
		directCommandBuffer[i+1] = fname[i];
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::PlaySoundFile(std::string name, bool loop)
{
	char fname[20];
	strcpy(fname, name.c_str());
	ViUInt8 directCommandBuffer[21];
	directCommandBuffer[0] = 0x02;
	directCommandBuffer[1] = loop;
	for(int i = 0; i < 20; i++)
		directCommandBuffer[i+2] = fname[i];
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::StopSound()
{
	ViUInt8 directCommandBuffer[] = { 0x0C };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

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

void NXT::SendDirectCommand(ViBoolean response, ViByte * dc_buf, int dc_buf_size, ViByte *re_buf, int re_buf_size) 
{
	Comm::SendDirectCommand(response, dc_buf, dc_buf_size, re_buf, re_buf_size);
}

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

void NXT::KeepAlive()
{
	ViUInt8 directCommandBuffer[] = { 0x0D };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}



//
int NXT::Motor::GetRotationCount(int port)
{
	ViUInt8 directCommandBuffer[] = { 0x06, port };
	ViInt8 responseBuffer[] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

	// Send the direct command to the NXT.
	Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
		reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));

	int i = responseBuffer[20];
	if(i < 0)
		i = 256 + i;
	if(responseBuffer[22] == -1)
		responseBuffer[22] = 0;
	if(responseBuffer[23] == -1)
		responseBuffer[23] = 0;

	int tacho = responseBuffer[23]*16777216+responseBuffer[22]*65536+responseBuffer[21]*256+i;

	return tacho;
}

void NXT::Motor::ResetRotationCount(int port, bool relative)
{
	ViUInt8 directCommandBuffer[] = { 0x0A, port, relative };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::Motor::SetForward(int port, int power)
{
	ViUInt8 directCommandBuffer[] = { 0x04, port, power, 0x01 | 0x04, 0x01, 0, 0x20, 0, 0, 0, 0 };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::Motor::SetReverse(int port, int power)
{
	ViUInt8 directCommandBuffer[] = { 0x04, port, -power, 0x01 | 0x04, 0x01, 0, 0x20, 0, 0, 0, 0 };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::Motor::Stop(int port, bool brake)
{
	if(brake)
	{
		ViUInt8 directCommandBuffer[] = { 0x04, port, 0, 0x01 | 0x02 | 0x04, 0x01, 0, 0x20, 0, 0, 0, 0 };
		Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
	}
	else
	{
		ViUInt8 directCommandBuffer[] = { 0x04, port, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0};
		Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
	}
}

void NXT::Motor::BrakeOn(int port)
{
	ViUInt8 directCommandBuffer[] = { 0x04, port, 0, 0x01 | 0x02 | 0x04, 0x01, 0, 0x20, 0, 0, 0, 0 };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::Motor::BrakeOff(int port)
{
	ViUInt8 directCommandBuffer[] = { 0x04, port, 0, 0x00, 0x00, 0, 0x00, 0, 0, 0, 0};
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::Motor::GoTo(int port, int power, int tacho, bool brake)
{
	if(tacho >= GetRotationCount(port))
	{
		SetForward(port, power);
		while(GetRotationCount(port) < tacho) {}
		Stop(port, brake);
	}
	else
	{
		SetReverse(port, power);
		while(GetRotationCount(port) > tacho) {}
		Stop(port, brake);
	}
}

void NXT::Sensor::SetTouch(int port)
{
	ViUInt8 directCommandBuffer[] = { 0x05, port, 0x01, 0x20 };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::Sensor::SetSound(int port)
{
	ViUInt8 directCommandBuffer[] = { 0x05, port, 0x07, 0x80 };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::Sensor::SetLight(int port, bool active)
{
	if(active)
	{
		ViUInt8 directCommandBuffer[] = { 0x05, port, 0x05, 0x80 };
		Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
	}
	else
	{
		ViUInt8 directCommandBuffer[] = { 0x05, port, 0x06, 0x80 };
		Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
	}
}

void NXT::Sensor::SetSonar(int port)
{
	ViUInt8 directCommandBuffer[] = { 0x05, port, 0x0B, 0x00 };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::Sensor::SetRaw(int port)
{
	ViUInt8 directCommandBuffer[] = { 0x05, port, 0x07, 0x00 };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

void NXT::Sensor::Set(int port, SensorType type)
{
	int msgtype, mode;
	switch (type)
	{
	case Touch:
		msgtype = 0x01;
		mode = 0x20;
		break;
	case Sound:
		msgtype = 0x07;
		mode = 0x80;
		break;
	case Light:
		msgtype = 0x06;
		mode = 0x80;
		break;
	case ActiveLight:
		msgtype = 0x05;
		mode = 0x80;
		break;
	case Sonar:
		msgtype = 0x0B;
		mode = 0x00;
		break;
	}
	ViUInt8 directCommandBuffer[] = { 0x05, port, msgtype, mode };
	Comm::SendDirectCommand( false, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ), NULL, 0);
}

int NXT::Sensor::GetValue(int port)
{
	ViUInt8 directCommandBuffer[] = { 0x07, port };
	ViUInt8 responseBuffer[] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

	Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
		reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));

	return responseBuffer[12]*256+responseBuffer[11];
}

int NXT::Sensor::LSGetStatus(int port)
{
	ViUInt8 directCommandBuffer[] = {0x0E, port};
	ViUInt8 responseBuffer[] = {1,1,1};


	Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer),
		   reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));

	return (int)responseBuffer[2];
}

int NXT::Sensor::GetSonarValue(int port)
{

	ViUInt8 directCommandBuffer[] = {0x0F, port, 0x03, 0x00, 0x02, 0x41, 0x02};
	ViUInt8 responseBuffer[] = { 1,1};

	Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
		reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));

	int bytesRead = 0;

	do {

	    ViUInt8 directCommandBuffer[] = {0x0F, port, 0x02, 0x01, 0x02, 0x42};
	    ViUInt8 responseBuffer[] = {1, 1};


	    Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
		        reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer));

         bytesRead = LSGetStatus(port);

	} while (bytesRead < 1);
	
	ViUInt8 directCommandBuffer2[] = {0x10, port};
	ViUInt8 responseBuffer2[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};


	Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer2 ), sizeof( directCommandBuffer2 ),
		reinterpret_cast< ViByte* >( responseBuffer2 ), sizeof( responseBuffer2 ));

	return (int)responseBuffer2[3];
	  
}

void NXT::Sensor::SetSonarOff(int port)
{
	ViUInt8 directCommandBuffer[] = {0x0F, port, 0x03, 0x00, 0x02, 0x41, 0x00};
	ViUInt8 responseBuffer[] = { 1,1};

	Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
		reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));
}

void NXT::Sensor::SetSonarSingleShot(int port)
{
	ViUInt8 directCommandBuffer[] = {0x0F, port, 0x03, 0x00, 0x02, 0x41, 0x01};
	ViUInt8 responseBuffer[] = { 1,1};

	Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
		reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));
}

void NXT::Sensor::SetSonarContinuous(int port)
{
	ViUInt8 directCommandBuffer[] = {0x0F, port, 0x03, 0x00, 0x02, 0x41, 0x02};
	ViUInt8 responseBuffer[] = { 1,1};

	Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
		reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));
}

void NXT::Sensor::SetSonarContinuousInterval(int port, int interval)
{
	ViUInt8 directCommandBuffer[] = {0x0F, port, 0x03, 0x00, 0x02, 0x40, interval};
	ViUInt8 responseBuffer[] = { 1,1};

	Comm::SendDirectCommand( true, reinterpret_cast< ViByte* >( directCommandBuffer ), sizeof( directCommandBuffer ),
		reinterpret_cast< ViByte* >( responseBuffer ), sizeof( responseBuffer ));
}

//! Retrieves a specific NXT file pointer by name.
int NXT::File::getNXTFile(std::string filename, NXTFile& file)
{
	bool isFatal;
	file = Comm::GetNXTFile(filename, isFatal);

	if (! isFatal)
		return 0;

	return -1;
}

//! Retrieves NXT file pointers by name.
int NXT::File::getNXTFilesByName(std::string pattern, NXTFileIterator& iter)
{			
	bool isFatal;
	iter = Comm::GetFileIterator(pattern, isFatal);
	
	if (!isFatal)
		return 0;
				
	//TODO: tStatus error code tranlation
	return IteratorFailed;
		
}

//! Retrieves NXT file pointers by type.
int NXT::File::getNXTFilesByType(int fileType, NXTFileIterator& iter)
{
	switch(fileType)
	{
	  case ANY:
		  return getNXTFilesByName("*.*", iter);
		  break;
	  case PROGRAM:
		  return getNXTFilesByName("*.rxe", iter);
		  break;

	  case SOUND:
		  return getNXTFilesByName("*.rso", iter);
		  break;

	  case GRAPHICS:
		  return getNXTFilesByName("*.ric", iter);
		  break;

	  case DATALOG:
		  return getNXTFilesByName("*.rdt", iter);
		  break;

	  default:
		  return UnsupportedType;

	}
	return 0;
}

//! Destroys a file iterator.
void NXT::File::destroyFileIterator(NXTFileIterator &iter)
{
	Comm::destroyFileIterator(iter);
}

//! Destroys a file pointer
void NXT::File::destroyFile(NXTFile& file)
{
	Comm::destroyFile(file);
}



//! Retrieves NXT module pointers by name.
int NXT::Module::getNXTModules(std::string pattern, NXTModuleIterator &iter)
{
	int status = 0;
	iter = Comm::GetModuleIterator(pattern, status);

	return status;
}

//! Retrieves a specific module pointer by name.
int NXT::Module::getNXTModule(std::string pattern, int moduleID, int moduleSize,
	int ioMapSize, int &status, NXTModule &module) 
{
	status = 0;
	module = Comm::GetModule(pattern, moduleID, moduleSize, ioMapSize, status);

	return status;			
}

void Wait(int time)
{
	int starttime = clock();
	while(clock()-starttime <= time) {}
}
