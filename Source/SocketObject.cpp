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

#include "SocketObject.h"

#include "spdlog/spdlog.h"

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cerrno>
#include <cstring>

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	///

	SocketObject::SocketObject () noexcept :
		m_address (""),
		m_port (0)
	{
	}

	////////////////////////////////////////////////////////////////////////////
	///

	SocketObject::~SocketObject () noexcept
	{
		Close ();
	}

	////////////////////////////////////////////////////////////////////////////
	///

	void SocketObject::Close () noexcept
	{
		if (m_handleId != INVALID_HANDLE_VALUE)
		{
			if (close (m_handleId) == 0)
			{
				spdlog::trace ("Closed {0} ", m_address);
			}
			else
			{
				spdlog::error ("Failed to close {0} with error number {1}", m_address, errno);
			}

			m_handleId = INVALID_HANDLE_VALUE;
		}
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool SocketObject::Open (const std::string& address, uint16_t port) noexcept
	{
		Close ();

		m_handleId = socket (AF_INET, SOCK_STREAM, 0);
		m_address = address;
		m_port = port;

		if (m_handleId == INVALID_HANDLE_VALUE)
		{
			spdlog::error ("Failed to open {0} on port {1} with error number {2}", address, port, errno);
			return false;
		}

		struct sockaddr_in bindingDetails;

		bindingDetails.sin_addr.s_addr = inet_addr (address.c_str ());
		bindingDetails.sin_port = htons (port);
		bindingDetails.sin_family = AF_INET;

		if (connect (m_handleId, reinterpret_cast<sockaddr*>(&bindingDetails), sizeof (bindingDetails)) != 0)
		{
			spdlog::error ("Failed to connect remote with error number {0}", errno);
			Close ();
			return false;
		}

		spdlog::info ("Opened socket stream connection to {0} on port {1}", m_address, m_port);
		return true;
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool SocketObject::ReOpen () noexcept
	{
		if (m_address == "" || m_port == 0) return false;
		return Open (m_address, m_port);
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool SocketObject::Send (const uint8_t* pBuffer, size_t length) noexcept
	{
		if (length == 0)
		{
			spdlog::debug ("Attempting to write 0 bytes to {0}", m_address);
			return false;
		}

		if (!IsOpen ()) return false;

		ssize_t bytesWritten = 0;

		while (bytesWritten < static_cast<ssize_t>(length) && bytesWritten != -1)
		{
			bytesWritten += send (m_handleId, &pBuffer[bytesWritten], length - bytesWritten, MSG_NOSIGNAL);
		}

		if (bytesWritten == -1)
		{
			spdlog::error ("Unable to write all data to {0} with error number {1}", m_address, errno);
			return false;
		}

		spdlog::trace ("Wrote {0} bytes to {1}", bytesWritten, m_address);
		return true;
	}

	////////////////////////////////////////////////////////////////////////////
	///

	size_t SocketObject::Read (uint8_t* pBuffer, size_t length) noexcept
	{
		if (!IsOpen ()) return 0;

		size_t available = 0;
		if (ioctl (m_handleId, FIONREAD, &available) < 0)
		{
			spdlog::error ("Unable to determine length of read data from {0} with error number {1}", m_address, errno);
			return 0;
		}

		length = std::min (available, length);
		if (length == 0) return 0;

		ssize_t bytesRead = recv (m_handleId, (void*)pBuffer, length, 0);
		if (bytesRead == -1)
		{
			spdlog::error ("Unable to read any data from {0} with error number {1}", m_address, errno);
			return 0;
		}

		if (bytesRead > 0)
		{
			spdlog::trace ("Read {0} bytes from {1}", bytesRead, m_address);
		}
		return bytesRead;
	}
}
