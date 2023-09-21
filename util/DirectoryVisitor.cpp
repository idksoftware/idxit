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

#include <string>
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <sstream>

#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>

#if defined  _WIN64 || defined WIN32
	//#include <sysstat.h>
#else
	#include <dirent.h>
	#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
//#include "CIDKFileFind.h"
#include <filesystem>
#include "DirectoryVisitor.h"
#include "SAUtils.h"
#include "CLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif






/**
 * This class is the directory node that is used to
 * create a folder path that the DirectoryVisitor uses
 * to navigate the directory 
 */
class DirNode {
	std::string m_dirpath;
	std::shared_ptr<DirNode> m_dirNode;
	std::shared_ptr<FolderVisitor> m_folderVisitor;
	
public:
	DirNode(std::shared_ptr<DirNode> dirNode, const char *dirpath) {
		m_dirNode = dirNode;
		m_dirpath = dirpath;
		m_folderVisitor = nullptr;
		printf("Create \"%s\"\n", m_dirpath.c_str());
	}
	
	DirNode(std::shared_ptr<DirNode> dirNode, const char *dirpath, std::shared_ptr<FolderVisitor> folderVisitor) {
		m_dirNode = dirNode;
		m_dirpath = dirpath;
		m_folderVisitor = folderVisitor;
		m_folderVisitor->onStart(dirpath);
		printf("Create \"%s\"\n", m_dirpath.c_str());
	}
	DirNode(const DirNode& x) {
	}
	
	~DirNode() {
		//CLogger& logger = CLogger::getLogger();
		printf("Delete \"%s\"\n", m_dirpath.c_str());
	}

	bool process();
};



DirectoryVisitor::DirectoryVisitor(std::shared_ptr<FolderVisitor> folderVisitor, bool val) {

	m_dirNode = nullptr;
	m_folderVisitor = folderVisitor;
	m_deleteFolderVisitor = val;

}

DirectoryVisitor::~DirectoryVisitor() {
	
}


bool DirectoryVisitor::process(const char *rf) {
	std::string rootFolder = rf;
	if (m_folderVisitor != 0) {
		m_dirNode = std::make_shared<DirNode>(nullptr, rootFolder.c_str(), m_folderVisitor->make());
	} else {
		m_dirNode = std::make_shared<DirNode>(nullptr, rootFolder.c_str());
	}
	m_dirNode->process();
	return true;
}

bool DirNode::process()
{
	CLogger& logger = CLogger::getLogger();

	std::string path = m_dirpath;

	try {
		for (auto const& dir_entry : std::filesystem::directory_iterator{ path })
		{
			std::filesystem::path p = dir_entry.path();
			std::string filename = p.string();

			if (std::filesystem::is_regular_file(p) == true) {
				if (m_folderVisitor) {
					if (m_folderVisitor->onFile(filename.c_str()) == true) {
						logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Processing File: %s - File was included", path.c_str());
					}
					else {
						logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Processing File: %s - File was excluded", path.c_str());
					}
				}
			}
			else {
				if (m_folderVisitor) {
					if (m_folderVisitor->onDirectory(filename.c_str()) == false) {
						continue;
					}
				}
				if (m_folderVisitor != 0) {
					m_dirNode = std::make_shared<DirNode>(nullptr, filename.c_str(), m_folderVisitor->make());
				}
				else {
					m_dirNode = std::make_shared<DirNode>(nullptr, filename.c_str());
				}
				m_dirNode->process();
			}

		}
	}
	catch (std::exception& ex) {
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Unable to access \"%s\" folder", path.c_str());
	}
	return true;
}


