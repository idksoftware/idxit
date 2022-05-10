/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#pragma once

#include <string>
#include <memory>
#include "ExtentionType.h"
#include "CSVArgs.h"


	class ExtentionsFilterFile;

	enum class AllowSelectionType
	{
		User,
		System,
		All
	};


	class ExtentionItem
	{
	public:
		enum class ExtError {
			NoError,
			UnknownImageType,
			ExtEmpty,
			MimeTypeEmpty,
			Unknown
		};

	private:
		std::string m_ext;
		ExtentionType m_type;
		std::string m_mimeType;
		std::string m_desciption;
		ExtError m_error;
	public:

		

		ExtentionItem() noexcept
		{
			m_ext = "";
			m_type = ExtentionType::Type::UNKNOWN_EXT;
		}

		ExtentionItem(const char* row, char delim = ':')
		{
			std::string data = row;
			//printf("%s", data.c_str());

			CSVArgs csvArgs(delim);
			csvArgs.process(row);
			m_ext = csvArgs.at(0);
			std::string typeStr = csvArgs.at(1);
			m_type = typeStr.c_str();
			m_mimeType = csvArgs.at(2);
			m_desciption = csvArgs.at(3);
		}

		ExtentionItem(std::string& ext,
					ExtentionType type,
		              std::string& mimeType,
		              std::string& desciption)
		{
			m_ext = ext;
			m_type = type;
			m_mimeType = mimeType;
			m_desciption = desciption;
		}

		const std::string& getDesciption() const
		{
			return m_desciption;
		}

		bool isValid()
		{
			if (m_type.getType() == ExtentionType::Type::UNKNOWN_EXT)
			{
				m_error = ExtError::UnknownImageType;
				return false;
			}
			if (m_ext.empty())
			{
				m_error = ExtError::ExtEmpty;
				return false;
			}
			if (m_mimeType.empty())
			{
				m_error = ExtError::MimeTypeEmpty;
				return false;
			}
			return true;
		}

		std::string getErrorString() {
			switch (m_error) {
			case ExtError::NoError:
				return "No error";
			case ExtError::UnknownImageType:
				return "No error";
			case ExtError::ExtEmpty:
				return "Extention not found";
			case ExtError::MimeTypeEmpty:
				return "Mime Type not found";
			default:
				break;
			}
			return "Unknown error";
		};

		void setDesciption(const std::string& desciption)
		{
			m_desciption = desciption;
		}

		const std::string& getExt()
		{
			return m_ext;
		}

		void setExt(const std::string& ext)
		{
			m_ext = ext;
		}

		const std::string& getMimeType() const
		{
			return m_mimeType;
		}

		void setMimeType(const std::string& mimeType)
		{
			m_mimeType = mimeType;
		}

		ExtentionType getType()
		{
			return m_type;
		}

		void setType(ExtentionType& type)
		{
			m_type = type;
		}

		std::string toString();

		ExtError getExtError() {
			return m_error;
		};
	};

	typedef std::unique_ptr<ExtentionItem> ExtentionItem_Ptr;

	class ExtentionsFilterObject
	{
	private:
		
		bool m_once;
		std::shared_ptr<ExtentionsFilterFile> m_extentionsFile;
		std::string m_extentionsFilePath;
		bool m_isError;
		static ExtentionType defaultExtentionItem;
	public:
		ExtentionsFilterObject() = default;
		~ExtentionsFilterObject() = default;;

		ExtentionType getType(const char* filename);
		ExtentionType findType(const char* ext);
		std::shared_ptr<ExtentionItem> find(const char* ext);
		bool insert(ExtentionItem& extentionItem);
		bool update(ExtentionItem& extentionItem);
		bool remove(const char* ext);
		
		bool isAllowed(const char* ext);
		//bool IsValid(const char *filename);
		bool IsValidXML(const char* filename);
		bool write();

		const std::string& getExtentionsFilePath()
		{
			return m_extentionsFilePath;
		}

		bool getList(std::vector<std::shared_ptr<ExtentionItem>>& list, AllowSelectionType selectionType);

		bool setExtentionsFilePath(const char* extentionsFilePath);

		bool isError()
		{
			return m_isError;
		}
	};

