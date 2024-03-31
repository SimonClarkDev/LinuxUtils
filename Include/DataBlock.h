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
	template <typename T>
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
		[[nodiscard]] const T* GetDataAt (size_t offset = 0) const noexcept { return &m_data.data ()[offset]; }
		[[nodiscard]] T* GetDataPointer () const noexcept { return const_cast<T*>(m_data.data ()); }
		[[nodiscard]] const std::vector<T> GetData () const noexcept { return m_data; }

		[[nodiscard]] T operator [] (int32_t offset) { return m_data.data ()[offset]; }

		bool SetSize (size_t newSize) noexcept
		{
			try
			{
				m_data.reserve (newSize);
				m_data.clear ();
			}
			catch(const std::exception&)
			{
				return false;
			}

			return true;
		}

		void SetData (const T* dataPointer, size_t lengthOfData) noexcept
		{
			m_data.clear ();
			AddData (dataPointer, lengthOfData);
		}

		void Clear () noexcept { m_data.clear (); }

		[[nodiscard]] T* ExpandBy (uint32_t numberOfElements) noexcept
		{
			size_t currentSize = m_data.size ();
			m_data.resize (currentSize + numberOfElements);
			return m_data.data () + currentSize;
		}

		void AddData (const T* dataPointer, size_t lengthOfData) noexcept
		{
			size_t currentSize = m_data.size ();
			m_data.resize (currentSize + lengthOfData);
			std::memmove (m_data.data () + currentSize, dataPointer, lengthOfData);
		}

	protected:

		std::vector<T> m_data;
	};

	////////////////////////////////////////////////////////////////////////////
	///

	struct ByteBlock : protected DataBlock<uint8_t>
	{
		ByteBlock () = default;
		virtual ~ByteBlock () = default;

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
	};
}

#endif
