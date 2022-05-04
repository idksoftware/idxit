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
#include "SACmdArgs.h"

#include "StandardOut.h"

#include "ImageExtentions.h"

#include "SAUtils.h"
#include "CLogger.h"

#include "FileInfo.h"

#include "HistoryEvent.h"

#include "HookCmd.h"

#include "SummaryFile.h"
#include "CIDKDate.h"
//#include "MirrorManager.h"

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
		static std::shared_ptr<FileFilter> m_fileFilter;
		static std::shared_ptr<FolderFilter> m_folderFilter;
		static bool m_fileFilterOn;
		static bool m_folderFilterOn;
	public:
		TestVisitor() = default;
		TestVisitor(const char* path) {
			if (m_xmlFileInfoWriter == nullptr) {
				m_xmlFileInfoWriter = std::make_shared<XMLFileInfoWriter>(path);
			}
			if (m_xmlFileInfoWriter == nullptr) {
				m_xmlFileInfoWriter = std::make_shared<XMLFileInfoWriter>(path);
			}
			if (m_fileFilter == nullptr) {
				m_fileFilter = std::make_shared<FileFilter>();
			}
			if (m_folderFilter == nullptr) {
				m_folderFilter = std::make_shared<FolderFilter>();
			}
		}

		void addFileFilter(const char* ext) {
			m_fileFilterOn = true;
			m_fileFilter->push_back(ext);
		}

		void addFolderFilter(const char* dir) {
			m_folderFilterOn = true;
			m_folderFilter->push_back(dir);
		}

		virtual bool onStart(const char* path)
		{
			//printf("Start: %s\n", path);

			m_path = path;
			return true;
		};

		bool matchFile(std::string p) {
			std::filesystem::path fullpath = p;
			if (m_fileFilter != nullptr) {
				std::string ext = fullpath.extension().string();
				return m_fileFilter->match(ext);
			}
			return true;
		}

		bool excludeFolder(std::string p) {
			std::filesystem::path fullpath = p;
			std::string folder = fullpath.filename().string();
			if (m_fileFilter != nullptr) {

				return m_folderFilter->match(folder);
			}
			return false;
		}

		virtual bool onFile(const char* path)
		{


			std::string p = path;
			//fileInfo.print();
			if (m_xmlFileInfoWriter != nullptr) {
				printf("Processing File: % s\n", path);
				if (m_fileFilterOn) {
					if (matchFile(p)) {

						FileInfo fileInfo(p);
						m_xmlFileInfoWriter->add(fileInfo);
					}
				}
				else {
					FileInfo fileInfo(p);
					m_xmlFileInfoWriter->add(fileInfo);
				}
			}
			return true;
		};

		virtual bool onDirectory(const char* path)
		{
			printf("Dir: %s\n", path);
			std::string p = path;
			//fileInfo.print();
			if (m_xmlFileInfoWriter != nullptr) {
				printf("Processing folder: % s\n", path);
				if (m_folderFilterOn) {
					if (excludeFolder(p)) {
						return true;
					}
				}
			}
			return false;
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
	std::shared_ptr<FileFilter> TestVisitor::m_fileFilter = nullptr;
	bool TestVisitor::m_fileFilterOn = false;
	std::shared_ptr<FolderFilter> TestVisitor::m_folderFilter = nullptr;
	bool TestVisitor::m_folderFilterOn = false;

	using namespace std::filesystem;


	int main(int argc, char* argv[]) {

		/*
			Storage::setPath("c://temp/vfs");
			std::string p = "c://temp//test.txt";
			FileInfo fileInfo(p);
			Storage storage;
			for (int i = 0; i < 0xFFFFFFFF; i++) {
				storage.add(fileInfo);
			}
		*/


		std::string distPath = "g:\\";
		//distPath += "local";
		std::shared_ptr<TestVisitor> testVisitor_ptr = std::make_shared<TestVisitor>("c:\\temp\\test\\gdrive.xml");
		//testVisitor_ptr->addFilter("*.*");
		//testVisitor_ptr->addFilter(".h");
		//testVisitor_ptr->addFilter(".mp4");
		testVisitor_ptr->addFolderFilter(".vs");
		testVisitor_ptr->addFolderFilter("games");
		//testVisitor_ptr->addFilter(".h");
		//testVisitor_ptr->addFilter(".mp4");
		DirectoryVisitor directoryVisitor(testVisitor_ptr);
		directoryVisitor.process(distPath.c_str());
		return 0;
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
