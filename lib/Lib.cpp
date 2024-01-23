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

//#include <fileapi.h>
#include <filesystem>
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


#include "RemoteServer.h"
#include <stdio.h>
#include <sstream>
#include "CDate.h"

#include "UpdateConfig.h"

#include <vector>
#include <filesystem>
	//#include <iterator_range>
#include <iostream>

#include "XMLWriter.h"
#include "IndexedCSV.h"
#include "FileInfo.h"
#include "IndexInfo.h"
#include "DirectoryVisitor.h"

#include "Storage.h"
#include "IgnoreList.h"
#include "DefinePaths.h"
#include "HomePaths.h"
#include "ScanVisitor.h"
#include "JounalVisitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#define VERSION	"1.00"
#define BUILD	"040115.1749"

#undef FILECODE
#define FILECODE IDXLib_CPP


	using namespace std::filesystem;

	bool IDXLib::prop(const char* sourcePath)
	{
		CLogger& logger = CLogger::getLogger();

		std::shared_ptr<JounalVisitor> jounalVisitor_ptr = std::make_shared<JounalVisitor>(sourcePath);

		DirectoryVisitor jounalDirectoryVisitor(jounalVisitor_ptr);
		jounalDirectoryVisitor.process(sourcePath);


		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Number of files to be processed: %d", jounalVisitor_ptr->getNoFiles());
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Number of folders to be processed: %d", jounalVisitor_ptr->getNoFolders());
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Total number of bytes to be processed: %llu", jounalVisitor_ptr->getSize());

		return true;
	}


	bool IDXLib::scan(const char* sourcePath, const char* idxPath, const char* ignoreFile, bool nousys, bool nouser, bool nosys, const char* incGroupFile, const char* excGroupFile)
	{
		CLogger& logger = CLogger::getLogger();

		std::shared_ptr<JounalVisitor> jounalVisitor_ptr = std::make_shared<JounalVisitor>(sourcePath);
		
		DirectoryVisitor jounalDirectoryVisitor(jounalVisitor_ptr);
		jounalDirectoryVisitor.process(sourcePath);
		
		
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Number of files to be processed: %d", jounalVisitor_ptr->getNoFiles());
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Number of folders to be processed: %d", jounalVisitor_ptr->getNoFolders());
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Total number of bytes to be processed: %llu", jounalVisitor_ptr->getSize());

		std::shared_ptr<ScanVisitor> scanVisitor_ptr = std::make_shared<ScanVisitor>(sourcePath, idxPath);

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
		std::filesystem::path p = idxPath;
		std::string idxPathRoot = p.parent_path().string();
		if (SAUtils::DirExists(idxPathRoot.c_str()) == false) {
	
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

			if (group.read(excGroupFile) == false) {
				return false;
			}
		}
		else {
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Exclude group file not enabled");
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



		std::vector<std::shared_ptr<GroupItem>>& list = group.getList();
		for (auto i : list) {
			std::shared_ptr<GroupItem> item = i;
			scanVisitor_ptr->addIncFileFilter(item->m_ext.c_str());
		}

		for (auto i : ignoreList) {
			std::shared_ptr<IgnorePath> item = i;
			scanVisitor_ptr->addSysIgnoreList(item);
		}

		IndexInfo& indexInfo = ScanVisitor::getIndexInfo();
		indexInfo.setFilesToBeCompleted(jounalVisitor_ptr->getNoFiles());
		indexInfo.setFoldersToBeCompleted(jounalVisitor_ptr->getNoFolders());
		indexInfo.setSizeToBeCompleted(jounalVisitor_ptr->getSize());

		
		CDate date = CDate::timeNow();

		char devicename[MAX_PATH + 1];
		DWORD dnsize = MAX_PATH;
		GetComputerName(devicename, &dnsize);
		indexInfo.setDevicename(devicename);
		char hostname[MAX_PATH + 1];
		gethostname(hostname, MAX_PATH);
		indexInfo.setHostname(hostname);

		DirectoryVisitor directoryVisitor(scanVisitor_ptr);
		scanVisitor_ptr->WriteHeader();
		directoryVisitor.process(sourcePath);

		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Number of files has been processed: %d", scanVisitor_ptr->getNoFiles());
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Number of folders has been processed: %d", scanVisitor_ptr->getNoFolders());
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Total number of bytes has been processed: %llu", scanVisitor_ptr->getSize());

		bool completed = true;
		if (jounalVisitor_ptr->getNoFiles() != scanVisitor_ptr->getNoFiles())
		{
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Number of files not processed: %d", jounalVisitor_ptr->getNoFiles() - scanVisitor_ptr->getNoFiles());
			completed = false;
		}
		if (jounalVisitor_ptr->getNoFolders() != scanVisitor_ptr->getNoFolders())
		{
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Number of Folders not processed: %d", jounalVisitor_ptr->getNoFolders() - scanVisitor_ptr->getNoFolders());
			completed = false;
		}
		if (jounalVisitor_ptr->getSize() != scanVisitor_ptr->getSize())
		{
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Number of bytes not processed: %d", jounalVisitor_ptr->getSize() - scanVisitor_ptr->getSize());
			completed = false;
		}

		if (completed == true) {
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Scan conmpeted Ok", jounalVisitor_ptr->getSize() - scanVisitor_ptr->getSize());
		}
		else {
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Scan conmpeted, not all files of folders processed?", jounalVisitor_ptr->getSize() - scanVisitor_ptr->getSize());
		}

		scanVisitor_ptr->WriteFooter();

		return true;
	}


	


/////////////////////////////////////////////////////////


	IDXLib::IDXLib() :
		
		m_winsockRequired(true),
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

	
