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

#include "FileObject.h"

#include "spdlog/spdlog.h"

#include <cerrno>
#include <cstring>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
///

FileObject::FileObject () noexcept :
	m_handleId (INVALID_HANDLE_VALUE)
{
}

////////////////////////////////////////////////////////////////////////////////
///

FileObject::~FileObject () noexcept
{
	Close ();
}

////////////////////////////////////////////////////////////////////////////////
///

void FileObject::Close () noexcept
{
	if (m_handleId != INVALID_HANDLE_VALUE)
	{
		if (close (m_handleId) == 0)
		{
			spdlog::trace ("Closed {0} ", m_pathName);
		}
		else
		{
			spdlog::error ("Failed to close {0} with error number {1}", m_pathName, errno);
		}

		m_handleId = INVALID_HANDLE_VALUE;
		m_pathName = "";
	}
}

////////////////////////////////////////////////////////////////////////////////
///

bool FileObject::Open (const std::string& pathName, bool appendOnly) noexcept
{
	Close ();

	m_handleId = open (pathName.c_str (), ((appendOnly) ? O_APPEND|O_WRONLY : O_RDWR));
	if (m_handleId < 0)
	{
		spdlog::error ("Failed to open {0} with error number {1}", pathName, errno);
		m_handleId = INVALID_HANDLE_VALUE;
		return false;
	}

	m_pathName = pathName;

	spdlog::info ("Opened {0}", m_pathName);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
///

bool FileObject::Create (const std::string& pathName, mode_t mode) noexcept
{
	Close ();

	m_handleId = creat (pathName.c_str (), mode);
	if (m_handleId < 0)
	{
		spdlog::error ("Failed to create {0} with error number {1}", pathName, errno);
		m_handleId = INVALID_HANDLE_VALUE;
		return false;
	}

	m_pathName = pathName;

	spdlog::trace ("Created {0}", m_pathName);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
///

bool FileObject::Write (const uint8_t* pBuffer, size_t length) const noexcept
{
	if (length == 0)
	{
		spdlog::debug ("Attempting to write 0 bytes to {0}", m_pathName);
		return false;
	}

	if (!IsOpen ()) return false;

	ssize_t bytesWritten = 0;

	while (bytesWritten < static_cast<ssize_t>(length) && bytesWritten != -1)
	{
		bytesWritten += write (m_handleId, &pBuffer[bytesWritten], length - bytesWritten);
	}

	if (bytesWritten == -1)
	{
		spdlog::error ("Unable to write all data to {0} with error number {1}", m_pathName, errno);
		return false;
	}

	spdlog::trace ("Wrote {0} bytes to {1}", bytesWritten, m_pathName);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
///

size_t FileObject::Read (uint8_t* pBuffer, size_t length) noexcept
{
	if (!IsOpen ()) return 0;

	ssize_t bytesRead = read (m_handleId, (void*)pBuffer, length);
	if (bytesRead == -1)
	{
		spdlog::error ("Unable to read any data from {0} with error number {1}", m_pathName, errno);
		return 0;
	}

	if (bytesRead > 0)
	{
		spdlog::trace ("Read {0} bytes from {1}", bytesRead, m_pathName);
	}
	return bytesRead;
}

////////////////////////////////////////////////////////////////////////////////
///

bool FileObject::SeekEnd () noexcept
{
	if (!IsOpen ()) return false;

	off_t offset = lseek (m_handleId, 0, SEEK_END);
	if (offset == -1)
	{
		spdlog::error ("Unable to seek to end of {0} with error number {1}", m_pathName, errno);
		return false;
	}

	spdlog::trace ("Seek to end of {0}", m_pathName);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
///

bool ASCIIFileObject::ReadNextLine (std::string& nextLine) noexcept
{
	uint8_t nextByte = 0;

	nextLine.clear ();

	while (Read (&nextByte, 1) != 0)
	{
		if (nextByte == CarriageReturn || nextByte == LineFeed)
		{
			if (!nextLine.empty ())
			{
				return true;
			}
		}
		else
		{
			nextLine += static_cast<char>(nextByte);
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
///

bool ASCIIFileObject::WriteString (const std::string_view& writeString) noexcept
{
	return Write (reinterpret_cast<const uint8_t*>(writeString.data ()), writeString.length ());
}

////////////////////////////////////////////////////////////////////////////////
///

bool ASCIIFileObject::WriteNewLine () noexcept
{
	return Write (reinterpret_cast<const uint8_t*>(NewLineString), sizeof (NewLineString) - 1);
}
