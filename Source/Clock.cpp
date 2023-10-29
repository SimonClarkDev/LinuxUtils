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

#include "../Include/Clock.h"

#include <sstream>
#include <cstring>

////////////////////////////////////////////////////////////////////////////////
///


////////////////////////////////////////////////////////////////////////////////
///

Clock::Clock () :
	m_clock (time_t {}),
	m_tm (tm {})

{
}

////////////////////////////////////////////////////////////////////////////////
///

Clock::Clock (const time_t& from) noexcept
{
	m_clock = from;
	memcpy (&m_tm, localtime (&m_clock), sizeof (m_tm));
}

////////////////////////////////////////////////////////////////////////////////
///

Clock::Clock (const Clock& from) noexcept
{
	*this = from;
}

////////////////////////////////////////////////////////////////////////////////
///

Clock& Clock::operator = (const time_t& from) noexcept
{
	m_clock = from;
	memcpy (&m_tm, localtime (&m_clock), sizeof (m_tm));
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
///

Clock& Clock::operator = (const Clock& from) noexcept
{
	m_clock = from.m_clock;
	memcpy (&m_tm, &from.m_tm, sizeof (m_tm));
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
///

time_t Clock::Now () noexcept
{
	return time (nullptr);
}

////////////////////////////////////////////////////////////////////////////////
///

void Clock::Attach (const time_t time) noexcept
{
	m_clock = time;
	memcpy (&m_tm, localtime (&m_clock), sizeof (m_tm));
}

////////////////////////////////////////////////////////////////////////////////
///

std::string Clock::GetASCIITime () const noexcept
{
	char buffer[sizeof ("00:00:00")];
	strftime (buffer, sizeof(buffer), "%T", &m_tm);
	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
///

std::string Clock::GetASCIIDate (const char separator) const noexcept
{
	char buffer[sizeof ("YYYY-MM-DD")];
	strftime (buffer, sizeof(buffer), "%Y %m %d", &m_tm);

	buffer[YearToMonthASCIIOffset] = separator;
	buffer[MonthToDayASCIIOffset] = separator;

	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
///

std::string Clock::GetASCIIYear () const noexcept
{
	char buffer[sizeof ("YYYY")];
	strftime (buffer, sizeof(buffer), "%Y", &m_tm);
	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
///

std::string Clock::GetASCIIShortMonth () const noexcept
{
	char buffer[sizeof ("Dec")];
	strftime (buffer, sizeof(buffer), "%b", &m_tm);
	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
///

std::string Clock::GetASCIICounter (uint32_t seconds)
{
    std::stringstream ss;

	ss << seconds / 60 << ":" << seconds % 60 << std::endl;
	return ss.str ();
}

////////////////////////////////////////////////////////////////////////////////
///

uint32_t Clock::GetSecondsToday () const noexcept
{
	return (static_cast<uint32_t>(GetHoursValue ()) * 60 * 60) +
		(static_cast<uint32_t>(GetMinutesValue ()) * 60) +
		static_cast<uint32_t>(GetSecondsValue ());
}

////////////////////////////////////////////////////////////////////////////////
///

uint32_t Clock::ElapsedSeconds () const noexcept
{
	return static_cast<uint32_t>(difftime (time (nullptr), m_clock));
}

////////////////////////////////////////////////////////////////////////////////
///

void Clock::SetTime (uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	m_tm.tm_hour = hours;
	m_tm.tm_min = minutes;
	m_tm.tm_sec = seconds;

	m_clock = mktime (&m_tm);
}

////////////////////////////////////////////////////////////////////////////////
///

void Clock::SetDate (uint16_t year, uint8_t month, uint8_t day)
{
	m_tm.tm_year = year;
	m_tm.tm_mon = month;
	m_tm.tm_mday = day;

	m_clock = mktime (&m_tm);
}
