#pragma once

#include <filesystem>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <filesystem>

#include <iostream>
#include "CDate.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "CLogger.h"
#include "FileInfo.h"
#include "GroupFile.h"
#include "CIDKDate.h"
#include "IndexedCSV.h"
#include "XMLWriter.h"
#include "DirectoryVisitor.h"
#include "Storage.h"
#include "IgnoreList.h"
#include "Filters.h"
#include "IndexInfo.h"


class ScanVisitor : public FolderVisitor {
	//static BackupVisitor *m_this;
	
	static std::string m_sourcePath;
	//static std::string m_idxPath;
	static std::shared_ptr<XMLFileInfoWriter> m_xmlFileInfoWriter;
	static std::shared_ptr<ICSVFileInfoWriter> m_icsvFileInfoWriter;
	static std::shared_ptr<Storage> m_storage;
	static std::shared_ptr<FileFilter> m_incFileFilter;
	static std::shared_ptr<FolderFilter> m_incFolderFilter;
	static bool m_incFileFilterOn;
	static bool m_incFolderFilterOn;
	static std::shared_ptr<FileFilter> m_excFileFilter;
	static std::shared_ptr<FolderFilter> m_excFolderFilter;
	static bool m_excFileFilterOn;
	static bool m_excFolderFilterOn;
	static std::shared_ptr<IgnoreList> m_sysIgnoreList;
	static std::shared_ptr<IgnoreList> m_usersysIgnoreList;
	static std::shared_ptr<IgnoreList> m_userIgnoreList;
	static bool m_sysIgnoreOn;
	static bool m_usersysIgnoreOn;
	//static bool m_userIgnoreOn;
	static bool m_scanHidden;

	static uint64_t m_files;
	static uint64_t m_folders;
	static uint64_t m_size;

	static uint64_t m_included;
	static uint64_t m_excluded;
	
	static IndexInfo m_indexInfo;
public:
	ScanVisitor() = default;
	ScanVisitor(const char* sourcePath, const char* idxPath) {
		//if (m_xmlFileInfoWriter == nullptr) {
		//	m_xmlFileInfoWriter = std::make_shared<XMLFileInfoWriter>(path);
		//}
		m_indexInfo.setSourcePath(sourcePath);
		m_indexInfo.setIdxPath(idxPath);

		if (m_icsvFileInfoWriter == nullptr) {
			m_icsvFileInfoWriter = std::make_shared<ICSVFileInfoWriter>(idxPath);
		}
		if (m_excFileFilter == nullptr) {
			m_excFileFilter = std::make_shared<FileFilter>();
			//m_indexInfo.setExcFileFilter(m_excFileFilter);
		}
		if (m_incFolderFilter == nullptr) {
			m_incFolderFilter = std::make_shared<FolderFilter>();
		}
		if (m_incFileFilter == nullptr) {
			m_incFileFilter = std::make_shared<FileFilter>();
		}
		if (m_incFolderFilter == nullptr) {
			m_incFolderFilter = std::make_shared<FolderFilter>();
		}
		if (m_sysIgnoreList == nullptr) {
			m_sysIgnoreList = std::make_shared<IgnoreList>();
		}
		if (m_usersysIgnoreList == nullptr) {
			m_usersysIgnoreList = std::make_shared<IgnoreList>();
		}
		if (m_userIgnoreList == nullptr) {
			m_userIgnoreList = std::make_shared<IgnoreList>();
		}
	}

	static IndexInfo& getIndexInfo() { return m_indexInfo; };

	void addIncFileFilter(const char* ext) {
		m_incFileFilterOn = true;
		m_incFileFilter->push_back(ext);
	}

	void addIncFolderFilter(const char* dir) {
		m_incFolderFilterOn = true;
		m_incFolderFilter->push_back(dir);
	}

	void addExcFileFilter(const char* ext) {
		m_excFileFilterOn = true;
		m_excFileFilter->push_back(ext);
	}

	void addExcFolderFilter(const char* dir) {
		m_excFolderFilterOn = true;
		m_excFolderFilter->push_back(dir);
	}

