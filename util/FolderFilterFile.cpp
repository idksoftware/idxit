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
#include "FolderFilterFile.h"
#include "SAUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif


	std::string FolderItem::toString()
	{
		std::stringstream s;
		s << m_path << ':' << ((m_isFullPath)?"True":"False") << ':' << m_desciption;
		return s.str();
	}

	typedef std::map<std::string, std::shared_ptr<FolderItem>> ExtentionsContainer;


	class FolderFilterFile
	{
		ExtentionsContainer m_extentionsContainer;
		bool insert(const char* row);
	public:
		FolderFilterFile() = default;
		virtual ~FolderFilterFile() = default;

		bool read(const char* datafile);
		bool write(const char* datafile);
		bool remove(const char* ext);
		bool edit(FolderItem& extentionItem);
		bool insert(FolderItem& extentionItem);
		std::shared_ptr<FolderItem> find(const char* ext);
		bool getList(std::vector<std::shared_ptr<FolderItem>>& list);
	};


	bool FolderFilterFile::read(const char* datafile)
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

	bool FolderFilterFile::write(const char* datafile)
	{
		std::ofstream file(datafile);
		if (file.is_open() == false)
		{
			return false;
		}
		for (auto ii = m_extentionsContainer.begin(); ii != m_extentionsContainer.end(); ++ii)
		{
			std::shared_ptr<FolderItem> data = ii->second;
			file << data->toString() << '\n';
		}

		file.close();
		return true;
	}

	bool FolderFilterFile::getList(std::vector<std::shared_ptr<FolderItem>>& list)
	{
		if (m_extentionsContainer.empty() == true) {
			return true;
		}
		for (auto ii : m_extentionsContainer)
		{
			std::shared_ptr<FolderItem> data = ii.second;
			list.push_back(data);
		}
		return true;
	}

	bool FolderFilterFile::remove(const char* ext)
	{
		return (m_extentionsContainer.erase(ext)) ? true : false;
	}

	bool FolderFilterFile::edit(FolderItem& folderItem)
	{
		auto i = m_extentionsContainer.find(folderItem.getPath());
		bool replaced = (i != m_extentionsContainer.end());
		std::shared_ptr<FolderItem> extentionItem_ptr = std::make_shared<FolderItem>(folderItem);
		m_extentionsContainer[folderItem.getPath()] = extentionItem_ptr;
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
	bool FolderFilterFile::insert(const char* row)
	{
		FolderItem folderItem(row);
		return insert(folderItem);
	}

	bool FolderFilterFile::insert(FolderItem& folderItem)
	{
		auto i = m_extentionsContainer.find(folderItem.getPath());
		if (i == m_extentionsContainer.end())
		{
			// insert if failed to find
			std::shared_ptr<FolderItem> extentionItem_ptr = std::make_shared<FolderItem>(folderItem);
			m_extentionsContainer[folderItem.getPath()] = extentionItem_ptr;
			return true;
		}
		return false;
	}


	std::shared_ptr<FolderItem> FolderFilterFile::find(const char* path)
	{
		std::string tmp = path;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
		auto it = m_extentionsContainer.find(tmp);
		bool found = (it != m_extentionsContainer.end());
		if (found)
		{
			std::shared_ptr<FolderItem> itemPtr(it->second);
			return itemPtr;
		}

		return nullptr;
	}

	//std::string ExtentionsFilterObject::m_extentionsFilePath;

	//std::unique_ptr<ExtentionsFilterFile> ExtentionsFilterObject::m_extentionsFile = nullptr;
	//bool ExtentionsFilterObject::m_once = true;
	//bool ExtentionsFilterObject::m_isError = false;
	//static ImageType defaultImageType;
	//ExtentionType ExtentionsFilterObject::defaultExtentionItem;

	
	/*
	bool FolderFilterObject::init()
	{
		
		if (m_once)
		{
			m_extentionsFile = std::make_unique<ExtentionsFilterFile>();
			std::string path = m_extentionsFilePath;
			if (SAUtils::FileExists(path.c_str()) == false)
			{
				m_isError = true;
				return false;
			}
			m_extentionsFile->read(path.c_str());
			m_once = false;
		}
		return true;
	}
	*/
	
	

	bool FolderFilterObject::getList(std::vector<std::shared_ptr<FolderItem>>& list)
	{
		return m_folderFile->getList(list);
	}

	bool FolderFilterObject::write()
	{
		std::string path = m_folderFilePath;
		if (SAUtils::FileExists(path.c_str()) == false)
		{
			m_isError = true;
			throw std::exception();
		}

		return m_folderFile->write(path.c_str());
	}
	
	bool FolderFilterObject::setFolderFilePath(const char* folderFilePath)
	{
		m_folderFilePath = folderFilePath;
		std::string path = m_folderFilePath;
		if (SAUtils::FileExists(path.c_str()) == false)
		{
			m_isError = true;
			return false;
		}
		return true;
	}

	

	
	std::shared_ptr<FolderItem> FolderFilterObject::find(const char* filename)
	{
		std::string ext = SAUtils::getExtention(filename);
		std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
		std::shared_ptr<FolderItem> item = m_folderFile->find(ext.c_str());
		return item;
	}

	bool FolderFilterObject::insert(FolderItem& extentionItem)
	{
		return m_folderFile->insert(extentionItem);
	}

	bool FolderFilterObject::update(FolderItem& folderItem)
	{
		return m_folderFile->edit(folderItem);
	}

	bool FolderFilterObject::remove(const char* ext)
	{
		return m_folderFile->remove(ext);
	}

	bool FolderFilterObject::isAllowed(const char* ext)
	{
		std::string tmp = ext;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
		std::shared_ptr<FolderItem> item = m_folderFile->find(ext);
		if (item == nullptr)
		{
			return false;
		}
		return true;
	}


	bool FolderFilterObject::findPath(const char* path)
	{
		/*
		std::shared_ptr<FolderItem> item = m_extentionsFile->find(path);
		if (item != nullptr)
		{
			ExtentionType type = item->getType();
			return type;
		}
		return defaultExtentionItem;
		*/
		return true;
	}

	

	
	
	bool FolderFilterObject::IsValidXML(const char* filename)
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

