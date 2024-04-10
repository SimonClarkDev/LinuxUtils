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

#ifndef _INI_FILE_H_
#define _INI_FILE_H_

#include <memory>
#include <optional>
#include <string>
#include <vector>

class FileObject;

////////////////////////////////////////////////////////////////////////////////
///

namespace spc
{
	////////////////////////////////////////////////////////////////////////////
	///

	class IniItem
	{
	public:

		IniItem (std::string name, const std::string& value) :
			m_name (name),
			m_value (value)
		{
		}
		virtual ~IniItem() = default;

		IniItem (IniItem&& from) = default;
		IniItem (const IniItem& from)
		{
			operator = (from);
		}

		IniItem& operator = (IniItem&& from) = default;
		IniItem& operator = (const IniItem& from)
		{
			m_name = from.m_name;
			m_value = from.m_value;
			return *this;
		}

		[[nodiscard]] static bool IsInt (const std::string& value) noexcept;
		[[nodiscard]] static bool IsFloat (const std::string& value) noexcept;

		[[nodiscard]] std::string GetName () const noexcept { return m_name; }
		[[nodiscard]] std::string GetValue () const noexcept { return m_value; }

	private:

		std::string m_name;
		std::string m_value;
	};

	////////////////////////////////////////////////////////////////////////////
	///

	class IniSection
	{
	public:

		IniSection (const std::string& sectionName) :
			m_sectionName (sectionName)
		{
		}

		virtual ~IniSection () = default;
		IniSection (IniSection&& from) = default;
		IniSection (const IniSection& from) = default;
		IniSection& operator = (IniSection&& from) = default;
		IniSection& operator = (const IniSection& from)
		{
			m_sectionName = from.m_sectionName;
			m_vectorOfItems = from.m_vectorOfItems;
			return *this;
		}

		void AddItem (const IniItem& iniItem)
		{
			m_vectorOfItems.push_back (iniItem);
		}

		[[nodiscard]] const std::vector<IniItem> GetItems () const noexcept { return m_vectorOfItems; }

		[[nodiscard]] bool SetItemValue (const std::string& name, const std::string& value) noexcept;
		[[nodiscard]] bool GetItemValueAsString (const std::string& name, std::string& result) const noexcept;
		[[nodiscard]] bool GetItemValueAsTime (const std::string& name, uint8_t& hours, uint8_t& minutes, uint8_t& seconds) const noexcept;
		[[nodiscard]] bool GetItemValueAsInt (const std::string &name, int32_t& result) const noexcept;
		[[nodiscard]] bool GetItemValueAsFloat (const std::string &name, float& result) const noexcept;

		[[nodiscard]] std::string GetSectionName () const noexcept { return m_sectionName; }

	private:

		static constexpr uint32_t HourToMinuteColonOffset = 2;
		static constexpr uint32_t MinuteToSecondColonOffset = 5;
		static constexpr char TimeColon = ':';

		std::vector<IniItem> m_vectorOfItems;
		std::string m_sectionName;
	};

	////////////////////////////////////////////////////////////////////////////
	///

	class IniFile
	{
	public:

		IniFile ();
		virtual ~IniFile () = default;
		IniFile (IniFile&& from) = delete;
		IniFile (const IniFile& from) = delete;
		IniFile& operator = (const IniFile&& from) = delete;
		IniFile& operator = (const IniFile& from) = delete;

		void AddSection (const IniSection& newSection);
		[[nodiscard]] bool WriteIniFile (const std::string& path);
		[[nodiscard]] bool ReadIniFile (const std::string& path);
		[[nodiscard]] std::string GetItemValueAsString (const std::string& sectionName, const std::string& entryName, const std::string& defaultValue = "") const noexcept;
		[[nodiscard]] int32_t GetItemValueAsInt (const std::string& sectionName, const std::string& entryName, int32_t defaultValue = 0) const noexcept;
		[[nodiscard]] float GetItemValueAsFloat (const std::string& sectionName, const std::string& entryName, float defaultValue = 0.0f) const noexcept;
		[[nodiscard]] bool GetItemValueAsTime (const std::string& sectionName, const std::string& entryName, uint8_t& hours, uint8_t& minutes, uint8_t& seconds) const noexcept;

		void SetItemValue (const std::string& sectionName, const std::string& entryName, const std::string& value) noexcept;

	private:

		static constexpr char SectionNameOpen = '[';
		static constexpr char SectionNameClose = ']';
		static constexpr char ItemAssignment = '=';

		void ParseNextLine (std::string& line) noexcept;

		std::vector<IniSection> m_vectorOfSections;
		IniSection* m_currentSection;
		bool m_modified;
	};
}

#endif
