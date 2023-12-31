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

#ifndef LINUX_FILEFIND_H_
#define LINUX_FILEFIND_H_

#include <string>
#include <dirent.h>

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	///

	class FileFind
	{
	public:

		FileFind ();
		virtual ~FileFind ();

		FileFind (const FileFind& from) = delete;
		FileFind (FileFind&& from) = delete;
		FileFind& operator = (const FileFind& from) = delete;
		FileFind& operator = (FileFind&& from) = delete;

		enum class FindType {Unknown, Directory, SymbolicLink, RegularFile};

		[[nodiscard]] bool IsOpen () const noexcept { return m_pDirectory != nullptr; }
		[[nodiscard]] FindType Type () const noexcept { return m_type; }
		[[nodiscard]] std::string Name () const noexcept { return m_name; }
		[[nodiscard]] bool Open (const std::string& path = ".") noexcept;
		[[nodiscard]] bool Next (bool includeHierarchy = false) noexcept;
		[[nodiscard]] std::string Path () const noexcept;

		void Close () noexcept;

	private:

		DIR*		m_pDirectory;
		FindType	m_type;
		std::string	m_name;
		std::string m_path;
	};
}

#endif
