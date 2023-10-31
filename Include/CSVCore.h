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

#ifndef _CSV_CORE_H_
#define _CSV_CORE_H_

#include <string>
#include <vector>

#include "Clock.h"

////////////////////////////////////////////////////////////////////////////////
//

class CSVColumn
{
public:

	enum class Type {Date, Time, Row, String, Int, Float};

	CSVColumn (Type type, std::string columnName = "",
		uint8_t width = 0, uint8_t precision = 0, bool leftJustified = false);
	CSVColumn (const CSVColumn& from) { operator = (from); }

	virtual ~CSVColumn () = default;

	CSVColumn (const CSVColumn&& from) = delete;
	CSVColumn& operator = (const CSVColumn& from);
	CSVColumn& operator = (const CSVColumn&& from) = delete;

	void Format (const std::string& text);
	void Format (int32_t value);
	void Format (float value);
	void FormatFixed (uint32_t rowNumber);

	const std::string& GetFormattedText () const noexcept {return m_formattedText;}
	const std::string& GetName () const noexcept {return m_columnName;}
	uint8_t GetPrecision () const noexcept {return m_precision;}
	uint8_t GetWidth () const noexcept {return m_width;}
	Type GetType () const noexcept {return m_type;}

	void SetName (const std::string& columnName) noexcept { m_columnName = columnName; }
	void SetFormattedText (const std::string& formattedText) noexcept { m_formattedText = formattedText; }
	void Clear () noexcept { m_formattedText.clear (); }

private:

	static constexpr uint32_t TEMPORARY_BUFFER_SIZE = 32;

	std::string	m_columnName;
	std::string m_formatText;
	std::string m_formattedText;
	uint8_t		m_precision;
	uint8_t		m_width;
	Type		m_type;
};

////////////////////////////////////////////////////////////////////////////////
//

class CSVColumnCollection
{
public:

	CSVColumnCollection () : m_rowNumber (0) {}

	virtual ~CSVColumnCollection () = default;

	CSVColumnCollection (const CSVColumnCollection& from) = delete;
	CSVColumnCollection (const CSVColumnCollection&& from) = delete;
	CSVColumnCollection& operator = (const CSVColumnCollection&& from) = delete;

	void AddColumn (const CSVColumn& column)
	{
		m_vecColumns.push_back (column);
	}

	CSVColumn& GetAt (uint8_t index)
	{
		return m_vecColumns[index];
	}

	uint32_t Columns ()
	{
		return m_vecColumns.size ();
	}

	void ClearAll ()
	{
		for (CSVColumn& item : m_vecColumns) item.Clear ();
	}

	uint32_t Length (void) { return m_vecColumns.size (); }

	CSVColumn& operator [] (const uint32_t index)
	{
		return m_vecColumns[index];
	}

	CSVColumnCollection& operator = (const CSVColumnCollection& from)
	{
		m_vecColumns.clear ();
		for (const CSVColumn& item : from.m_vecColumns) m_vecColumns.push_back(item);
		m_rowNumber = from.m_rowNumber;
		return *this;
	}

	void SetFixedColumns ()
	{
		++m_rowNumber;
		for (CSVColumn& item : m_vecColumns)
			item.FormatFixed (m_rowNumber);
	}

private:

	std::vector<CSVColumn>	m_vecColumns;
	uint32_t				m_rowNumber;
};

////////////////////////////////////////////////////////////////////////////////
//

class CSVCore
{
public:

	CSVCore (CSVColumnCollection& columnCollection) :
		m_columnCollection (columnCollection)
	{
		ClearLine ();
	}

	virtual ~CSVCore () = default;

	CSVCore (const CSVCore& from) = delete;
	CSVCore (const CSVCore&& from) = delete;
	CSVCore& operator = (const CSVCore& from) = delete;
	CSVCore& operator = (const CSVCore&& from) = delete;

	void SetStringAt (uint8_t index, const std::string& text)
	{
		if (index >= m_columnCollection.Columns ()) return;
		m_columnCollection.GetAt (index).Format (text);
	}

	void SetIntegerAt (uint8_t index, int32_t value)
	{
		if (index >= m_columnCollection.Columns ()) return;
		m_columnCollection.GetAt (index).Format (value);
	}

	void SetFloatAt (uint8_t index, float value)
	{
		if (index >= m_columnCollection.Columns ()) return;
		m_columnCollection.GetAt (index).Format (value);
	}

	void SetFormattedAt (uint8_t index, const std::string& text)
	{
		if (index >= m_columnCollection.Columns ()) return;
		m_columnCollection.GetAt (index).SetFormattedText (text);
	}

protected:

	void ClearLine ()
	{
		m_columnCollection.ClearAll ();
	}

	CSVColumnCollection& m_columnCollection;
};

#endif
