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


	class FolderFilterFile;

	


	class FolderItem
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
		std::string m_path;
		bool m_isFullPath;
		std::string m_desciption;
		ExtError m_error{ ExtError::NoError };
	public:

		

		FolderItem() noexcept
		{};

		FolderItem(const char* row, char delim = ':')
		{
			std::string data = row;
			//printf("%s", data.c_str());

			CSVArgs csvArgs(delim);
			csvArgs.process(row);
			m_path = csvArgs.at(0);
			std::string fullPathStr = csvArgs.at(1);
			m_isFullPath = (fullPathStr.compare("True") == 0);
			m_desciption = csvArgs.at(2);
		}

		FolderItem(std::string& path,
					bool isFullPath,
					std::string& desciption)
		{
			m_path = path;
			m_isFullPath = isFullPath;
			m_desciption = desciption;
		}

		const std::string& getDesciption() const
		{
			return m_desciption;
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

		const std::string& getPath()
		{
			return m_path;
		}

		void setExt(const std::string& path)
		{
			m_path = path;
		}

		bool isFullPath()
		{
			return m_isFullPath;
		}

		void setType(bool isFullPath)
		{
			m_isFullPath = isFullPath;
		}

		std::string toString();

		ExtError getExtError() {
			return m_error;
		};
	};

	typedef std::unique_ptr<FolderItem> FolderItem_Ptr;

	class FolderFilterObject
	{
	private:
		
		bool m_once;
		std::shared_ptr<FolderFilterFile> m_folderFile;
		std::string m_folderFilePath;
		bool m_isError;
		static ExtentionType defaultExtentionItem;
	public:
		FolderFilterObject() = default;
		~FolderFilterObject() = default;

		bool init();

		bool findPath(const char* path);
		std::shared_ptr<FolderItem> find(const char* path);
		bool insert(FolderItem& folderItem);
		bool update(FolderItem& folderItem);
		bool remove(const char* path);
		
		bool isAllowed(const char* path);
		//bool IsValid(const char *filename);
		bool IsValidXML(const char* filename);
		bool write();

		const std::string& getFolderFilePath()
		{
			return m_folderFilePath;
		}

		bool getList(std::vector<std::shared_ptr<FolderItem>>& list);

		bool setFolderFilePath(const char* folderFilePath);

		bool isError()
		{
			return m_isError;
		}
	};

