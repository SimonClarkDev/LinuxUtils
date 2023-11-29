////////////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2022 Open Technologies Limited
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#ifndef _THREAD_QUEUE_H_
#define _THREAD_QUEUE_H_

#include <deque>
#include <mutex>

/////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	///

	template<typename T>
	class ITQCompare
	{
		public:

		virtual ~ITQCompare () {}
		virtual bool Compare (const T& ta, const T& tb) const noexcept = 0;
	};

	template<typename T>
	class ThreadQueue : private std::deque<T>
	{
		std::mutex m_protectionMutex;

		public:

		ThreadQueue () = default;
		virtual ~ThreadQueue () = default;
		ThreadQueue (const ThreadQueue& from) = delete;
		ThreadQueue (ThreadQueue&& from) = delete;
		ThreadQueue& operator = (const ThreadQueue& from) = delete;
		ThreadQueue& operator = (ThreadQueue&& from) = delete;

		[[nodiscard]] const T& Front () noexcept
		{
			std::scoped_lock lock (m_protectionMutex);
			return std::deque<T>::front ();
		}

		void PushBack (const T& item) noexcept
		{
			std::scoped_lock lock (m_protectionMutex);
			std::deque<T>::push_back (item);
		}

		[[nodiscard]] size_t Size () noexcept
		{
			std::scoped_lock lock (m_protectionMutex);
			return std::deque<T>::size ();
		}

		void PopFront () noexcept
		{
			std::scoped_lock lock (m_protectionMutex);
			std::deque<T>::pop_front ();
		}

		[[nodiscard]] bool Find (const T& find, const ITQCompare<T&> compareBy) noexcept
		{
			std::scoped_lock lock (m_protectionMutex);
			for (const T& message : this)
			{
				if (!compareBy.Compare (message, find)) return true;
			}

			return false;
		}
	};
}

#endif
