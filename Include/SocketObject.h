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

#ifndef _SOCKET_OBJECT_H_
#define _SOCKET_OBJECT_H_

#include <fcntl.h>
#include <string>

#include "../Include/ISerialise.h"

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Socket object class
/// </summary>

class SocketObject : public ISerialise
{
public:

	SocketObject (const SocketObject& from) = delete;
	SocketObject (SocketObject&& from) = delete;
	SocketObject& operator = (SocketObject&& from) = delete;
	SocketObject& operator = (const SocketObject& from) = delete;

	SocketObject () noexcept;
	virtual ~SocketObject () noexcept;

	void Open (const std::string& address, uint16_t port) noexcept;

	virtual void ReOpen () noexcept;
	virtual void Close () noexcept;
	virtual bool Send (const uint8_t* pBuffer, size_t length) noexcept;
	virtual size_t Read (uint8_t* pBuffer, size_t length) noexcept;

protected:

	std::string		m_address;
	uint16_t		m_port;
};

#endif

