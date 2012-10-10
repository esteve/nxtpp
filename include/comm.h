// File: comm.h
//
// Copyright (c) 2007 by XXX
// -------------------------------------------------------------------------
// DESCRIPTION
// Header file containing OS specific function. 
//
// HISTORY
// Date         By who         Description
// -------------------------------------------------------------------------
// 04/26/2007   maddock     Header comments
// 05/12/2007   phyizal     Added fantom file support
// 05/16/2007   phyizal     Removed fantom function call 

#ifndef _COMM_H_
#define _COMM_H_

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "fantom/iNXT.h"
#include "fantom/iNXTIterator.h"
#include "fantom/tStatus.h"

//Error codes.
//TODO: Make them more comprehensive based on status code from NXT.
//       Also provide a method to translate numeric error code to more
//       user friendly error string.

enum ErrorCode
{		
	UnsupportedType = -1,
	IteratorFailed = -2,
	InvalidFileIterator = -3,
	IteratorAdvanceFailed = -4,
	InvalidFileHandler = -5,
	FileOpenFailed = -6,		
	WriteNotEnabled = -7,
	FileNotOpen = -8,		
	ReadFailed = -9,
	WriteFailed = -10,
	FileCloseFailed = -11,		
	WriteBufferIllegalSize = -12,
};

//! Abstraction for NXT iFile interface
struct NXTFile
{
	//! The NXTFile constructor.
	NXTFile() : file(NULL) , name(NULL)
	{
		size = 0;
		isOpen = false;
		isWriteable = false;
	}

	//! This constructs an NXTFile from the Fantom file type. Only in the Windows version.
	NXTFile(nFANTOM100::iFile* f): name((char*) malloc(20))
	{
      file = f;

	  if (f == NULL) return;

	  f->getName(name);

	  nFANTOM100::tStatus status;

	  size = f->getSize(status);

	  if (status.isFatal())
		  size = -1;

	  isOpen = false;
	  isWriteable =false;
	}

	//! The name of the file.
	char * name;
	//! The size in bytes of the file.
	int size;
	//! The mode the file is currently in.
	int mode;

	//! Whether the file is open or not.
	bool isOpen;
	//! Whether the file is writeable or not.
	bool isWriteable;

	//Won't be use in case of linux
	//! The Fantom file pointer. Only in the Windows version.
	nFANTOM100::iFile* file; 
	bool isValid();	
	int open(int mode, int size=100);
	int remove();
	int read(char* buff, int size);
	int write(char* buff, int size);
	int close();
};

//! Abstraction for NXT iFileIterator interface.
struct NXTFileIterator
{
	//! The NXTFileIterator constructor.
	NXTFileIterator() : iter(NULL)
	{
	}

	//! This constructs an NXTFileIterator from the Fantom file iterator type. Only in the Windows version.
	NXTFileIterator(nFANTOM100::iFileIterator *i)
	{
		iter = i;
	}

	//! The Fantom file iterator pointer. Only in the Windows version.
	nFANTOM100::iFileIterator *iter;
	//! Checks whether or not the NXTFileIterator is valid.
	bool isValid();
	//! Iterates to the next file in the NXTFileIterator.
	int next();
	//! Closes the NXTFileIterator.
	int close();
	//! Retrieves the current file into an NXTFile type.
	int getFile(NXTFile& file);
	//! Returns the size of the current file.
	int getCurrFileSize();
	//! Returns the name of the current file.
	const char* getCurrFileName();
	
};

//! Abstraction for NXT iModule interface.
struct NXTModule
{
	//! The NXTModule constructor.
	NXTModule() : internalModule(NULL)
	{
	}

	//! This constructs an NXTModule from the Fantom module type. Only in the Windows version.
	NXTModule(nFANTOM100::iModule *m)
	{
		internalModule = m;
	}

	//! The Fantom module pointer. Only in the Windows version.
	nFANTOM100::iModule *internalModule;
	//! Retrieves the name of the module.
	int getName(std::string &name);
	//! Retrieves the ID of the module.
	int getModuleID(int &id);
	//! Retrieves the size of the module.
	int getModuleSize(int &size);
	//! Retrieves the IOMap size.
	int getIOMapSize(int &size);
	//! Reads the IOMap into a buffer.
	int readIOMap(ViPByte buffer, int offset, int number_of_bytes);
	//! Writes data to the IOMap.
	int writeIOMap(const ViPByte, int offset, int number_of_bytes);

};

//! Abstraction for NXT iModuleIterator interface.
struct NXTModuleIterator
{
	//! The NXTModuleIterator
	NXTModuleIterator() : internalIter(NULL)
	{
	}

	//! This constructs an NXTModule from the Fantom module type. Only in the Windows version.
	NXTModuleIterator(nFANTOM100::iModuleIterator *i)
	{
		internalIter = i;
	}

	//! The Fantom module iterator pointer. Only in the Windows version.
	nFANTOM100::iModuleIterator *internalIter;
	//! Iterates to the next module in the NXTModuleIterator.
	int next();
	//! Retrieves the current module into an NXTModule type.
	int getModule(NXTModule &module);
	//! Returns the name of the current module.
	int getName(std::string &s);
};



bool Open();
bool OpenBT();
void Close();
void SendDirectCommand(ViBoolean response, ViByte * dc_buf, int dc_buf_size, ViByte *re_buf, int re_buf_size);
double GetProtocolVersion();
double GetFirmwareVersion();
std::string GetName();
double GetAvailableFlash();
//bool Write();


//! NXT++ communication helper functions. Basic users should not need to use this.
namespace Comm
{
	bool Open();
	
	bool OpenBT();
	
	void Close();
	
	void SendDirectCommand(ViBoolean response, ViByte * dc_buf, int dc_buf_size, ViByte *re_buf, int re_buf_size);
	
	// Why is this in here? It's not refrenced by anything
	/* bool Write()
	{
		ViUInt8 sysCommandBuffer[] = {0x01, 0x88};
		
		/*, 0x2A, 0x2E, 0x72, 0x78, 0x65, 0x00, 0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};*//*
        
		nxtPtr->write(reinterpret_cast<ViByte*> (sysCommandBuffer), sizeof (sysCommandBuffer), status);

		
		ViUInt8 sysCommandResponse[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		nxtPtr->read(reinterpret_cast<ViPBuf> (sysCommandResponse), 7, status);


		std::cout<<(int)sysCommandResponse[3]<<std::endl;

		return status.isFatal();
	} */

	double GetProtocolVersion();
	
	double GetFirmwareVersion();
	
	std::string GetName();

	//! Returns the amount of flash memory left
	double GetAvailableFlash();

	//! Retrieves a specified file into an NXTFile.
	NXTFile GetNXTFile(std::string filename, bool &isFatal);

	//! Searches for files on the NXT and stores the results in an NXTFileIterator.
	NXTFileIterator GetFileIterator(std::string pattern, bool &isFatal);

	//!Destroys an NXTFileIterator.
	void destroyFileIterator(NXTFileIterator &iter);
	//! Destroys an NXTFile.
	void destroyFile(NXTFile &file);

	//! Searches for modules on the NXT and stores the results in an NXTModuleIterator.
	NXTModuleIterator GetModuleIterator(std::string pattern, int &statusCode);

	//! Destroys an NXTModuleIterator.
	void destroyModuleIterator(NXTModuleIterator &iter);

	//! Retrieves a specified module into an NXTModule.
	NXTModule GetModule(std::string moduleName, int moduleID, int moduleSize, int ioMapSize, int &statCode);

	//! Destroys an NXTModule.
	void destroyModule(NXTModule &module);
};

#endif
