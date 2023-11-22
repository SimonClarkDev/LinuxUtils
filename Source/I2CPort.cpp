////////////////////////////////////////////////////////////////////////////////
/// MIT License
///
/// Copyright 2020-2023 Simon Clark
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the “Software”), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.

#include "I2CPort.h"

#include "spdlog/spdlog.h"

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

////////////////////////////////////////////////////////////////////////////////
///

bool I2CPort::Open (const std::string& pathName, uint8_t slaveAddress) noexcept
{
	if (!FileObject::Open (pathName)) return false;

	if (ioctl (m_handleId, I2C_SLAVE, slaveAddress) != 0)
	{
		spdlog::error ("Failed to setup I2C slave address {0} with error number {1}", slaveAddress, errno);

		Close ();
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
///

bool I2CPort::ReadByte (uint8_t baseAddress, uint8_t& readData) noexcept
{
	if (Write (&baseAddress, sizeof (uint8_t)))
	{
		return Read (&readData, sizeof (uint8_t)) == sizeof (uint8_t);
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
///

bool I2CPort::ReadWord (uint8_t baseAddress, uint16_t& readData) noexcept
{
	if (Write (&baseAddress, sizeof (uint8_t)))
	{
		uint8_t dataBuffer[sizeof (uint16_t)];
		if (Read (dataBuffer, sizeof (uint16_t)) == sizeof (uint16_t))
		{
			readData = static_cast<uint16_t>(dataBuffer[0] | (dataBuffer[1] << 8));
			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
///

bool I2CPort::WriteByte (uint8_t baseAddress, uint8_t writeData) noexcept
{
	uint8_t outputBuffer[] = {baseAddress, writeData};
	return Write (outputBuffer, sizeof (outputBuffer));
}

////////////////////////////////////////////////////////////////////////////////
///

bool I2CPort::WriteWord (uint8_t baseAddress, uint16_t writeData) noexcept
{
	uint8_t outputBuffer[] = {baseAddress, static_cast<uint8_t>(writeData >> 8), static_cast<uint8_t>(writeData)};
	return Write (outputBuffer, sizeof (outputBuffer));
}
