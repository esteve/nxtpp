
#include "linuxcomm.h"

bool Open();
bool OpenBT();
void Close();
void SendDirectCommand(ViBoolean response, ViByte * dc_buf, int dc_buf_size,
	ViByte * re_buf, int re_buf_size);
void SendSystemCommand(ViBoolean response, ViByte * dc_buf, int dc_buf_size,
	ViByte * re_buf, int re_buf_size);
double GetProtocolVersion();
double GetFirmwareVersion();
std::string GetName();
double GetAvailableFlash();

static const int NXT_VENDOR_ID = 0x0694;
static const int NXT_PRODUCT_ID = 0x0002;

void *status = 0;

static struct usb_device *Comm::USB::DevInit(void)
{
	struct usb_bus *usb_bus;
	struct usb_device *dev;

	usb_init();
	usb_find_busses();
	usb_find_devices();

	for (usb_bus = usb_busses; usb_bus; usb_bus = usb_bus->next)
	{
		for (dev = usb_bus->devices; dev; dev = dev->next)
		{
			if ((dev->descriptor.idVendor == NXT_VENDOR_ID) && (dev->descriptor.idProduct == NXT_PRODUCT_ID))
				return dev;
		}
	}
	return 0;
}

static bool Comm::USB::DevOpen()
{
	struct usb_device *usb_dev;
	int nEp = 0;

	usb_dev = DevInit();

	if (usb_dev == 0)
	{
		std::cerr << "Device not found!" << std::endl;
		return 0;
	}

	pUSBHandle = usb_open(usb_dev);

	if (pUSBHandle == 0)
	{
		std::cerr << "Not able to claim the USB device" << std::endl;
		return 0;
	}
	else
	{
		if (usb_dev->config)
		{
			if (usb_dev->config->interface)
			{
				if (usb_dev->config->interface->altsetting)
				{
					usb_claim_interface(pUSBHandle, usb_dev->config->interface->altsetting->bInterfaceNumber);
					if ((nEp = usb_dev->config->interface->altsetting->bNumEndpoints))
					{
						if (usb_dev->config->interface->altsetting->endpoint)
						{
							ucEpIn = (unsigned char) (usb_dev->config->interface->altsetting->endpoint[0].bEndpointAddress);
							if (nEp > 1)
								ucEpOut = (unsigned char) (usb_dev->config->interface->altsetting->endpoint[1].bEndpointAddress);

						}
					}
				}
			}
		}
	}
	return 1;
}

static int Comm::USB::DevReset()
{
	return usb_reset(pUSBHandle);
}

static void Comm::USB::DevWrite(ViByte * buf, int buf_size)
{
	if (pUSBHandle)
		usb_bulk_write(pUSBHandle, ucEpIn, reinterpret_cast < char *>(buf), buf_size, USB::TIMEOUT);
}

static void Comm::USB::DevRead(ViByte * buf, int buf_size)
{
	if (pUSBHandle)
		usb_bulk_read(pUSBHandle, ucEpOut, reinterpret_cast < char *>(buf), buf_size, USB::TIMEOUT);
}

static void Comm::USB::DevClose()
{
	usb_close(pUSBHandle);
}

bool Comm::Open()
{
	bool result = false;

	if (USB::DevOpen())
	{
		if (USB::DevReset() == 0)
			result = true;
	}
	return result;
}

bool Comm::OpenBT()
{
	return 0;
}

void Comm::Close()
{
	if (USB::pUSBHandle)
		USB::DevClose();
}

void Comm::SendDirectCommand(ViBoolean response, ViByte * dc_buf,
	int dc_buf_size, ViByte * re_buf, int re_buf_size)
{
	ViByte buf[dc_buf_size + 1];

	memcpy(buf + 1, dc_buf, dc_buf_size * sizeof(ViByte));
	buf[0] = response ? 0x00 : 0x80;

	USB::DevWrite(buf, dc_buf_size + 1);

	if (response)
	{
		ViByte tempreadbuf[re_buf_size + 1];
		USB::DevRead(tempreadbuf, re_buf_size + 1);

		for (int i = 1; i < re_buf_size + 1; i++)
			re_buf[i - 1] = tempreadbuf[i];
	}
}

void Comm::SendSystemCommand(ViBoolean response, ViByte * dc_buf,
	int dc_buf_size, ViByte * re_buf, int re_buf_size)
{
	ViByte buf[dc_buf_size + 1];

	memcpy(buf + 1, dc_buf, dc_buf_size * sizeof(ViByte));
	buf[0] = response ? 0x01 : 0x81;

	USB::DevWrite(buf, dc_buf_size + 1);

	if (response)
	{
		USB::DevRead(re_buf, re_buf_size);

		// DEBUG
		/*std::cout << "USB read returned package: " << std::endl;

		for(int i=0; i < re_buf_size; i++)
		{
			std::cout << "byte " << i << ": " << (int)re_buf[i] << std::endl;
		} */
		// END DEBUG
	}
}

