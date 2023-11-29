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

#include "FileFind.h"

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	///

	FileFind::FileFind () :
		m_pDirectory (nullptr),
		m_type (FindType::Unknown)
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	///

	FileFind::~FileFind ()
	{
		Close ();
	}

	////////////////////////////////////////////////////////////////////////////////
	///

	bool FileFind::Open (const std::string& path) noexcept
	{
		Close ();

		m_pDirectory = opendir (path.c_str ());

		if (m_pDirectory != nullptr)
		{
			m_path = path;
			return true;
		}

		return false;
	}

	////////////////////////////////////////////////////////////////////////////////
	///

	void FileFind::Close () noexcept
	{
		if (m_pDirectory == nullptr) return;
		closedir (m_pDirectory);
		m_pDirectory = nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////
	///

	bool FileFind::Next (bool includeHierarchy) noexcept
	{
		if (!IsOpen ()) return false;

		do
		{
			const struct dirent* pEntry = readdir (m_pDirectory);
			if (pEntry == nullptr) return false;

			m_name = pEntry->d_name;

			switch (pEntry->d_type)
			{
			case	DT_DIR:	m_type = FindType::Directory; break;
			case	DT_LNK:	m_type = FindType::SymbolicLink; break;
			case	DT_REG:	m_type = FindType::RegularFile; break;

			default:
				m_type = FindType::Unknown;
				break;
			}
		}
		while (((m_name == "." || m_name == "..") && !includeHierarchy) || m_type == FindType::Unknown);
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////
	///

	std::string FileFind::Path () const noexcept
	{
		if (!IsOpen ()) return "";
		return m_path + "/" + m_name;
	}
}
