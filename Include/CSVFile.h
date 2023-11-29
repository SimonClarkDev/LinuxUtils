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

#ifndef _CSV_FILE_H_
#define _CSV_FILE_H_

#include "CSVCore.h"
#include "FileObject.h"

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	class CSVWriter : public CSVCore
	{
	public:

		CSVWriter (CSVColumnCollection& columnCollection) : CSVCore (columnCollection)
		{
		}
		virtual ~CSVWriter () = default;

		CSVWriter (const CSVWriter& from) = delete;
		CSVWriter (const CSVWriter&& from) = delete;
		CSVWriter& operator = (const CSVWriter& from);
		CSVWriter& operator = (const CSVWriter&& from) = delete;

		[[nodiscard]] bool Open (const std::string& filePath, bool append = true) noexcept;
		void Close (bool flush = false) noexcept;

		[[nodiscard]] bool WriteHeader () noexcept;
		[[nodiscard]] bool WriteLine () noexcept;

	private:

		static constexpr const std::string_view CommaSeparator = ", ";

		ASCIIFileObject	m_fileObject;
	};

	////////////////////////////////////////////////////////////////////////////
	///

	class CSVReader : public CSVCore
	{
	public:

		CSVReader (CSVColumnCollection& columnCollection) : CSVCore (columnCollection)
		{
		}
		virtual ~CSVReader () = default;

		CSVReader (const CSVReader& from) = delete;
		CSVReader (const CSVReader&& from) = delete;
		CSVReader& operator = (const CSVReader& from);
		CSVReader& operator = (const CSVReader&& from) = delete;

		[[nodiscard]] bool Open (const std::string& filePath) noexcept;
		void Close () noexcept;

		[[nodiscard]] bool ReadHeader () noexcept;
		[[nodiscard]] bool ReadLine () noexcept;

	private:

		static constexpr const char Comma = ',';

		ASCIIFileObject	m_fileObject;
	};
}

#endif
