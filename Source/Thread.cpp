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

#include "../Include/Thread.h"

#include "spdlog/spdlog.h"
#include <chrono>

/////////////////////////////////////////////////////////////////////////////////
//

Thread::Thread (const std::string& threadName, uint32_t threadDelayus) noexcept :
	m_threadDelayus (threadDelayus),
	m_threadName (threadName),
	m_running (false),
	m_stopped (true)
{
}

/////////////////////////////////////////////////////////////////////////////////
//

Thread::~Thread ()
{
	StopExecution ();
}

/////////////////////////////////////////////////////////////////////////////////
//

void Thread::StartExecution ()
{
	if (m_running)
	{
		spdlog::error ("Thread {0} already running", m_threadName);
		return;
	}

	m_thread = std::thread (&Thread::ThreadWrapper, this);
}

/////////////////////////////////////////////////////////////////////////////////
//

void Thread::Sleep (uint32_t delayms) noexcept
{
	usleep (delayms * 1000);
}

/////////////////////////////////////////////////////////////////////////////////
//

void Thread::StopExecution ()
{
	m_running = false;
	while (!m_stopped) usleep (StoppingWaitus);
}

/////////////////////////////////////////////////////////////////////////////////
//

void Thread::ThreadWrapper ()
{
	m_stopped = false;
	m_running = true;

	spdlog::trace ("Thread {0} starting", m_threadName);
	ThreadStart ();
	spdlog::trace ("Thread {0} started", m_threadName);

	std::chrono::duration<long int, std::micro> threadWaitDelay (m_threadDelayus);
	auto expectedEndTime = std::chrono::high_resolution_clock::now ();

	while (m_running)
	{
		if (ThreadMethod () == TaskAction::Wait)
		{
			auto currentTime = std::chrono::high_resolution_clock::now ();
			expectedEndTime += threadWaitDelay;

			std::chrono::duration<double, std::micro> sleepDuraction = expectedEndTime - currentTime;
			if (currentTime < expectedEndTime)
			{
				usleep (sleepDuraction.count ());
			}
			else
			{
				//spdlog::error ("Thread overrun time slice by {}us", std::abs (sleepDuraction.count ()));
			}
		}
	}

	spdlog::trace ("Thread {0} stopping", m_threadName);
	ThreadStop ();
	spdlog::trace ("Thread {0} stopped", m_threadName);

	m_stopped = true;
}
