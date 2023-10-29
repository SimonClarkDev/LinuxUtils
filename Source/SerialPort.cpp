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

////////////////////////////////////////////////////////////////////////////////
//
// https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
//

#include "SerialPort.h"

#include "spdlog/spdlog.h"

#include <asm/termbits.h>
#include <sys/ioctl.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////////
//

bool SerialPort::Open (std::string pathName, uint32_t baudRate, bool enableRTSCTS)
{
	if (!FileObject::Open (pathName)) return false;

	struct termios2 tty;
	memset (&tty, 0, sizeof tty);

	if (ioctl (m_handleId, TCGETS2, &tty) != 0)
	{
		spdlog::error ("Failed to load serial port settings {0} with error number {1}", m_pathName, errno);

		Close ();
		return false;
	}

	tty.c_cflag &= ~(PARENB|CSTOPB|CBAUD);

	tty.c_cflag |= CBAUDEX;
	tty.c_ispeed = baudRate;
	tty.c_ospeed = baudRate;

	tty.c_cflag &= ~(CS5|CS6|CS7);
	tty.c_cflag |= CS8;

	if (enableRTSCTS)
		tty.c_cflag |= CRTSCTS;
	else
		tty.c_cflag &= ~CRTSCTS;

	tty.c_cflag |= CREAD|CLOCAL;
	tty.c_lflag &= ~(ECHO|ECHOE|ECHONL|ICANON);
	tty.c_lflag &= ~ISIG;
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
	tty.c_iflag &= ~(IXON|IXOFF|IXANY);
	tty.c_oflag &= ~(OPOST|ONLCR);

	tty.c_cc[VTIME] = 0;
	tty.c_cc[VMIN] = 0;

	if (ioctl (m_handleId, TCSETS2, &tty) != 0)
	{
		spdlog::error ("Failed to save serial port settings {0} with error number {1}", m_pathName, errno);

		Close ();
		return false;
	}

	return true;
}
