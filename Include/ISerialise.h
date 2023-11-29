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

#ifndef _I_SERIALISE_H_
#define _I_SERIALISE_H_

#include <cstdint>
#include <cstddef>

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Base class to define interface for common Linux serialisation types
	/// </summary>

	class ISerialise
	{
	public:

		ISerialise () : m_handleId (INVALID_HANDLE_VALUE) {	}
		virtual ~ISerialise () = default;

		[[nodiscard]] bool IsOpen () const noexcept {return m_handleId != INVALID_HANDLE_VALUE;}

		[[nodiscard]] virtual bool Send (const uint8_t* pBuffer, size_t length) = 0;
		[[nodiscard]] virtual size_t Read (uint8_t* pBuffer, size_t length) = 0;
		virtual void Close () = 0;
		[[nodiscard]] virtual bool ReOpen () = 0;

	protected:

		static constexpr int32_t INVALID_HANDLE_VALUE = -1;

		int32_t m_handleId;
	};
}

#endif