	void addSysIgnoreList(std::shared_ptr<IgnorePath> ip) {
		m_sysIgnoreOn = true;
		std::shared_ptr<IgnorePath> ipp = std::make_shared<IgnorePath>(*ip);
		m_sysIgnoreList->push_back(ipp);
	}

	void addUserSysIgnoreList(IgnorePath& ip) {
		m_usersysIgnoreOn = true;
		std::shared_ptr<IgnorePath> ipp = std::make_shared<IgnorePath>(ip);
		m_usersysIgnoreList->push_back(ipp);
	}


	virtual bool onStart(const char* path)
	{
		//printf("Start: %s\n", path);

		m_sourcePath = path;
		return true;
	};

	bool matchIncFile(std::string p) {
		std::filesystem::path fullpath = p;
		if (m_incFileFilter != nullptr) {
			std::string ext = fullpath.extension().string();
			size_t len = ext.length();
			if (len == 0) {
				return false;
			}
			std::string noDotExt;
			if (ext[0] == '.') {
				noDotExt = ext.substr(1, ext.length() - 1);
			}
			else {
				noDotExt = ext;
			}
			return m_incFileFilter->match(noDotExt);
		}
		return true;
	}

	bool excludeFolder(std::string p) {
		std::filesystem::path fullpath = p;
		std::string folder = fullpath.filename().string();
		if (m_excFileFilter != nullptr) {

			return m_excFolderFilter->match(folder);
		}
		return false;
	}

	virtual bool onFile(const char* path)
	{
		CLogger& logger = CLogger::getLogger();

		std::string p = path;
		m_files++;


		uint64_t filesize = SAUtils::fileSize(path);
		m_size += filesize;

		//fileInfo.print();
		if (m_icsvFileInfoWriter != nullptr) {
			if (SAUtils::isHidden(path)) {
				if (m_scanHidden == false) {
					logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Found hidden File: %s Hidden files ignored", path);
					logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Processing File: %s - File was excluded", path);
					m_excluded++;
					return false;
				}
				logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Found hidden File: %s Hidden files to be included", path);
			}
			if (m_incFileFilterOn) {
				if (matchIncFile(p)) {
					logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "File filter included: %s", path);
				}
				else {
					logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "File filter excluded:: %s", path);
					logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Processing File: %s - File was excluded", path);
					m_excluded++;
					return false;
				}
			}
			else {
				logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "No file filter applied including: %s", path);
			}
		}
		FileInfo fileInfo(p);
		//m_xmlFileInfoWriter->add(fileInfo);
		m_icsvFileInfoWriter->add(fileInfo);

		m_included++;
		

		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Processing File: %s - File was included", path);
		return true;
	};

	virtual bool onDirectory(const char* path)
	{
		CLogger& logger = CLogger::getLogger();

		std::string p = path;
		m_folders++;
		//fileInfo.print();
		if (m_icsvFileInfoWriter != nullptr) {
			if (SAUtils::isHidden(path)) {
				if (m_scanHidden == false) {
					logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Found hidden File: %s Hidden files ignored", path);
					return false;
				}
				logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Found hidden File: %s Hidden files to be included", path);
			}
			if (m_excFolderFilterOn) {

				if (excludeFolder(p)) {
					logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Processing Folder: %s - Folder was included", path);
					return true;
				}
				else {
					logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Processing Folder: %s - Folder was included", path);
				}
			}
		}


		return true;
	};

	virtual bool onEnd()
	{
		//printf("onEnd: %s\n", m_path.c_str());

		return true;
	};
	virtual std::shared_ptr<FolderVisitor> make() {
		return (std::make_shared<ScanVisitor>());
	}
	static bool VolumeInformation(std::ofstream& file);
	static bool WriteHeader();
	static bool WriteFooter();

	static uint64_t getNoFiles() { return m_files; };
	static uint64_t getNoFolders() { return m_folders; };
	static uint64_t getSize() { return m_size; };

	static uint64_t getNoIncluded() { return m_included; };
	static uint64_t getNoExcluded() { return m_excluded; };
};

