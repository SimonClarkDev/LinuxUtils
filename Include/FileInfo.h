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

#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include <sys/stat.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////
///

class FileInfo
{
public:

	FileInfo ();
	virtual ~FileInfo () = default;

	FileInfo (const FileInfo& from) = delete;
	FileInfo (const FileInfo&& from) = delete;
	FileInfo& operator = (const FileInfo& from) = delete;
	FileInfo& operator = (const FileInfo&& from) = delete;

	FileInfo (const std::string& filePath) noexcept;
	bool Attach (const std::string& filePath) noexcept;

	time_t AccessedTime () const noexcept { return m_fileStatus.st_atime; }
	time_t CreatedTime () const noexcept { return m_fileStatus.st_ctime; }
	time_t ModifiedTime () const noexcept { return m_fileStatus.st_mtime; }
	off_t Size () const noexcept { return m_fileStatus.st_size; }

private:

	bool Refresh ();

	struct stat m_fileStatus;
	std::string m_filePath;
};

#endif
