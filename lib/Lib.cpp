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
**	Filename	: FileInfo.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include "Lib.h"
#include "AppConfig.h"
//#include "SIAArcAppOptions.h"
//#include "SACmdArgs.h"

#include "StandardOut.h"


#include "SAUtils.h"
#include "CLogger.h"

#include "FileInfo.h"

#include "HistoryEvent.h"

#include "HookCmd.h"
#include "GroupFile.h"

#include "SummaryFile.h"
#include "CIDKDate.h"


#include "UDPOut.h"

#include "TerminalServer.h"
#include "ArchivePath.h"

#include "RemoteServer.h"
#include <stdio.h>
#include <sstream>
#include "WorkspaceFiles.h"

#include "UpdateConfig.h"

#include <vector>
#include <filesystem>
	//#include <iterator_range>
#include <iostream>

#include "XMLWriter.h"
#include "FileInfo.h"
#include "DirectoryVisitor.h"

#include "Storage.h"
#include "IgnoreList.h"
#include "DefinePaths.h"
#include "HomePaths.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#define VERSION	"1.00"
#define BUILD	"040115.1749"

#undef FILECODE
#define FILECODE IDXLib_CPP

//int pythonmain();

namespace simplearchive {
	



	class FileFilter : public std::vector<std::string> {
	public:
		bool match(std::string& ext) {
			if (ext.empty()) {
				return false;
			}
			for (auto i = begin(); i != end(); i++) {
				if (i->compare(ext) == 0) {
					return true;
				}
			}
			return false;
		}

	};

	class FolderFilter : public std::vector<std::string> {
	public:
		bool match(std::string& folder) {
			for (auto i = begin(); i != end(); i++) {
				if (i->compare(folder) == 0) {
					return true;
				}
			}
			return false;
		}

	};

