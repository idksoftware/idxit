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
//#include "DCRawArgs.h"
//#include "LUAScript.h"

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
