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
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	/// Lightweight real time clock wrapper
	///

	class Clock
	{
	public:

		Clock ();
		virtual ~Clock () = default;
		Clock (const time_t& from) noexcept;
		Clock (const Clock& from) noexcept;
		Clock (const Clock&& from) = delete;

		Clock& operator = (const time_t& from) noexcept;
		Clock& operator = (const Clock& from) noexcept;
		operator time_t () const { return m_clock; }

		[[nodiscard]] bool TimeEquals (uint8_t hours, uint8_t minutes, uint8_t seconds) const noexcept
		{
			return m_tm.tm_hour == hours && m_tm.tm_min == minutes && m_tm.tm_sec == seconds;
		}

		[[nodiscard]] bool operator > (const Clock& from) const noexcept
		{
			return m_clock > from.m_clock;
		}

		[[nodiscard]] bool operator >= (const Clock& from) const noexcept
		{
			return m_clock >= from.m_clock;
		}

		[[nodiscard]] bool operator < (const Clock& from) const noexcept
		{
			return m_clock < from.m_clock;
		}

		[[nodiscard]] bool operator <= (const Clock& from) const noexcept
		{
			return m_clock <= from.m_clock;
		}

		void Attach (const time_t time) noexcept;
		void SetTime (uint8_t hours, uint8_t minutes, uint8_t seconds);
		void SetDate (uint16_t year, uint8_t month, uint8_t day);

		[[nodiscard]] static time_t Now () noexcept;
		[[nodiscard]] std::string GetASCIITime () const noexcept;
		[[nodiscard]] std::string GetASCIIDate (const char separator = '/') const noexcept;
		[[nodiscard]] std::string GetASCIIYear () const noexcept;
		[[nodiscard]] std::string GetASCIIShortMonth () const noexcept;
		[[nodiscard]] uint8_t GetDayOfWeek () const noexcept;
		[[nodiscard]] uint32_t ElapsedSeconds () const noexcept;
		[[nodiscard]] static std::string GetASCIICounter (uint32_t seconds);
		[[nodiscard]] uint32_t GetSecondsToday () const noexcept;
		[[nodiscard]] uint8_t GetSecondsValue () const noexcept { return static_cast<uint8_t>(m_tm.tm_sec); }
		[[nodiscard]] uint8_t GetMinutesValue () const noexcept { return static_cast<uint8_t>(m_tm.tm_min); }
		[[nodiscard]] uint8_t GetHoursValue () const noexcept { return static_cast<uint8_t>(m_tm.tm_hour); }
		[[nodiscard]] uint8_t GetMonthsValue () const noexcept { return static_cast<uint8_t>(m_tm.tm_mon); }

	private:

		static constexpr int YearToMonthASCIIOffset = 4;
		static constexpr int MonthToDayASCIIOffset = 7;

		time_t m_clock;
		tm m_tm;
	};
}

#endif
