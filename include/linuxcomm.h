
#ifndef _LINUXCOMM_H_
#define _LINUXCOMM_H_

// File: linuxcomm.h
//
// Copyright (c) 2007 by XXX
// -------------------------------------------------------------------------
// DESCRIPTION
// Header file containing OS specific function. 
//
// HISTORY
// Date         By who         Description
// -------------------------------------------------------------------------
// 04/26/2007   maddock     Porting functions from win32 version

#include <cstring>
#include <iostream>
#include <usb.h>

#include "visatype.h"

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
	NXTFile()
	{
		size = 0;
		isOpen = false;
		isWriteable = false;
	}

	NXTFile(int filed, char *n, int s)
	{
		fd = filed;
		name = (char *) malloc(strlen(n));
		memcpy(name, n, strlen(n));
		size = s;
	}

	char *name;
	int fd;
	int size;
	int mode;

	bool isOpen;
	bool isWriteable;

	bool isValid();
	int open(int mode, int size = 100);
	int remove();
	int read(char *buff, int size);
	int write(char *buff, int size);
	int close();
};

//! Abstraction for NXT iFileIterator interface.
struct NXTFileIterator
{
	NXTFileIterator()
	{
	}

	NXTFileIterator(int desc, char *currFN, int currFileS)
	{
		fd = desc;
		currFileName = currFN;
		currFileSize = currFileS;
	}

	int fd;
	char *currFileName;
	int currFileSize;

	bool isValid();
	int next();
	int close();
	int getFile(NXTFile & file);
	int getCurrFileSize();
	const char *getCurrFileName();
};

struct NXTModule
{
	NXTModule()
	{
	}

	int getName(std::string & name);
	int getModuleID(int &id);
	int getModuleSize(int &size);
	int getIOMapSize(int &size);
	int readIOMap(ViPByte buffer, int offset, int number_of_bytes);
	int writeIOMap(const ViPByte, int offset, int number_of_bytes);
	
};

struct NXTModuleIterator
{
	NXTModuleIterator()
	{
	}

	int next();
	int getModule(NXTModule & module);
	int getName(std::string & s);
};

namespace Comm
{
	namespace USB
	{
		/* USB specific */
		static usb_dev_handle *pUSBHandle = 0;
		static unsigned char ucEpOut = 0;
		static unsigned char ucEpIn = 0;
		static const int TIMEOUT = 0x0BB8;

		static struct usb_device *DevInit(void);
		static bool DevOpen();
		static int DevReset();
		static void DevWrite(ViByte * buf, int buf_size);
		static void DevRead(ViByte * buf, int buf_size);
		static void DevClose();
	}

	namespace BT
	{
	}
  
	bool Open();
	bool OpenBT();
	void Close();
	void SendDirectCommand(ViBoolean response, ViByte * dc_buf,
		int dc_buf_size, ViByte * re_buf, int re_buf_size);
	void SendSystemCommand(ViBoolean response, ViByte * dc_buf,
		int dc_buf_size, ViByte * re_buf, int re_buf_size);
	double GetProtocolVersion();
	double GetFirmwareVersion();
	void GetDeviceInfo(ViUInt8 * outBuf, int size);
	std::string GetName();
	double GetAvailableFlash();
	NXTFileIterator GetFileIterator(std::string pattern, bool & isFatal);
	NXTFile GetNXTFile(std::string filePattern, bool & isFatal);
	void destroyFileIterator(NXTFileIterator & iter);
	void destroyFile(NXTFile & file);
	NXTModuleIterator GetModuleIterator(std::string pattern, int &statusCode);
	void destroyModuleIterator(NXTModuleIterator & iter);
	NXTModule GetModule(std::string moduleName, int moduleID, int moduleSize,
		int ioMapSize, int &statCode);
	void destroyModule(NXTModule & module);
}

#endif
