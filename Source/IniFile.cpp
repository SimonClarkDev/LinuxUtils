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

#include "../Include/IniFile.h"

#include <algorithm>

#include "../Include/FileObject.h"

////////////////////////////////////////////////////////////////////////////////
//

IniFile::IniFile () :
	m_currentSection (nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
//

void IniFile::AddSection (const IniSection& newSection)
{
	m_vectorOfSections.push_back (newSection);
}

////////////////////////////////////////////////////////////////////////////////
//

int32_t IniFile::GetItemValueAsInt (const std::string& sectionName, const std::string& entryName) const noexcept
{
	for (const IniSection& nextSection : m_vectorOfSections)
	{
		if (nextSection.GetSectionName () == sectionName)
		{
			return nextSection.GetItemValueAsInt (entryName);
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//

std::string IniFile::GetItemValueAsString (const std::string& sectionName, const std::string& entryName) const noexcept
{
	for (const IniSection& nextSection : m_vectorOfSections)
	{
		if (nextSection.GetSectionName () == sectionName)
		{
			return nextSection.GetItemValueAsString (entryName);
		}
	}

	return "";
}

////////////////////////////////////////////////////////////////////////////////
//

bool IniFile::GetItemValueAsTime (const std::string& sectionName, const std::string& entryName, uint8_t& hours, uint8_t& minutes, uint8_t& seconds) const noexcept
{
	for (const IniSection& nextSection : m_vectorOfSections)
	{
		if (nextSection.GetSectionName () == sectionName)
		{
			return nextSection.GetItemValueAsTime (entryName, hours, minutes, seconds);
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
//

bool IniFile::ReadIniFile (const std::string& path)
{
	FileObject iniFile;

	if (!iniFile.Open (path)) return false;

	while (true)
	{
		std::string nextLine = ReadNextLine (iniFile);

		if (nextLine.length () == 0)
		{
			return true;
		}

		ParseNextLine (nextLine);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//

std::string IniFile::ReadNextLine (FileObject& file) noexcept
{
	uint8_t nextByte = 0;
	std::string nextLine;

	while (file.Read (&nextByte, 1) != 0)
	{
		if (nextByte == CarriageReturn || nextByte == LineFeed)
		{
			if (!nextLine.empty ())
			{
				return nextLine;
			}
		}
		else
		{
			nextLine += static_cast<char>(nextByte);
		}
	}

	return nextLine;
}

////////////////////////////////////////////////////////////////////////////////
//

void IniFile::ParseNextLine (std::string& line) noexcept
{
	line.erase (std::remove_if (line.begin (), line.end (), isspace), line.end ());
	if (line.length () == 0) return;

	if (line[0] == SectionNameOpen && line[line.length () - 1] == SectionNameClose)
	{
		m_currentSection = nullptr;

		std::string section = line.substr (1, line.length () - 2);

		for (IniSection& item : m_vectorOfSections)
		{
			if (item.GetSectionName () == section)
			{
				m_currentSection = &item;
				return;
			}
		}

		AddSection (IniSection (section));
	}
	else if (m_currentSection != nullptr)
	{
		size_t position = line.find (ItemAssignment);

		if (position == std::string::npos) return;

		std::string entryName = line.substr (0, position);
		std::string entryValue = line.substr (position + 1, line.length () - 1);

		m_currentSection->SetItemValue (entryName,  entryValue);
	}
}

////////////////////////////////////////////////////////////////////////////////
//

void IniSection::SetItemValue (const std::string& name, const std::string& value) noexcept
{
	for (IniItem& item : m_vectorOfItems)
	{
		if (item.GetName () == name)
		{
			item = IniItem (name, value);
			return;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//

std::string IniSection::GetItemValueAsString (const std::string &name) const noexcept
{
	for (const IniItem& item : m_vectorOfItems)
	{
		if (item.GetName () == name)
		{
			return item.GetValue ();
		}
	}

	return "";
}

////////////////////////////////////////////////////////////////////////////////
//

int32_t IniSection::GetItemValueAsInt (const std::string &name) const noexcept
{
	std::string value = GetItemValueAsString (name);
	return static_cast<int32_t>(std::stoi (value));
}

////////////////////////////////////////////////////////////////////////////////
//

bool IniSection::GetItemValueAsTime (const std::string& name, uint8_t& hours, uint8_t& minutes, uint8_t& seconds) const noexcept
{
	std::string asciiTime = GetItemValueAsString (name);

	if (asciiTime.length () != 8) return false;
	if (asciiTime[HourToMinuteColonOffset] != TimeColon ||
		asciiTime[MinuteToSecondColonOffset] != TimeColon) return false;

	hours = static_cast<uint8_t>(std::stoi (asciiTime.substr (0, 2)));
	minutes = static_cast<uint8_t>(std::stoi (asciiTime.substr (3, 2)));
	seconds = static_cast<uint8_t>(std::stoi (asciiTime.substr (6, 2)));

	if (hours > 23) return false;
	if (minutes > 59) return false;
	if (seconds > 59) return false;
	return true;
}
