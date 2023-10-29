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

#ifndef _RTTHREAD_H_
#define _RTTHREAD_H_

#include "Clock.h"
#include "Thread.h"

////////////////////////////////////////////////////////////////////////////////
///

class OneSecondThread : protected Thread
{
public:

	OneSecondThread (const std::string& threadName);
	virtual ~OneSecondThread () = default;

protected:

	virtual void ClockMethod () noexcept = 0;

private:

	static constexpr uint32_t RealTimeThreadWait = 10000; // us

	virtual void ThreadStart () noexcept;
	virtual TaskAction ThreadMethod () noexcept;

	Clock m_clock;
	uint32_t m_clockSeconds;
};

#endif /* _RTTHREAD_H_ */
