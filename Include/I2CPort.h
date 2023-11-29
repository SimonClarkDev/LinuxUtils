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

#ifndef _I2C_PORT_H_
#define _I2C_PORT_H_

#include <string>

#include "FileObject.h"

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// I2C port wrapper class
	/// </summary>

	class I2CPort : public FileObject
	{
	public:

		I2CPort () = default;
		virtual ~I2CPort () = default;

		I2CPort (const I2CPort& from) = default;
		I2CPort (I2CPort&& from) = default;
		I2CPort& operator = (const I2CPort& from) = default;
		I2CPort& operator = (I2CPort&& from) = default;

		[[nodiscard]] bool Open (const std::string& pathName, uint8_t slaveAddress) noexcept;
		[[nodiscard]] bool ReadByte (uint8_t baseAddress, uint8_t& readData) noexcept;
		[[nodiscard]] bool ReadWord (uint8_t baseAddress, uint16_t& readData) noexcept;
		[[nodiscard]] bool WriteByte (uint8_t baseAddress, uint8_t writeData) noexcept;
		[[nodiscard]] bool WriteWord (uint8_t baseAddress, uint16_t writeData) noexcept;
	};
}

#endif
