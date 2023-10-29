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

#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <ctime>
#include <string>

////////////////////////////////////////////////////////////////////////////////
/// Lightweight real time clock wrapper
///

class Clock
{
public:

	////////////////////////////////////////////////////////////////////////////
	/// @brief Clock construction
	///

	Clock ();
	virtual ~Clock () = default;
	Clock (const time_t& from) noexcept;
	Clock (const Clock& from) noexcept;
	Clock (const Clock&& from) = delete;

	////////////////////////////////////////////////////////////////////////////
	/// @brief Clock overloaded operators
	///

	Clock& operator = (const time_t& from) noexcept;
	Clock& operator = (const Clock& from) noexcept;
	operator time_t () const { return m_clock; }

	bool TimeEquals (uint8_t hours, uint8_t minutes, uint8_t seconds) const noexcept
	{
		return m_tm.tm_hour == hours && m_tm.tm_min == minutes && m_tm.tm_sec == seconds;
	}

	////////////////////////////////////////////////////////////////////////////
	/// @brief Determine if right clock is greater/later/after left
	///
	bool operator > (const Clock& from) const noexcept
	{
		return m_clock > from.m_clock;
	}

	////////////////////////////////////////////////////////////////////////////
	/// @brief Determine if right clock is greater/later/after or equal to left
	///
	bool operator >= (const Clock& from) const noexcept
	{
		return m_clock >= from.m_clock;
	}

	////////////////////////////////////////////////////////////////////////////
	/// @brief Determine if right clock is less/earlier/before left
	///
	bool operator < (const Clock& from) const noexcept
	{
		return m_clock < from.m_clock;
	}

	////////////////////////////////////////////////////////////////////////////
	/// @brief Determine if right clock is less/earlier/before or equal to left
	///
	bool operator <= (const Clock& from) const noexcept
	{
		return m_clock <= from.m_clock;
	}

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get the time right now
	///
	static time_t Now () noexcept;

	////////////////////////////////////////////////////////////////////////////
	// @brief Attach the supplied time to the current object
	///
	void Attach (const time_t time) noexcept;

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get current time as an ASCII string HH:MM:SS
	///
	std::string GetASCIITime () const noexcept;

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get current date as an ASCII string YYYY-MM-DD
	///
	std::string GetASCIIDate (const char separator = '/') const noexcept;

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get current year as an ASCII string YYYY
	///
	std::string GetASCIIYear () const noexcept;

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get current short month as an ASCII string 'Jan', 'Feb' etc...
	///
	std::string GetASCIIShortMonth () const noexcept;

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get number of seconds elapsed since time was last refreshed
	///
	uint32_t ElapsedSeconds () const noexcept;

	////////////////////////////////////////////////////////////////////////////
	/// @brief Set time overriding current stored time (date unchanged)
	///
	void SetTime (uint8_t hours, uint8_t minutes, uint8_t seconds);

	////////////////////////////////////////////////////////////////////////////
	/// Set date overriding current stored date (time unchanged)
	///
	void SetDate (uint16_t year, uint8_t month, uint8_t day);

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get the clock time from the supplied number of seconds
	///
	static std::string GetASCIICounter (uint32_t seconds);

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get the total number of seconds elapsed today
	///
	uint32_t GetSecondsToday () const noexcept;

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get the number of seconds in the current minute
	///
	uint8_t GetSecondsValue () const noexcept { return static_cast<uint8_t>(m_tm.tm_sec); }

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get the number of minutes in the current hour
	///
	uint8_t GetMinutesValue () const noexcept { return static_cast<uint8_t>(m_tm.tm_min); }

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get the current hour (from 0-23)
	///
	uint8_t GetHoursValue () const noexcept { return static_cast<uint8_t>(m_tm.tm_hour); }

	////////////////////////////////////////////////////////////////////////////
	/// @brief Get the current month (from 0-11)
	///
	uint8_t GetMonthsValue () const noexcept { return static_cast<uint8_t>(m_tm.tm_mon); }

private:

	static constexpr int YearToMonthASCIIOffset = 4;
	static constexpr int MonthToDayASCIIOffset = 7;

	////////////////////////////////////////////////////////////////////////////s
	/// @brief The raw clock associated with this clock instance
	///
	time_t m_clock;

	////////////////////////////////////////////////////////////////////////////s
	/// @brief The local tm structure associated with this clock instance
	///
	tm m_tm;
};

#endif