	class TestVisitor : public FolderVisitor {
		//static BackupVisitor *m_this;
		std::string m_path;
		static std::shared_ptr<XMLFileInfoWriter> m_xmlFileInfoWriter;
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
		static bool m_userIgnoreOn;
		static bool m_scanHidden;
	public:
		TestVisitor() = default;
		TestVisitor(const char* path) {
			if (m_xmlFileInfoWriter == nullptr) {
				m_xmlFileInfoWriter = std::make_shared<XMLFileInfoWriter>(path);
			}
			if (m_xmlFileInfoWriter == nullptr) {
				m_xmlFileInfoWriter = std::make_shared<XMLFileInfoWriter>(path);
			}
			if (m_excFileFilter == nullptr) {
				m_excFileFilter = std::make_shared<FileFilter>();
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

		void addSysIgnoreList(std::shared_ptr<IgnorePattern> ip) {
			m_sysIgnoreOn = true;
			std::shared_ptr<IgnorePattern> ipp = std::make_shared<IgnorePattern>(*ip);
			m_sysIgnoreList->push_back(ipp);
		}

		void addUserSysIgnoreList(IgnorePattern& ip) {
			m_usersysIgnoreOn = true;
			std::shared_ptr<IgnorePattern> ipp = std::make_shared<IgnorePattern>(ip);
			m_usersysIgnoreList->push_back(ipp);
		}


		virtual bool onStart(const char* path)
		{
			//printf("Start: %s\n", path);

			m_path = path;
			return true;
		};

		bool matchIncFile(std::string p) {
			std::filesystem::path fullpath = p;
			if (m_incFileFilter != nullptr) {
				std::string ext = fullpath.extension().string();
				if (ext.empty()) {
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
			//fileInfo.print();
			if (m_xmlFileInfoWriter != nullptr) {
				if (SAUtils::isHidden(path)) {
					if (m_scanHidden == false) {
						logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Found hidden File: %s Hidden files ignored", path);
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
						return false;
					}
				}
				else {
					logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "No file filter applied including: %s", path);
				}
			}
			FileInfo fileInfo(p);
			m_xmlFileInfoWriter->add(fileInfo);
			return true;
		};

		virtual bool onDirectory(const char* path)
		{
			CLogger& logger = CLogger::getLogger();
			
			std::string p = path;
			//fileInfo.print();
			if (m_xmlFileInfoWriter != nullptr) {
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
			return (std::make_shared<TestVisitor>());
		}
	};

	std::shared_ptr<XMLFileInfoWriter> TestVisitor::m_xmlFileInfoWriter = nullptr;
	std::shared_ptr<Storage> TestVisitor::m_storage = nullptr;
	std::shared_ptr<FileFilter> TestVisitor::m_incFileFilter = nullptr;
	bool TestVisitor::m_incFileFilterOn = false;
	std::shared_ptr<FolderFilter> TestVisitor::m_incFolderFilter = nullptr;
	bool TestVisitor::m_incFolderFilterOn = false;
	std::shared_ptr<FileFilter> TestVisitor::m_excFileFilter = nullptr;
	bool TestVisitor::m_excFileFilterOn = false;
	std::shared_ptr<FolderFilter> TestVisitor::m_excFolderFilter = nullptr;
	bool TestVisitor::m_excFolderFilterOn = false;
	std::shared_ptr<IgnoreList> TestVisitor::m_sysIgnoreList = nullptr;
	bool TestVisitor::m_sysIgnoreOn = true;
	std::shared_ptr<IgnoreList> TestVisitor::m_usersysIgnoreList = nullptr;
	bool TestVisitor::m_usersysIgnoreOn = true;
	std::shared_ptr<IgnoreList> TestVisitor::m_userIgnoreList = nullptr;
	bool TestVisitor::m_scanHidden = false;

	using namespace std::filesystem;


	bool IDXLib::scan(const char* sourcePath, const char* idxPath, const char* ignoreFile, bool nousys, bool nouser, bool nosys, const char* incGroupFile, const char* excGroupFile)
	{
		CLogger& logger = CLogger::getLogger();

		if (sourcePath == nullptr) {
			logger.log(LOG_COMPLETED, CLogger::Level::FATAL, "No source path found");
			return false;
		}
		if (SAUtils::DirExists(sourcePath) == false) {
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Source path not found: %s", sourcePath);
			return false;
		}
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Source path found: %s", sourcePath);

		if (idxPath == nullptr) {
			logger.log(LOG_COMPLETED, CLogger::Level::FATAL, "No Index output file found");
			return false;
		}
		if (SAUtils::FileExists(idxPath) == false) {
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Index output file not found: %s", idxPath);
			return false;
		}
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Index output file found: %s", idxPath);

		IgnoreList ignoreList;
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "No system ignore {nosys) set: %s", (nosys) ? "True - file will not be read" : "False - file will be read");
		if (nosys == false) {
			std::string homePath = IdxItHome::getIdxItHome();
			std::string filterPath = homePath;
			filterPath += FILTERS_PATH;
			filterPath += "/sys.ign";
			ignoreList.read(filterPath.c_str());
			// read system ignore file
			//logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Reading system ignore file: %s", (nosys)?"True":"False");
		}

		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "No user system ignore (nousys): %s", (nousys) ? "True - file will not be read" : "False - file will be read");
		if (nousys == false) {
			// read system ignore file
			//logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "No user system ignore: %s", (nousys) ? "True" : "False");
		}

		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "No user scanning (nousys): %s", (nousys) ? "True - user space will not be scanned" : "False - user space will be scanned");
		if (nouser == false) {
			// read system ignore file
			//logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "No user scanning: %s", (nousys) ? "True" : "False");
		}

		if (ignoreFile != nullptr) {
		
			if (SAUtils::FileExists(ignoreFile) == false) {
				logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Ignore file path not found: %s", ignoreFile);
				return false;
			}
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Ignore file found: %s", ignoreFile);
		}
		else {
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Ignore file not  enabled");
		}
		GroupFile group;
		if (incGroupFile != nullptr) {

			if (SAUtils::FileExists(incGroupFile) == false) {
				logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Include group file path not found: %s", incGroupFile);
				return false;
			}
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Include group file found: %s", incGroupFile);
			
			if (group.read(incGroupFile) == false) {
				return false;
			}
			
		}
		else {
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Include group file not enabled");
		}

		

		if (excGroupFile != nullptr) {

			if (SAUtils::FileExists(excGroupFile) == false) {
				logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Exclude group file path not found: %s", excGroupFile);
				return false;
			}
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Exclude group file found: %s", excGroupFile);
		}
		else {
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Exclude group file not enabled");

			if (group.read(excGroupFile) == false) {
				return false;
			}
		}
		
		
		//bool nousys, bool nouser, bool nosys,


		
		//std::string ignoreFileString = ignoreFile;
		//std::string incGroupFileString = incGroupFile;
		//std::string excGroupFileString = excGroupFile;
		/*
			Storage::setPath("c://temp/vfs");
			std::string p = "c://temp//test.txt";
			FileInfo fileInfo(p);
			Storage storage;
			for (int i = 0; i < 0xFFFFFFFF; i++) {
				storage.add(fileInfo);
			}
		*/
		
		std::shared_ptr<TestVisitor> testVisitor_ptr = std::make_shared<TestVisitor>(idxPath);

		std::vector<std::shared_ptr<GroupItem>>& list = group.getList();
		for (auto i : list) {
			std::shared_ptr<GroupItem> item = i;
			testVisitor_ptr->addIncFileFilter(item->m_ext.c_str());
		}

		for (auto i : ignoreList) {
			std::shared_ptr<IgnorePattern> item = i;
			testVisitor_ptr->addSysIgnoreList(item);
		}
		
		DirectoryVisitor directoryVisitor(testVisitor_ptr);
		directoryVisitor.process(sourcePath);
		return true;
	}


	


/////////////////////////////////////////////////////////


