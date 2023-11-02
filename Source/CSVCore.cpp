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

#include "CSVCore.h"

#include <iostream>

/////////////////////////////////////////////////////////////////////////////////
///

CSVColumn::CSVColumn (Type type, std::string columnName, uint8_t width, uint8_t precision, bool leftJustified)
{
	char format[TEMPORARY_BUFFER_SIZE];

	char* pDst = format;

	if (leftJustified) *pDst++ = '-';

	// Depending on type...
	switch (type)
	{
	case	Type::Date: break;
	case	Type::Time: break;

	case	Type::String:
		sprintf (pDst, "%%.%ds", width);
		break;

	case	Type::Row:
	case	Type::Int:
		sprintf (pDst, "%%%d.%dd", width, precision);
		break;

	case	Type::Float:
		sprintf (pDst, "%%%d.%df", width, precision);
		break;
	}

	m_formatText = format;
	m_columnName = columnName;
	m_precision = precision;
	m_width = width;
	m_type = type;
}

/////////////////////////////////////////////////////////////////////////////////
///

void CSVColumn::Format (const std::string& text)
{
	char formattedText[128];

	sprintf (formattedText, m_formatText.c_str (), text.c_str ());
	m_formattedText = formattedText;
}

/////////////////////////////////////////////////////////////////////////////////
///

void CSVColumn::Format (int32_t value)
{
	if (value == 0)
	{
		m_formattedText = "0";
		return;
	}

	char formattedText[TEMPORARY_BUFFER_SIZE];
	sprintf (formattedText, m_formatText.c_str (), value);
	m_formattedText = formattedText;
}

/////////////////////////////////////////////////////////////////////////////////
///

void CSVColumn::Format (float value)
{
	char formattedText[TEMPORARY_BUFFER_SIZE];
	sprintf (formattedText, m_formatText.c_str (), value);
	m_formattedText = formattedText;
}

/////////////////////////////////////////////////////////////////////////////////
///

double CSVColumn::GetValueAsDouble () const noexcept
{
	return std::stod (m_formattedText);
}

/////////////////////////////////////////////////////////////////////////////////
///

float CSVColumn::GetValueAsFloat () const noexcept
{
	return std::stof (m_formattedText);
}

/////////////////////////////////////////////////////////////////////////////////
///

int32_t CSVColumn::GetValueAsInt () const noexcept
{
	return std::stoi (m_formattedText);
}

/////////////////////////////////////////////////////////////////////////////////
///

void CSVColumn::FormatFixed (uint32_t rowNumber)
{
	char formattedText[TEMPORARY_BUFFER_SIZE];
	Clock clock;

	switch (m_type)
	{
	case	CSVColumn::Type::Row:
		sprintf (formattedText, m_formatText.c_str (), rowNumber);
		m_formattedText = formattedText;
		break;

	case	CSVColumn::Type::Date:
		clock = Clock::Now ();
		m_formattedText = clock.GetASCIIDate ();
		break;

	case	CSVColumn::Type::Time:
		clock = Clock::Now ();
		m_formattedText = clock.GetASCIITime ();
		break;

	default:
		// Nothing to do
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////
///

CSVColumn& CSVColumn::operator = (const CSVColumn& from)
{
	m_formattedText = from.m_formattedText;
	m_columnName = from.m_columnName;
	m_formatText = from.m_formatText;
	m_precision = from.m_precision;
	m_width = from.m_width;
	m_type = from.m_type;
	return *this;
}
