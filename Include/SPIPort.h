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

#ifndef _SPI_PORT_H_
#define _SPI_PORT_H_

#include <vector>

#include "FileObject.h"

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// I2C port wrapper class
/// </summary>

class SPIPort : public FileObject
{
public:

	SPIPort ();
	virtual ~SPIPort () = default;

	SPIPort (const SPIPort& from) = default;
	SPIPort (SPIPort&& from) = default;
	SPIPort& operator = (const SPIPort& from) = default;
	SPIPort& operator = (SPIPort&& from) = default;

	bool Open (const std::string& pathName, uint32_t speed = 1000000) noexcept;
	bool Write (const uint8_t* pBuffer, size_t length) noexcept;
	size_t Read (uint8_t* pBuffer, size_t length) noexcept;

private:

	static constexpr uint32_t MAX_BUFFER_SIZE = 16;

	uint8_t m_recvBuffer[MAX_BUFFER_SIZE];
	uint32_t m_sentBytes;
};

#endif