	IDXLib::IDXLib() :
		
		m_winsockRequired(false),
		m_socklibStarted(false),
		m_enableEvents(false) 
	{}


	IDXLib::~IDXLib()
	{
		if (m_socklibStarted) {
#ifdef _WIN32
			WSACleanup();
#endif
		}
	}

	


	void  IDXLib::enableEvents(const char *address, int port) {
		m_udpPortNum = port;
		m_udpAddress = address;
		m_enableEvents = true;
		m_winsockRequired = true;
	}

	void  IDXLib::enableServer(int port) {
		m_tcpPortNum = port;
		m_enableServer = true;
		m_winsockRequired = true;
	}

	const int IDXLib::getLastCode() {
		return CLogger::getLastCode();
	}
	const char *IDXLib::getLastMessage() {
		return CLogger::getLastMessage();
	}


	int IDXLib::initalise(const char *appName) {
		
		AppConfig &config = AppConfig::get();
		//ImagePath::initPaths();
		std::string appPerName = appName;
		appPerName += "-";
		CLogger::setAppName(appPerName);
		CLogger::setLogLevel(config.getLogLevel());
		CLogger::setConsoleLevel(config.getConsoleLevel());
		CLogger::setSilent(config.isSilent());
		CLogger::setLogPath(config.getLogPath());
		try {
			CLogger::startLogging();
		}
		catch (std::exception e) {
			printf("Failed to start logging");
			return -1;
		}
		

		CLogger &logger = CLogger::getLogger();

#ifdef _WIN32
		WSADATA wsa;
		if (m_winsockRequired) {
			//Initialise winsock
			if (m_socklibStarted == false) {
				
				if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
				{
					//printf("Failed. Error Code : %d", WSAGetLastError());
					return false;
				}
				
				m_socklibStarted = true;
			}
		}
#endif
		

		m_enableEvents = config.isEventsOn();
		if (m_enableEvents == true && m_socklibStarted == true) {
			m_udpPortNum = config.eventPort();
			m_udpAddress = config.eventAddress();
			if (UDPOut::enableUDPOutput(m_udpPortNum, m_udpAddress.c_str()) == false) {
				return false;
			}
		}


		m_enableServer = config.isServerOn();
		if (m_enableServer == true && m_socklibStarted == true) {
			if (UDPOut::enableUDPOutput(m_udpPortNum, m_udpAddress.c_str()) == false) {
				return false;
			}
		}

		CIDKDate date;

		date.Now();


		//logger.log(LOG_STARTING, CLogger::Level::SUMMARY, "Application starting at %s", date.Print().c_str());
		logger.log(LOG_INITALISATION, CLogger::Level::FINE, "Home path is \"%s\"", config.getHomePath());

		
		

		try {



		}
		catch (SIAException e) {
			logger.log(LOG_INITALISATION, CLogger::Level::FATAL, "Failed to complete initalisation %s\n", e.what());
			return -1;
		}
		logger.log(LOG_INITALISATION, CLogger::Level::INFO, "Initalisation complete");
		return 0;
	}

	void IDXLib::start() {
		AppConfig& config = AppConfig::get();
		SummaryFile::setPath(config.getHistoryPath());
		SummaryFile& summaryFile = SummaryFile::getSummaryFile();
		CIDKDate date;
		date.Now();
		summaryFile.log(SummaryFile::SF_BRIEF, SummaryFile::SF_COMMENT, "Summary start %s", date.Print().c_str());
	}
	
	

	
	int IDXLib::complete() {
		CLogger &logger = CLogger::getLogger();
		SummaryFile &summaryFile = SummaryFile::getSummaryFile();
		CIDKDate date;
		date.Now();
		logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Application completed successfully at %s\n", date.Print().c_str());
		summaryFile.log(SummaryFile::SF_BRIEF, SummaryFile::SF_COMMENT, "Summary start");
		return 0;
	}

	
	

	
} // simplearchive
