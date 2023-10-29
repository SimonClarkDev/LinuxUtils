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

#include "../Include/CSVFile.h"

/////////////////////////////////////////////////////////////////////////////////
///

bool CSVFile::Open (const std::string& filePath, bool append) noexcept
{
	if (append)
	{
		if (m_fileObject.Open (filePath, true)) return true;
	}

	return m_fileObject.Create (filePath);
}

/////////////////////////////////////////////////////////////////////////////////
///

void CSVFile::Close (bool flush) noexcept
{
	if (m_columnCollection.Length () > 0 && flush)
	{
		Write ();
	}

	m_fileObject.Close ();
}

/////////////////////////////////////////////////////////////////////////////////
///

void CSVFile::WriteHeader () noexcept
{
	m_columnCollection.SetFixedColumns ();

	for (uint32_t index = 0; index < m_columnCollection.Length (); ++index)
	{
		if (index != 0) m_fileObject.Write (reinterpret_cast<const uint8_t*>(commaSep), sizeof (commaSep) - 1);

		std::string formattedText = m_columnCollection[index].GetName ();
		m_fileObject.Write ((const uint8_t*)formattedText.c_str (), formattedText.length ());
	}

	m_fileObject.Write ((uint8_t*)newLine, sizeof (newLine) - 1);
}

/////////////////////////////////////////////////////////////////////////////////
///

void CSVFile::Write () noexcept
{
	m_columnCollection.SetFixedColumns ();

	for (uint32_t index = 0; index < m_columnCollection.Length (); ++index)
	{
		if (index != 0) m_fileObject.Write (reinterpret_cast<const uint8_t*>(commaSep), sizeof (commaSep) - 1);

		std::string formattedText = m_columnCollection[index].GetFormattedText ();
		m_fileObject.Write ((const uint8_t*)formattedText.c_str (), formattedText.length ());
	}

	m_fileObject.Write ((uint8_t*)newLine, sizeof (newLine) - 1);

	ClearLine ();
}
