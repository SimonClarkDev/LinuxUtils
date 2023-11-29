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

#include <CSVFile.h>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	///

	bool CSVWriter::Open (const std::string& filePath, bool append) noexcept
	{
		if (append)
		{
			if (m_fileObject.Open (filePath, true)) return true;
		}

		return m_fileObject.Create (filePath);
	}

	////////////////////////////////////////////////////////////////////////////
	///

	void CSVWriter::Close (bool flush) noexcept
	{
		if (m_columnCollection.Length () > 0 && flush)
		{
			(void)WriteLine ();
		}

		m_fileObject.Close ();
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool CSVWriter::WriteHeader () noexcept
	{
		m_columnCollection.SetFixedColumns ();

		for (uint32_t index = 0; index < m_columnCollection.Length (); ++index)
		{
			if (index != 0)
			{
				if (!m_fileObject.WriteString (CommaSeparator)) return false;
			}
			if (!m_fileObject.WriteString (m_columnCollection[index].GetName ())) return false;
		}

		return m_fileObject.WriteNewLine ();
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool CSVWriter::WriteLine () noexcept
	{
		bool writeValid = true;
		m_columnCollection.SetFixedColumns ();

		for (uint32_t index = 0; index < m_columnCollection.Length () && writeValid; ++index)
		{
			if (index != 0)
			{
				if (!m_fileObject.WriteString (CommaSeparator)) writeValid = false;
			}
			if (!m_fileObject.WriteString (m_columnCollection[index].GetValueAsText ())) writeValid = false;
		}

		if (writeValid)
		{
			ClearLine ();
			if (!m_fileObject.WriteNewLine ()) return false;
		}

		return writeValid;
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool CSVReader::Open (const std::string& filePath) noexcept
	{
		return m_fileObject.Open (filePath);
	}

	////////////////////////////////////////////////////////////////////////////
	///

	void CSVReader::Close () noexcept
	{
		m_fileObject.Close ();
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool CSVReader::ReadHeader () noexcept
	{
		if (ReadLine ())
		{
			for (uint32_t index = 0; index < m_columnCollection.Length (); ++index)
			{
				m_columnCollection.GetAt (index).SetName (m_columnCollection.GetAt (index).GetValueAsText ());
			}

			return true;
		}

		return false;
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool CSVReader::ReadLine () noexcept
	{
		std::string nextLine;
		bool readValid = m_fileObject.ReadNextLine (nextLine);
		nextLine.erase (std::remove_if (nextLine.begin (), nextLine.end (), isspace), nextLine.end ());

		m_columnCollection.ClearAll ();
		uint32_t lastOffset = 0;

		for (uint32_t index = 0; index < m_columnCollection.Length () && lastOffset < nextLine.length (); ++index)
		{
			size_t nextOffset = nextLine.find (Comma, lastOffset);

			if (nextOffset == std::string::npos)
			{
				nextOffset = nextLine.length ();
			}

			SetFormattedAt (index, nextLine.substr (lastOffset, nextOffset - lastOffset));
			lastOffset = nextOffset + 1;
		}

		return readValid;
	}
}
