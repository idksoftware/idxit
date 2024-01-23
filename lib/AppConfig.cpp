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

#include <sstream>
#include <iomanip>
#include "AppConfig.h"
#include "SAUtils.h"
#include "DefinePaths.h"

#include "CLogger.h"
#include "HomePaths.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#undef FILECODE
#define FILECODE APPCONFIG_CPP

#ifdef WIN32
#define STRICMP _stricmp
#else
#include <strings.h>
#define STRICMP strcasecmp
#endif



	std::unique_ptr<AppConfig> m_this = nullptr;
	
	bool AppConfig::m_verbose = false;
	bool AppConfig::m_quiet = true;
	bool AppConfig::m_silent = false;
	std::string AppConfig::m_logLevel = "SUMMARY";
	std::string AppConfig::m_consoleLevel;
	bool AppConfig::m_dry_run = false;
	
	bool AppConfig::m_serverMode = false;
	

	bool AppConfig::m_eventsOn = false; // UDP events
	bool AppConfig::m_serverOn = false;

	int AppConfig::m_tcpPortNum = 1626;
	int AppConfig::m_udpPortNum = 560;
	std::string AppConfig::m_udpAddress = "127.0.0.1";

	
	std::string AppConfig::m_configPath;
	
	std::string AppConfig::m_tempPath;
	std::string AppConfig::m_logPath;
	std::string AppConfig::m_homePath;
	std::string AppConfig::m_systemPath;
	std::string AppConfig::m_indexPath;
	std::string AppConfig::m_historyPath;	

	std::string AppConfig::m_ExternalExifTool;
	std::string AppConfig::m_ExternalCommandLine;

	/*
		Default paths based on UserDrive and UserHome

	*/
	

	void AppConfig::settup() {
		getTempPath();
		
		
		getLogPath();
		getIndexPath();
		getHistoryPath();
		getConfigPath();
	    getHomePath();
		isDryRun();
		isQuiet();
		isVerbose();
		getLogLevel();
	}


	
	void SharedConfig::fileBasedValues(const char *home, const char *tmpPath) {
		CLogger &logger = CLogger::getLogger();
		logger.log(LOG_OK, CLogger::Level::INFO, "Reading configuration file ");
		// Home Path (The path to this file will be based on the home path)
		std::string homePath = home;
		setHomePath(home);
		
	// General	
		std::string quiet = "false";
		setGeneral(QUIET_LABEL, quiet, quiet);
		AppConfig::m_quiet = (STRICMP(quiet.c_str(), "true") == 0);

		std::string silent = "false";
		setGeneral(SILENT_LABEL, silent, silent);
		AppConfig::m_silent = (STRICMP(silent.c_str(), "true") == 0);

		std::string dry_run = "false";
		setGeneral(DRY_RUN_LABEL, dry_run, dry_run);
		AppConfig::m_dry_run = (STRICMP(dry_run.c_str(), "true") == 0);

		std::string logLevel = "STATUS";
		setLogging(LOG_LEVEL_LABEL, logLevel, logLevel);
		AppConfig::m_logLevel = logLevel;

		std::string consoleLevel = "STATUS";
		setLogging(CONSOLE_LEVEL_LABEL, consoleLevel, consoleLevel);
		AppConfig::m_consoleLevel = consoleLevel;

		

		std::string serverMode = "false";
		setGeneral(REMOTE_SERVER_MODE_LABEL, serverMode, serverMode);
		AppConfig::m_serverMode = (STRICMP(serverMode.c_str(), "true") == 0);
		
		
	// System Folders
		// Master Archive Path
		//std::shared_ptr<ConfigBlock> folders = getSystemFolders();
		//if (folders == nullptr || getRoot().value("MasterPath", AppConfig::m_masterPath) == true) {
		//	ArchivePath::setMasterPath(AppConfig::m_masterPath);
		//}
		//
//#define CONFIG_PATH_LABEL				"ConfigPath"	 
//#define TOOLS_PATH_LABEL           		"ToolsPath"

		std::string tmpStr = tmpPath;
		std::string defauleValue = homePath + TEMP_PATH;
		if (tmpStr.empty() == false) {
			defauleValue = tmpStr;
		}
		setSystemFolders(TEMP_PATH_LABEL, AppConfig::m_tempPath, defauleValue);


		defauleValue = homePath + SYSTEM_PATH;
		setSystemFolders(SYSTEM_PATH_LABEL, AppConfig::m_systemPath, defauleValue);

		defauleValue = homePath + LOG_PATH;
		setSystemFolders(LOG_PATH_LABEL, AppConfig::m_logPath, defauleValue);

		
		defauleValue = homePath + HISTORY_PATH;
		setSystemFolders(HISTORY_PATH_LABEL, AppConfig::m_historyPath, defauleValue);
		
		
	
	
	// Network
		std::string eventsOn = AppConfig::m_eventsOn ? "true" : "false";
		setNetwork(EVENTS_ENABLED_LABEL, eventsOn, eventsOn);
		AppConfig::m_eventsOn = (STRICMP(eventsOn.c_str(), "true") == 0);
		
		std::string eventsPort = std::to_string(AppConfig::m_udpPortNum);
		setNetwork(EVENTS_PORT_LABEL, eventsPort, eventsPort);
		AppConfig::m_udpPortNum = std::stoi(eventsPort);

		std::string  udpAddress = AppConfig::m_udpAddress;
		setNetwork(EVENTS_ADDRESS_LABEL, AppConfig::m_udpAddress, udpAddress);

		std::string commandsOn = AppConfig::m_serverOn ? "true" : "false";
		setNetwork(EVENTS_ENABLED_LABEL, commandsOn, commandsOn);
		AppConfig::m_eventsOn = (STRICMP(commandsOn.c_str(), "true") == 0);
		
		std::string tcpPortNum = std::to_string(AppConfig::m_tcpPortNum);
		setNetwork(COMMANDS_PORT_LABEL, tcpPortNum, tcpPortNum);
		AppConfig::m_tcpPortNum = std::stoi(tcpPortNum);


		
		

		logger.log(LOG_OK, CLogger::Level::INFO, "    General");
		
		logger.log(LOG_OK, CLogger::Level::INFO, "        Verbose:                    %s", (AppConfig::m_verbose) ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Quiet:                      %s", (AppConfig::m_quiet) ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Silent:                     %s", (AppConfig::m_silent) ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Dry run enabled:            %s", (AppConfig::m_dry_run) ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Log level:                  %s", AppConfig::m_logLevel.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Console level:              %s", AppConfig::m_consoleLevel.c_str());
	
		logger.log(LOG_OK, CLogger::Level::INFO, "        m_eventsOn:                 %s", (AppConfig::m_eventsOn) ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "        m_serverOn:                 %s", (AppConfig::m_serverOn) ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "    System paths");
		logger.log(LOG_OK, CLogger::Level::INFO, "        System path:               \"%s\"", AppConfig::m_systemPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Log path:                  \"%s\"", AppConfig::m_logPath.c_str());
	
		logger.log(LOG_OK, CLogger::Level::INFO, "        History path:              \"%s\"", AppConfig::m_historyPath.c_str());
		
		logger.log(LOG_OK, CLogger::Level::INFO, "        Exif Tool:                 \"%s\"", AppConfig::m_ExternalExifTool.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Exif command line:         \"%s\"", AppConfig::m_ExternalCommandLine.c_str());
		
		
	}


	void SharedConfig::setHomePath(const char *homePath) {
		AppConfig::m_homePath = homePath;
		
		AppConfig::m_configPath = homePath;
		AppConfig::m_configPath += CONFIG_PATH;
	}
	
	const char *AppConfig::getHomePath() {
		
		return m_homePath.c_str();

	}

	
	

	void SharedConfig::setLogLevel(const char *logLevel) {
		AppConfig::m_logLevel = logLevel;
	}

	void SharedConfig::setConsoleLevel(const char *logLevel) {
		AppConfig::m_consoleLevel = logLevel;
	}

	void SharedConfig::setDryRun(bool dryRun) {
		AppConfig::m_dry_run = dryRun;
	}

	void SharedConfig::setQuiet(bool quiet) {
		AppConfig::m_quiet = quiet;
	}

	

	void SharedConfig::setSilent(bool silent) {
		AppConfig::m_silent = silent;
	}
	void SharedConfig::setVerbose(bool verbose) {
		AppConfig::m_verbose = verbose;
	}

	void SharedConfig::setEventsOn(bool evt) {
		AppConfig::m_eventsOn = evt;
	}

	void SharedConfig::setEventPort(int port) {
		AppConfig::m_eventsOn = true;
		AppConfig::m_udpPortNum = port;
	}

	void SharedConfig::setEventAddress(const char *address) {
		AppConfig::m_eventsOn = true;
		AppConfig::m_udpAddress = address;
	}

	

	const char *AppConfig::getTempPath() {
		return m_tempPath.c_str();
	}

	const char *AppConfig::getLogPath() {
		return m_logPath.c_str();
	}

	const char *AppConfig::getLogLevel() {
		return m_logLevel.c_str();
	}

	const char *AppConfig::getConsoleLevel() {
		return m_consoleLevel.c_str();
	}

	
	
	const char *AppConfig::getSystemPath() {
		
		return m_systemPath.c_str();
	}


	const char *AppConfig::getIndexPath() {	
		//m_indexPath = m_systemPath + "/index";
		return m_indexPath.c_str();
	}

	const char *AppConfig::getHistoryPath() {
		
		return 	m_historyPath.c_str();
	}

	const char *AppConfig::getConfigPath() {
		
		return 	m_configPath.c_str();
	}

	

	bool AppConfig::isDryRun() const {
		return m_dry_run;
	}

	bool AppConfig::isQuiet() const {
		return m_quiet;
	}

	bool AppConfig::isSilent() const {
		return m_silent;
	}
	
	bool AppConfig::isVerbose() const {
		return m_verbose;
	}

	

	bool AppConfig::isEventsOn() {
		return m_eventsOn;
	}

	int AppConfig::eventPort() {
		return m_udpPortNum;

	}
	const char *AppConfig::eventAddress() {
		return m_udpAddress.c_str();
	}

	bool AppConfig::isServerOn() {
		return m_serverOn;
	}

	int AppConfig::serverPort() {
		return m_tcpPortNum;

	}



	void AppConfig::isServerOn(bool on) {
		m_serverOn = on;
	}

	void AppConfig::setServerPort(int port) {
		m_tcpPortNum = port;
	}

	//void setSourcePath(const char *sourcePath);


	bool AppConfig::validHomePath() {
		return true;
	}

	std::string AppConfig::toString() {
		/// @brief Gets the source path.
		///const char *getSourcePath();

		/// @brief Gets the archive path.
		/// user definable
		std::stringstream str;
		str << "Configuration" << '\n';
		str << "=============" << '\n';
	
		/// @brief Gets the temp file path.
		/// user definable
		str << "Temp path:               " << getTempPath() << '\n';
		
		/// @brief Gets the hooks path
		/// user definable
		/// 
		
	
		/// @brief Gets log file path
		str << "Log path:                " << getLogPath() << '\n';
		/// @brief Gets the path to the crc index database.
		str << "Index path:              " << getIndexPath() << '\n';
		/// @brief Gets the path the history file are stored. These files are used for
		/// @brief the history of the changes made to the archive.
		str << "History path:            " << getHistoryPath() << '\n';
		/// @brief gets external Command line
	
		str << "Config path:             " << getConfigPath() << '\n';
		/// @brief Gets home path. This is the root path all default paths are made.
		str << "Home path:               " << getHomePath() << '\n';
		
		str << "Is dryrun:               " << (isDryRun()?"true":"false") << '\n';
		str << "Is quiet:                " << (isQuiet() ? "true" : "false") << '\n';
		str << "Is verbose:              " << (isVerbose() ? "true" : "false") << '\n';
		str << "Log level:               " << getLogLevel() << '\n';

		return str.str();
	}

	std::string AppConfig::toXMLString() {
		/// @brief Gets the source path.
		///const char *getSourcePath();

		/// @brief Gets the archive path.
		/// user definable
		std::stringstream str;
		str << "<Configuration>" << '\n';
		
		/// @brief Gets the temp file path.
		/// user definable
		str << "<TempPath>" << getTempPath() << "</TempPath>" << '\n';
		
		/// @brief Gets the hooks path
		/// user definable
		/// 
		
		
		/// @brief Gets log file path
		str << "<LogPath>" << getLogPath() << "</LogPath>" << '\n';
		/// @brief Gets the path to the crc index database.
		str << "<IndexPath>" << getIndexPath() << "</IndexPath>" << '\n';
		/// @brief Gets the path the history file are stored. These files are used for
		/// @brief the history of the changes made to the archive.
		str << "<HistoryPath>" << getHistoryPath() << "</HistoryPath>" << '\n';
		/// @brief gets external Command line
		
		str << "<IsDryRun>" << (isDryRun() ? "true" : "false") << "</IsDryRun>" << '\n';
		str << "<IsQuiet>" << (isQuiet() ? "true" : "false") << "</IsQuiet>" << '\n';
		str << "<IsVerbose>" << (isVerbose() ? "true" : "false") << "</IsVerbose>" << '\n';
		str << "<LogLevel>" << getLogLevel() << "</LogLevel>" << '\n';
		
		str << "</Configuration>" << '\n';
		return str.str();
	}

	
	bool AppConfig::setDefaultLocations() {
		return locations(IdxItHome::getIdxItHome().c_str());
	}

	bool AppConfig::locations(const char* home) {

		std::string homePath = home;
		// Repository Archive Path
		m_historyPath = homePath + HISTORY_PATH;
		return false;
	}




