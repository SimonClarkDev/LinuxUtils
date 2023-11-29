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

#ifndef _THREAD_H_
#define _THREAD_H_

#include <string>
#include <thread>
#include <mutex>

/////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	///

	class Thread
	{
	public:

		static constexpr uint32_t StoppingWaitus = 100;

		enum class TaskAction {NoWait, Wait};

		Thread (const std::string& threadName, uint32_t threadDelayus) noexcept;
		virtual ~Thread ();

		Thread (const Thread& from) = delete;
		Thread (Thread&& from) = delete;
		Thread& operator = (Thread&& from) = delete;
		Thread& operator = (const Thread& from) = delete;

		void StartExecution ();
		void StopExecution ();

		static void Sleep (uint32_t delayms) noexcept;
		virtual void ThreadStart () noexcept {}
		virtual void ThreadStop () noexcept {}
		virtual TaskAction ThreadMethod () noexcept = 0;

	protected:

		uint32_t	m_threadDelayus;

	private:

		void ThreadWrapper ();

		std::thread m_thread;
		std::string	m_threadName;

		bool		m_running;
		bool		m_stopped;
	};

	////////////////////////////////////////////////////////////////////////////
	///

	class ThreadA : public Thread
	{
	public:

		virtual TaskAction ThreadMethodA (void) noexcept = 0;
		virtual TaskAction ThreadMethod (void) noexcept { return ThreadMethodA (); }
	};


	////////////////////////////////////////////////////////////////////////////
	//

	class ThreadB : public Thread
	{
	public:

		virtual TaskAction ThreadMethodB (void) noexcept = 0;
		virtual TaskAction ThreadMethod (void) noexcept { return ThreadMethodB (); }
	};

	////////////////////////////////////////////////////////////////////////////
	//

	class AutoLock
	{
		std::mutex& m_autoMutex;

	public:

		AutoLock (std::mutex& autoMutex) : m_autoMutex (autoMutex)
		{
			autoMutex.lock ();
		}
		virtual ~AutoLock ()
		{
			m_autoMutex.unlock ();
		}

		AutoLock () = delete;
		AutoLock (const AutoLock& from) = delete;
		AutoLock (AutoLock&& from) = delete;
		AutoLock& operator = (const AutoLock& from) = delete;
		AutoLock& operator = (AutoLock&& from) = delete;
	};
}

#endif
