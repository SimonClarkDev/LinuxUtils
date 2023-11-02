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

#include "SPIPort.h"

#include <memory>
#include <linux/spi/spidev.h>
#include <spdlog/spdlog.h>
#include <sys/ioctl.h>

////////////////////////////////////////////////////////////////////////////////
///

SPIPort::SPIPort () :
	m_sentBytes (0)
{
}

////////////////////////////////////////////////////////////////////////////////
///

bool SPIPort::Open (const std::string& pathName, uint32_t speed) noexcept
{
	if (!FileObject::Open (pathName)) return false;

    uint8_t mode = SPI_MODE_0;

    if (ioctl (m_handleId, SPI_IOC_WR_MODE, &mode) == -1)
    {
		spdlog::error ("Failed to setup SPI port mode with errno {}", errno);
        return false;
    }

    if (ioctl (m_handleId, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
    {
		spdlog::error ("Failed to setup SPI port speed with errno {}", errno);
        return false;
    }

	return true;
}

////////////////////////////////////////////////////////////////////////////////
///

bool SPIPort::Write (const uint8_t* pBuffer, size_t length) noexcept
{
	if (length == 0 || length > MAX_BUFFER_SIZE)
	{
		spdlog::error ("SPI write length is invalid. Must be between 1 and {} bytes", MAX_BUFFER_SIZE);
		return false;
	}

    struct spi_ioc_transfer transfer = {};

    transfer.tx_buf = reinterpret_cast<uint64_t>(pBuffer);
    transfer.rx_buf = reinterpret_cast<uint64_t>(m_recvBuffer);
    transfer.len = length;

    if (ioctl (m_handleId, SPI_IOC_MESSAGE (1), &transfer) < 0)
    {
    	spdlog::error ("SPI transfer failed with errno {}", errno);
        return false;
    }

    m_sentBytes = length;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///

size_t SPIPort::Read (uint8_t* pBuffer, size_t length) noexcept
{
	if (length == 0 || length > m_sentBytes)
	{
		spdlog::error ("SPI read length is invalid. Must be between 1 and {} bytes", m_sentBytes);
		return 0;
	}

	memmove (pBuffer, m_recvBuffer, length);
	return length;
}
