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

#ifndef _FILE_OBJECT_H_
#define _FILE_OBJECT_H_

#include <fcntl.h>
#include <string>

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Core file based object class
/// </summary>

class FileObject
{
public:

	FileObject () noexcept;
	virtual ~FileObject () noexcept;

	FileObject (const FileObject& from)
	{
		operator = (from);
	}
	FileObject (FileObject&& from)
	{
		operator = (from);
	}
	FileObject& operator = (const FileObject& from)
	{
		m_handleId = from.m_handleId;
		m_pathName = from.m_pathName;
		return *this;
	}
	FileObject& operator = (FileObject&& from)
	{
		m_handleId = from.m_handleId;
		m_pathName = from.m_pathName;
		from.m_handleId = INVALID_HANDLE_VALUE;
		from.m_pathName = "";
		return *this;
	}

	bool IsOpen () const noexcept {return m_handleId != INVALID_HANDLE_VALUE;}
	bool Create (const std::string& pathName, mode_t mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) noexcept;
	bool Open (const std::string& pathName, bool appendOnly = false) noexcept;
	bool Write (const uint8_t* pBuffer, size_t length) const noexcept;
	size_t Read (uint8_t* pBuffer, size_t length) noexcept;
	bool SeekEnd () noexcept;
	void Close () noexcept;

protected:

	static constexpr int32_t	INVALID_HANDLE_VALUE = -1;

	std::string		m_pathName;
	int32_t			m_handleId;
};

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// ASCII file based object class
/// </summary>

class ASCIIFileObject : public FileObject
{
public:

	bool WriteString (const std::string_view& writeString) noexcept;
	bool WriteNewLine () noexcept;
	bool ReadNextLine (std::string& nextLine) noexcept;

private:

	static constexpr char CarriageReturn = '\r';
	static constexpr char LineFeed = '\n';
	static constexpr const char* NewLineString = "\r\n";
};

#endif