double Comm::GetProtocolVersion()
{
	ViUInt8 inBuf[] = { 0x88 };
	ViUInt8 outBuf[7];
	const int min = 3, maj = 4;

	// Send the system command to the NXT.
	Comm::SendSystemCommand(true,
				reinterpret_cast < ViByte * >(inBuf),
				sizeof(inBuf),
				reinterpret_cast <
				ViByte * >(outBuf) /* no response buffer */ ,
				sizeof(outBuf));

	double version = outBuf[min];

	while (version >= 1)
		version /= 10;
	version += outBuf[maj];

	return version;
}

double Comm::GetFirmwareVersion()
{
	ViUInt8 inBuf[] = { 0x88 };
	ViUInt8 outBuf[7];
	const int min = 5, maj = 6;

	// Send the system command to the NXT.
	Comm::SendSystemCommand(true,
				reinterpret_cast < ViByte * >(inBuf),
				sizeof(inBuf),
				reinterpret_cast <
				ViByte * >(outBuf) /* no response buffer */ ,
				sizeof(outBuf));

	double version = outBuf[min];

	while (version >= 1)
		version /= 10;
	version += outBuf[maj];

	return version;
}

void Comm::GetDeviceInfo(ViUInt8 * outBuf, int size)
{
	ViUInt8 inBuf[] = { 0x01, 0x9B };

	// Send the system command to the NXT.
	Comm::SendSystemCommand(true,
				reinterpret_cast < ViByte * >(inBuf),
				sizeof(inBuf),
				reinterpret_cast <
				ViByte * >(outBuf) /* no response buffer */ ,
				size);
}

std::string Comm::GetName()
{
	ViUInt8 outBuf[33];
	char name[15];

	name[0] = '\0';

	GetDeviceInfo(outBuf, sizeof(outBuf));

	memcpy(name, outBuf + 3, 15 * sizeof(ViUInt8));

	name[15] = '\0';

	return std::string(name);
}
double Comm::GetAvailableFlash()
{
	ViUInt8 outBuf[33];
	ViUInt32 flash = 0;

    GetDeviceInfo(outBuf, sizeof(outBuf));

	for (int i = 0; i < 4; i++)
	{
		flash += outBuf[i + 29] << (sizeof(ViUInt8) * 8 * i);
	}

	return (double) (flash / 1024);
}

NXTFileIterator Comm::GetFileIterator(std::string pattern, bool & isFatal)
{
	ViByte inBuf[21] = { 0x86 };
	ViByte outBuf[28] = { 1 };
	NXTFileIterator iter;
	isFatal = false;

	const char *file = pattern.data();
	memcpy(inBuf + 1, file, pattern.length());

	Comm::SendSystemCommand(true,
				reinterpret_cast < ViByte * >(inBuf),
				sizeof(inBuf),
				reinterpret_cast <
				ViByte * >(outBuf), sizeof(outBuf));

	int status = outBuf[2];
	if (status != 0)
	{
		isFatal = true;
		return iter;
	}

	char *fileout = (char *) malloc(19);
	memcpy(fileout, outBuf + 4, 19);

	long size = outBuf[24] + (256 * outBuf[25]) + (outBuf[26] == 0 ? 0 : 65536 * outBuf[26])
		+ (outBuf[27] == 0 ? 0 : outBuf[27] * 16777216);
	iter = NXTFileIterator(outBuf[3], fileout, size);

	return iter;
}

NXTFile Comm::GetNXTFile(std::string filePattern, bool & isFatal)
{
	isFatal = false;
	NXTFile f(-1, const_cast < char *>(filePattern.data()), -1);
	/* NXTFileIterator nxtfIter = Comm::GetFileIterator(filePattern, isFatal);
	if (isFatal)
		return f;

	int status = nxtfIter.getFile(f);
	if (status != 0)
		isFatal = true; */

	return f;
}

void Comm::destroyFileIterator(NXTFileIterator & iter)
{
}

void Comm::destroyFile(NXTFile & file)
{
}

NXTModuleIterator Comm::GetModuleIterator(std::string pattern, int &statusCode)
{
	NXTModuleIterator iter;
	std::cout << "Not Implemented Yet" << std::endl;
	return iter;
}

void Comm::destroyModuleIterator(NXTModuleIterator & iter)
{
}

NXTModule Comm::GetModule(std::string moduleName, int moduleID, int moduleSize,
	int ioMapSize, int &statCode)
{
	NXTModule module;
	std::cout << "Not implemented yet" << std::endl;
	return module;
}

void Comm::destroyModule(NXTModule & module)
{
}
  ///START + CLEAN UP ABOVE
// A utility method for converting
// size into bits
int *getSizeBytes(long size)
{
	int b[4] = { 0 };
	int count = 0;
	while (size > 0)
	{
		if (count >= 4)
			break;
		b[count] = size % 256;
		size = (int) (size / 256);
		count++;
	}

	return b;
}

bool NXTFile::isValid()
{
	return 0;
}

