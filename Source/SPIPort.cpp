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

#include <linux/spi/spidev.h>

#include <memory>
#include <spdlog/spdlog.h>
#include <sys/ioctl.h>

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	///

	bool SPIPort::Initialise (uint32_t speedInHz, SPIPort::Mode mode, uint32_t bitPerWord) noexcept
	{
		if (!FileObject::IsOpen ())
		{
			spdlog::error ("SPI port must be opened before it is initialised");
			return false;
		}

		if (ioctl (m_handleId, SPI_IOC_WR_MAX_SPEED_HZ, &speedInHz) == -1)
		{
			spdlog::error ("Failed to setup SPI port speed with errno {}", errno);
			return false;
		}

		uint32_t spiMode = 0;
		switch (mode)
		{
		case	Mode::MODE_0: spiMode = SPI_MODE_0; break;
		case	Mode::MODE_1: spiMode = SPI_MODE_1; break;
		case	Mode::MODE_2: spiMode = SPI_MODE_2; break;
		case	Mode::MODE_3: spiMode = SPI_MODE_3; break;
		}

		if (ioctl (m_handleId, SPI_IOC_WR_MODE, &spiMode) == -1)
		{
			spdlog::error ("Failed to setup SPI port mode with errno {}", errno);
			return false;
		}

		if (ioctl (m_handleId, SPI_IOC_WR_BITS_PER_WORD, &bitPerWord) == -1)
		{
			spdlog::error ("Failed to setup SPI port bits with errno {}", errno);
			return false;
		}

		return true;
	}

	////////////////////////////////////////////////////////////////////////////
	///

    bool SPIPort::Initialise (uint32_t speedInHz, SPIPort::Mode mode, uint32_t bitPerWord) noexcept
    {
	    if (!FileObject::IsOpen ())
	    {
		    spdlog::error ("SPI port must be opened before it is initialised");
            return false;
	    }

        if (ioctl (m_handleId, SPI_IOC_WR_MAX_SPEED_HZ, &speedInHz) == -1)
        {
		    spdlog::error ("Failed to setup SPI port speed with errno {}", errno);
            return false;
        }

        uint32_t spiMode = 0;
        switch (mode)
        {
        case	Mode::MODE_0: spiMode = SPI_MODE_0; break;
        case	Mode::MODE_1: spiMode = SPI_MODE_1; break;
        case	Mode::MODE_2: spiMode = SPI_MODE_2; break;
        case	Mode::MODE_3: spiMode = SPI_MODE_3; break;
        }

        if (ioctl (m_handleId, SPI_IOC_WR_MODE, &spiMode) == -1)
        {
		    spdlog::error ("Failed to setup SPI port mode with errno {}", errno);
            return false;
        }

        if (ioctl (m_handleId, SPI_IOC_WR_BITS_PER_WORD, &bitPerWord) == -1)
        {
		    spdlog::error ("Failed to setup SPI port bits with errno {}", errno);
            return false;
        }
        
        ////////////////////////////////////////////////////////////////////////////
        ///

	    bool SPIPort::Open (const std::string& pathName) noexcept
	    {
		    return FileObject::Open (pathName);
	    }

	    ////////////////////////////////////////////////////////////////////////////
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

		    return true;
        }

	    ////////////////////////////////////////////////////////////////////////////
	    ///

	    size_t SPIPort::Read (uint8_t* pBuffer, size_t length) noexcept
	    {
		    if (!Write (pBuffer, length))
		    {
			    spdlog::error ("SPI read transfer failed");
			    return 0;
		    }

		    memmove (pBuffer, m_recvBuffer, length);
		    return length;
	    }
    }
}
