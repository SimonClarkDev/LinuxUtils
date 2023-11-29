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

#ifndef _DATA_BLOCK_H_
#define _DATA_BLOCK_H_

#include <vector>
#include <string>
#include <cstdint>
#include <cstring>

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	struct DataBlock
	{
		DataBlock () = default;
		virtual ~DataBlock () = default;

		DataBlock (const DataBlock& from) noexcept { operator = (from); }
		DataBlock (DataBlock&& from) noexcept { operator = (from); }

		DataBlock& operator = (const DataBlock& from) noexcept
		{
			m_data.resize (from.GetSize ());
			std::memmove (m_data.data (), from.m_data.data (), from.GetSize ());
			return *this;
		}

		DataBlock& operator = (DataBlock&& from) noexcept
		{
			m_data.swap (from.m_data);
			return *this;
		}

		[[nodiscard]] size_t GetSize () const noexcept { return m_data.size (); }
		[[nodiscard]] const uint8_t* GetData (uint32_t offset = 0) const noexcept { return &m_data.data ()[offset]; }

		[[nodiscard]] uint8_t operator [] (int32_t offset) { return m_data.data ()[offset]; }

		void SetData (const uint8_t* dataPointer, size_t lengthOfData) noexcept
		{
			m_data.clear ();
			AddData (dataPointer, lengthOfData);
		}

		void Clear () noexcept { m_data.clear (); }

		[[nodiscard]] uint8_t* ExpandBy (uint32_t numberOfBytes) noexcept
		{
			size_t currentSize = m_data.size ();
			m_data.resize (currentSize + numberOfBytes);
			return m_data.data () + currentSize;
		}

		void AddData (const uint8_t* dataPointer, size_t lengthOfData) noexcept
		{
			size_t currentSize = m_data.size ();
			m_data.resize (currentSize + lengthOfData);
			std::memmove (m_data.data () + currentSize, dataPointer, lengthOfData);
		}

		void AddString (const std::string& value) noexcept
		{
			AddData (reinterpret_cast<const uint8_t*>(value.c_str ()), value.length ());
		}

		void AddByte (uint8_t value) noexcept
		{
			AddData (&value, sizeof(uint8_t));
		}

		void AddWord (uint16_t value) noexcept
		{
			AddByte (value >> 8);
			AddByte (value & 0xFF);
		}

	protected:

		std::vector<uint8_t> m_data;
	};
}

#endif
