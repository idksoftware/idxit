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

#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include "ExtentionsFilterFile.h"
#include "SAUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif


	std::string ExtentionItem::toString()
	{
		std::stringstream s;
		s << m_ext << ':' << m_type.toString() << ':' << m_mimeType << ':' << m_desciption;
		return s.str();
	}

	typedef std::map<std::string, std::shared_ptr<ExtentionItem>> ExtentionsContainer;


	class ExtentionsFilterFile
	{
		ExtentionsContainer m_extentionsContainer;
		bool insert(const char* row);
	public:
		ExtentionsFilterFile() = default;
		virtual ~ExtentionsFilterFile() = default;

		bool read(const char* datafile);
		bool write(const char* datafile);
		bool remove(const char* ext);
		bool edit(ExtentionItem& extentionItem);
		bool insert(ExtentionItem& extentionItem);
		std::shared_ptr<ExtentionItem> find(const char* ext);
		bool getList(std::vector<std::shared_ptr<ExtentionItem>>& list, AllowSelectionType selectionType);
	};


	bool ExtentionsFilterFile::read(const char* datafile)
	{
		std::string text;
		std::ifstream file(datafile);
		if (file.is_open() == false)
		{
			return false;
		}

		while (std::getline(file, text))
		{
			insert(text.c_str());
		}
		file.close();

		return true;
	}

	bool ExtentionsFilterFile::write(const char* datafile)
	{
		std::ofstream file(datafile);
		if (file.is_open() == false)
		{
			return false;
		}
		for (auto ii = m_extentionsContainer.begin(); ii != m_extentionsContainer.end(); ++ii)
		{
			std::shared_ptr<ExtentionItem> data = ii->second;
			file << data->toString() << '\n';
		}

		file.close();
		return true;
	}

	bool ExtentionsFilterFile::getList(std::vector<std::shared_ptr<ExtentionItem>>& list, AllowSelectionType selectionType)
	{
		for (auto ii = m_extentionsContainer.begin(); ii != m_extentionsContainer.end(); ++ii)
		{
			std::shared_ptr<ExtentionItem> data = ii->second;
			ExtentionType type = data->getType();
			switch (selectionType)
			{
			case AllowSelectionType::System:
				if (type.getType() == ExtentionType::Type::SYSTEM_EXT)
				{
					list.push_back(data);
				}
				break;
			case AllowSelectionType::User:
				if (type.getType() == ExtentionType::Type::USER_EXT)
				{
					list.push_back(data);
				}
				break;
			case AllowSelectionType::All:
				list.push_back(data);
				break;
			default:
				return false;
			}
		}
		return true;
	}

	bool ExtentionsFilterFile::remove(const char* ext)
	{
		return (m_extentionsContainer.erase(ext)) ? true : false;
	}

	bool ExtentionsFilterFile::edit(ExtentionItem& extentionItem)
	{
		auto i = m_extentionsContainer.find(extentionItem.getExt());
		bool replaced = (i != m_extentionsContainer.end());
		std::shared_ptr<ExtentionItem> extentionItem_ptr = std::make_shared<ExtentionItem>(extentionItem);
		m_extentionsContainer[extentionItem.getExt()] = extentionItem_ptr;
		return replaced;
	}

	/*
	bool CExtentionsFile::add(const char *ext, const char *name) {
	
		std::string extStr = ext;
		std::string nameStr = name;
		std::string *row = new std::string(extStr + ':' + nameStr);
		if (find(ext) != -1) {
			return false; // found
		}
		//insert(ext, *row);
		return true;
	}
	*/
	bool ExtentionsFilterFile::insert(const char* row)
	{
		ExtentionItem extentionItem(row);
		return insert(extentionItem);
	}

	bool ExtentionsFilterFile::insert(ExtentionItem& extentionItem)
	{
		auto i = m_extentionsContainer.find(extentionItem.getExt());
		if (i == m_extentionsContainer.end())
		{
			// insert if failed to find
			std::shared_ptr<ExtentionItem> extentionItem_ptr = std::make_shared<ExtentionItem>(extentionItem);
			m_extentionsContainer[extentionItem.getExt()] = extentionItem_ptr;
			return true;
		}
		return false;
	}


	std::shared_ptr<ExtentionItem> ExtentionsFilterFile::find(const char* ext)
	{
		std::string tmp = ext;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
		auto it = m_extentionsContainer.find(tmp);
		bool found = (it != m_extentionsContainer.end());
		if (found)
		{
			std::shared_ptr<ExtentionItem> itemPtr(it->second);
			return itemPtr;
		}

		return nullptr;
	}

	//std::string ExtentionsFilterObject::m_extentionsFilePath;

	//std::unique_ptr<ExtentionsFilterFile> ExtentionsFilterObject::m_extentionsFile = nullptr;
	//bool ExtentionsFilterObject::m_once = true;
	//bool ExtentionsFilterObject::m_isError = false;
	//static ImageType defaultImageType;
	ExtentionType ExtentionsFilterObject::defaultExtentionItem;

	
	/*
	ExtentionsFilterObject& ExtentionsFilterObject::get()
	{
		static ExtentionsFilterObject INSTANCE;
		if (m_once)
		{
			std::string path = m_extentionsFilePath + "/ext.dat";
			if (SAUtils::FileExists(path.c_str()) == false)
			{
				m_isError = true;
				throw std::exception();
			}
			m_extentionsFile = std::make_unique<ExtentionsFilterFile>();
			m_extentionsFile->read(path.c_str());
			m_once = false;
		}
		return INSTANCE;
	}
	*/
	

	bool ExtentionsFilterObject::getList(std::vector<std::shared_ptr<ExtentionItem>>& list, AllowSelectionType selectionType)
	{
		return m_extentionsFile->getList(list, selectionType);
	}

	bool ExtentionsFilterObject::write()
	{
		std::string path = m_extentionsFilePath + "/ext.dat";
		if (SAUtils::FileExists(path.c_str()) == false)
		{
			m_isError = true;
			throw std::exception();
		}

		return m_extentionsFile->write(path.c_str());
	}
	
	bool ExtentionsFilterObject::setExtentionsFilePath(const char* extentionsFilePath)
	{
		m_extentionsFilePath = extentionsFilePath;
		std::string path = m_extentionsFilePath + "/ext.dat";
		if (SAUtils::FileExists(path.c_str()) == false)
		{
			m_isError = true;
			return false;
		}
		return true;
	}

	

	
	std::shared_ptr<ExtentionItem> ExtentionsFilterObject::find(const char* filename)
	{
		std::string ext = SAUtils::getExtention(filename);
		std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
		std::shared_ptr<ExtentionItem> item = m_extentionsFile->find(ext.c_str());
		return item;
	}

	bool ExtentionsFilterObject::insert(ExtentionItem& extentionItem)
	{
		return m_extentionsFile->insert(extentionItem);
	}

	bool ExtentionsFilterObject::update(ExtentionItem& extentionItem)
	{
		return m_extentionsFile->edit(extentionItem);
	}

	bool ExtentionsFilterObject::remove(const char* ext)
	{
		return m_extentionsFile->remove(ext);
	}

	bool ExtentionsFilterObject::isAllowed(const char* ext)
	{
		std::string tmp = ext;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
		std::shared_ptr<ExtentionItem> item = m_extentionsFile->find(ext);
		if (item == nullptr)
		{
			return false;
		}
		return true;
	}

	ExtentionType ExtentionsFilterObject::findType(const char* ext)
	{
		std::string tmp = ext;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
		std::shared_ptr<ExtentionItem> item = m_extentionsFile->find(ext);
		if (item != nullptr)
		{
			ExtentionType type = item->getType();
			return type;
		}
		return defaultExtentionItem;
	}

	ExtentionType ExtentionsFilterObject::getType(const char* filename)
	{
		std::string ext = SAUtils::getExtention(filename);
		return findType(ext.c_str());
	}

	/*
	bool ExtentionsFilterObject::IsValid(const char *filename) {
	
		if (SAUtils::hasExt(filename) == false) {
			return false;
		}
		std::string ext = SAUtils::getExtention(filename);
		ExtentionType& type = findType(ext.c_str());
		if (type.getType() == ExtentionType::Type::UNKNOWN_EXT) {
			return false;
		}
		return true;
	}
	*/

	
	bool ExtentionsFilterObject::IsValidXML(const char* filename)
	{
		if (SAUtils::hasExt(filename) == false)
		{
			return false;
		}
		std::string ext = SAUtils::getExtention(filename);
		std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
		if (ext.compare("xml") == 0)
		{
			return true;
		}
		return false;
	}