int NXTFile::open(int mode, int write_size)
{
	ViByte inBuf[24] = { 0 };
	ViByte outBuf[7] = { 8 };

	memcpy(inBuf + 1, name, strlen(name));

	switch (mode) {
	case 0:
		//read
		inBuf[0] = 0x80;
		break;
	case 1:
		//write
		inBuf[0] = 0x81;
		break;
	case 2:
		//append    
		inBuf[0] = 0x8c;
		break;
	case 3:
		//data write
		inBuf[0] = 0x8B;
		break;
	case 4:
		//linear write
		inBuf[0] = 0x89;
	break;
	}

	if (mode > 0)
	{
		//write command require size.
		memcpy(inBuf + 21, getSizeBytes(write_size), 4);
	}

	Comm::SendSystemCommand(true,
				reinterpret_cast < ViByte * >(inBuf),
				sizeof(inBuf),
				reinterpret_cast <
				ViByte * >(outBuf), sizeof(outBuf));

	int status = (int) outBuf[2];
	if (status == 0)
	{
		fd = outBuf[3];		//file handle
	}

	return status;
}

int NXTFile::remove()
{
	ViByte inBuf[20] = { 0x85 };
	ViByte outBuf[23] = { 1 };

	memcpy(inBuf + 1, name, strlen(name));

	Comm::SendSystemCommand(true,
			reinterpret_cast < ViByte * >(inBuf),
			sizeof(inBuf),
			reinterpret_cast <
			ViByte * >(outBuf), sizeof(outBuf));

	int status = outBuf[2];
	//The response also contains the naem of the removed file.
	return status;
}

int NXTFile::read(char *buff, int read_size)
{
	int *read_size_bytes = getSizeBytes(read_size);
	ViByte inBuf[4] = { 0x82, fd, read_size_bytes[0], read_size_bytes[1] };
	ViByte outBuf[6 + read_size];

	Comm::SendSystemCommand(true,
				reinterpret_cast < ViByte * >(inBuf),
				sizeof(inBuf),
				reinterpret_cast <
				ViByte * >(outBuf), sizeof(outBuf));

	int status = outBuf[2];

	if (status == 0)
	{
		memcpy(buff, outBuf + 6, sizeof(outBuf) - 6);
		fd = (int) outBuf[3];
	}

	return status;
}

int NXTFile::write(char *buff, int write_size)
{
	ViByte inBuf[2 + write_size];

	inBuf[0] = 0x83;
	inBuf[1] = fd;
	ViByte outBuf[6];

	memcpy(inBuf + 2, buff, write_size);
	Comm::SendSystemCommand(true,
				reinterpret_cast < ViByte * >(inBuf),
				sizeof(inBuf),
				reinterpret_cast <
				ViByte * >(outBuf), sizeof(outBuf));

	int status = outBuf[2];

	if (status == 0)
	{
		fd = (int) outBuf[3];
	}

	return status;
}

int NXTFile::close()
{
	ViByte inBuf[2] = { 0x84, fd };
	ViByte outBuf[4] = { 1 };

	Comm::SendSystemCommand(true,
				reinterpret_cast < ViByte * >(inBuf),
				sizeof(inBuf),
				reinterpret_cast <
				ViByte * >(outBuf), sizeof(outBuf));

	int status = outBuf[2];
	return status;
}

const char *NXTFileIterator::getCurrFileName()
{
	return currFileName;
}

long NXTFileIterator::getCurrFileSize()
{
	return currFileSize;
}

int NXTFileIterator::getFile(NXTFile & file)
{
	file = NXTFile(fd, currFileName, currFileSize);
	return 0;
}

bool NXTFileIterator::isValid()
{
	return 0;
}

int NXTFileIterator::next()
{
	ViByte inBuf[2] = { 0x87, fd };
	ViByte outBuf[28] = { 1 };
	NXTFileIterator iter;

	Comm::SendSystemCommand(true,
				reinterpret_cast < ViByte * >(inBuf),
				sizeof(inBuf),
				reinterpret_cast <
				ViByte * >(outBuf), sizeof(outBuf));

	int status = (int) outBuf[2];
	char *fileout = (char *) malloc(19);
	memcpy(fileout, outBuf + 4, 19);
	currFileSize = outBuf[24] + (256 * outBuf[25]) + (outBuf[26] == 0 ? 0 : 65536 * outBuf[26])
												+ (outBuf[27] == 0 ? 0 : outBuf[27] * 16777216);
	fd = outBuf[3];
	currFileName = fileout;

	return status;
}

int NXTModuleIterator::next()
{
	return -1;
}

int NXTModuleIterator::getModule(NXTModule & module)
{
	return -1;
}

int NXTModuleIterator::getName(std::string & s)
{
	return -1;
}
int NXTModule::getName(std::string & s)
{
	return -1;
}

int NXTModule::getModuleID(int &id)
{
	return -1;
}

int NXTModule::getModuleSize(int &size)
{
	return -1;
}

int NXTModule::getIOMapSize(int &size)
{
	return -1;
}

int NXTModule::readIOMap(ViPByte buffer, int offset, int bytes)
{
	return -1;
}

int NXTModule::writeIOMap(ViPByte buffer, int offset, int bytes)
{
	 return -1;
}

