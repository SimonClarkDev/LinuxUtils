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

#include "IniFile.h"
#include "FileObject.h"

#include <algorithm>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	///

	IniFile::IniFile () :
		m_currentSection (nullptr),
		m_modified (false)
	{
	}

	////////////////////////////////////////////////////////////////////////////
	///

	void IniFile::AddSection (const IniSection& newSection)
	{
		m_vectorOfSections.push_back (newSection);
		m_modified = true;
	}

	////////////////////////////////////////////////////////////////////////////
	///

	int32_t IniFile::GetItemValueAsInt (const std::string& sectionName, const std::string& entryName, int32_t defaultValue) const noexcept
	{
		for (const IniSection& nextSection : m_vectorOfSections)
		{
			if (nextSection.GetSectionName () == sectionName)
			{
				int32_t foundValue;
				if (nextSection.GetItemValueAsInt (entryName, foundValue))
				{
					return foundValue;
				}
			}
		}

		return defaultValue;
	}

	////////////////////////////////////////////////////////////////////////////
	///

	float IniFile::GetItemValueAsFloat (const std::string& sectionName, const std::string& entryName, float defaultValue) const noexcept
	{
		for (const IniSection& nextSection : m_vectorOfSections)
		{
			if (nextSection.GetSectionName () == sectionName)
			{
				float foundValue;
				if (nextSection.GetItemValueAsFloat (entryName, foundValue))
				{
					return foundValue;
				}
			}
		}

		return defaultValue;
	}

	////////////////////////////////////////////////////////////////////////////
	///

	std::string IniFile::GetItemValueAsString (const std::string& sectionName, const std::string& entryName, const std::string& defautlValue) const noexcept
	{
		for (const IniSection& nextSection : m_vectorOfSections)
		{
			if (nextSection.GetSectionName () == sectionName)
			{
				std::string foundValue;
				if (nextSection.GetItemValueAsString (entryName, foundValue))
				{
					return foundValue;
				}
            }
		}

		return defautlValue;
	}

	////////////////////////////////////////////////////////////////////////////
	///

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

	////////////////////////////////////////////////////////////////////////////
	///

	void IniFile::SetItemValue (const std::string& sectionName, const std::string& entryName, const std::string& value) noexcept
	{
		for (IniSection& nextSection : m_vectorOfSections)
		{
			if (nextSection.GetSectionName () == sectionName)
			{
				if (nextSection.SetItemValue (entryName, value))
				{
					m_modified = true;
				}
				return;
            }
		}

		// TODO: Create new section and add to that (recursive call?)
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool IniFile::ReadIniFile (const std::string& path)
	{
		ASCIIFileObject iniFile;

		if (!iniFile.Open (path)) return false;

		std::string nextLine;

		while (iniFile.ReadNextLine (nextLine))
		{
			ParseNextLine (nextLine);
		}

		m_modified = false;
		return true;
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool IniFile::WriteIniFile (const std::string& path)
	{
		ASCIIFileObject iniFile;

		if (!m_modified) return true;
		if (!iniFile.Create (path)) return false;

		for (IniSection& section : m_vectorOfSections)
		{
			std::stringstream ssSectionTitle;

			ssSectionTitle << '[' << section.GetSectionName ().c_str () << ']' << std::endl;

			if (!iniFile.WriteString (ssSectionTitle.str ())) return false;

			const std::vector<IniItem> items = section.GetItems();

			for (const IniItem& item : items)
			{
				std::stringstream ssItem;

				ssItem << item.GetName ().c_str () << " = " << item.GetValue().c_str () << std::endl;

				if (!iniFile.WriteString (ssItem.str ())) return false;
			}
		}

		m_modified = false;
		return true;
	}

	////////////////////////////////////////////////////////////////////////////
	///

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

			if (m_currentSection->SetItemValue (entryName,  entryValue))
			{
				m_modified = true;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool IniSection::SetItemValue (const std::string& name, const std::string& value) noexcept
	{
		for (IniItem& item : m_vectorOfItems)
		{
			if (item.GetName () == name)
			{
				if (item.GetValue() != value)
				{
					item = IniItem (name, value);
					return true;
				}
			}
		}

		return false;
	}

	////////////////////////////////////////////////////////////////////////////
	///

	bool IniSection::GetItemValueAsString (const std::string &name, std::string& result) const noexcept
	{
		for (const IniItem& item : m_vectorOfItems)
		{
			if (item.GetName () == name)
			{
				result = item.GetValue ();
				return true;
			}
		}

		return false;
	}

    ////////////////////////////////////////////////////////////////////////////
    ///
    
    bool IniSection::GetItemValueAsInt (const std::string &name, int32_t& result) const noexcept
    {
	    std::string value;
	    if (GetItemValueAsString (name, value))
	    {
		    if (!IniItem::IsInt (value)) return false;
		    result = static_cast<int32_t>(std::stoi (value));
		    return true;
	    }

	    return false;
    }

    ////////////////////////////////////////////////////////////////////////////
    ///

    bool IniSection::GetItemValueAsFloat (const std::string &name, float& result) const noexcept
    {
	    std::string value;
	    if (GetItemValueAsString (name, value))
	    {
		    if (!IniItem::IsFloat (value)) return false;
		    result = std::stof (value);
		    return true;
	    }

	    return false;
    }

	////////////////////////////////////////////////////////////////////////////
	///

    bool IniSection::GetItemValueAsTime (const std::string& name, uint8_t& hours,
        uint8_t& minutes, uint8_t& seconds) const noexcept
    {
	    std::string asciiTime;

	    if (!GetItemValueAsString (name, asciiTime)) return false;
	    if (asciiTime.length () != 8) return false;
	    if (asciiTime[HourToMinuteColonOffset] != TimeColon ||
		    asciiTime[MinuteToSecondColonOffset] != TimeColon) return false;

	    std::string asciiHours = asciiTime.substr (0, 2);
	    std::string asciiMinutes = asciiTime.substr (3, 2);
	    std::string asciiSeconds = asciiTime.substr (6, 2);

	    if (!IniItem::IsInt (asciiHours)) return false;
	    if (!IniItem::IsInt (asciiMinutes)) return false;
	    if (!IniItem::IsInt (asciiSeconds)) return false;

	    hours = static_cast<uint8_t>(std::stoi (asciiHours));
	    minutes = static_cast<uint8_t>(std::stoi (asciiMinutes));
	    seconds = static_cast<uint8_t>(std::stoi (asciiSeconds));
        
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    ///

	bool IniItem::IsInt (const std::string& value) noexcept
	{
		for (char const &character : value)
		{
			if (!std::isdigit (character)) return false;
		}
		return true;
	}


    ///////////////////////////////////////////////////////////////////////////
    ///

	bool IniItem::IsFloat (const std::string& value) noexcept
	{
		for (char const &character : value)
		{
			if (!std::isdigit (character) && character != '.') return false;
		}
		return true;
	}
}
