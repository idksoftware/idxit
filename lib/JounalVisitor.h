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
#include "DirectoryVisitor.h"
#include "Storage.h"
#include "IgnoreList.h"


class JounalVisitor : public FolderVisitor {
	//static BackupVisitor *m_this;
	static std::string m_sourcePath;
	
	static uint64_t m_files;
	static uint64_t m_folders;
	static uint64_t m_size;
public:
	JounalVisitor() = default;
	JounalVisitor(const char* sourcePath) {
		
		m_sourcePath = sourcePath;
		
	}

	


	virtual bool onStart(const char* path)
	{
		//printf("Start: %s\n", path);

		m_sourcePath = path;
		return true;
	};
	
	

	virtual bool onFile(const char* path)
	{
		CLogger& logger = CLogger::getLogger();
		uint64_t filesize = 0;
		std::string p = path;
		m_files++;

		
		filesize = SAUtils::fileSize(path);
		m_size += filesize;

		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Processing File: %s", path);
		return true;
	};

	virtual bool onDirectory(const char* path)
	{
		CLogger& logger = CLogger::getLogger();

		std::string p = path;
		m_folders++;
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Processing Folder: %s", path);

		return true;
	};

	virtual bool onEnd()
	{
		//printf("onEnd: %s\n", m_path.c_str());

		return true;
	};
	virtual std::shared_ptr<FolderVisitor> make() {
		return (std::make_shared<JounalVisitor>());
	}

	static uint64_t getNoFiles() { return m_files; };
	static uint64_t getNoFolders() { return m_folders; };
	static uint64_t getSize() { return m_size; };
	
};

