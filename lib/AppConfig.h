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

#pragma once
#include "ConfigReader.h"
#include "ExifDateTime.h"





#define SYSTEM_FOLDERS			"System Folders" /*< Path to hook scripts */
#define MASTER_ARCHIVE			"Master Archive"	 /*< Main configuration path */ 
//#define TOOLS_PATH_LABEL           		"ToolsPath"

#define GENERAL_LABEL			"general"
#define LOGGING_LABEL			"logging"
#define NETWORK_LABEL			"network"
#define FOLDERS_LABEL			"folders"
#define MASTER_LABEL			"master"
#define DERIVATIVE_LABEL		"derivative"
#define ARCHIVE_LABEL			"archive"
#define USERSPACE_LABEL			"userspace"
#define METADATA_LABEL			"metadata"
#define WORKSPACE_LABEL			"workspace"
#define BACKUP_LABEL			"backup"
#define EXIFTOOL_LABEL			"exiftool"
#define ALL_LABEL				"all"
#define RAW_LABEL			    "raw"
#define PICTURE_LABEL			"picture"
#define WWW_LABEL			    "www"
#define PLAIN_TEXT_LABEL		"plain"
#define XML_LABEL				"xml"
#define JSON_LABEL				"json"
#define HTML_LABEL				"html"

#define USERALL_LABEL				"user-all"
#define USER_WORKSPACE_LABEL		"user=workspace"
#define USER_PICTURE_LABEL			"user=picture"
#define USER_WWW_LABEL				"user=www"
#define USER_METADATA_LABEL			"user=metadata"

#define SYSTEMALL_LABEL				"system-all"
#define SYSTEM_LABEL				"system=system"
#define LOG_LABEL					"system=log"
#define TOOL_LABEL					"system=tool"
#define HISTORY_LABEL				"system=history"
#define TEMPLATE_LABEL				"system=template"
#define DATABASE_LABEL				"system=database"



/**
	@brief These #defines define the labels the hook files will use
	to reference envronment varibles.
*/
#define HOOK_SCRIPTS_PATH_LABEL			"HookScripsPath" /*< Path to hook scripts */
#define CONFIG_PATH_LABEL				"ConfigPath"	 /*< Main configuration path */ 
#define TOOLS_PATH_LABEL           		"ToolsPath"
#define TEMP_PATH_LABEL           		"TempPath"
#define SOURCE_PATH_LABEL         		"SourcePath"
#define SYSTEM_PATH_LABEL         		"SystemPath"
#define MASTER_PATH_LABEL         		"MasterPath"
#define SQL_LABEL         				"SQL"
#define FILE_RENAMING_LABEL				"FileRename"
//#define MASTER_VIEW_PATH_LABEL         	"MasterCataloguePath"
#define DERIVATIVE_PATH_LABEL         	"DerivativePath"
#define SQL_DATABASE_PATH_LABEL         "SQLDatabasePath"
#define LOG_PATH_LABEL					"LogPath"
#define HOME_PATH_LABEL					"HomePath"
#define INDEX_PATH_LABEL				"IndexPath"
#define HISTORY_PATH_LABEL				"HistoryPath"
#define LIGHTROOM_PATH_LABEL			"LightroomPath"
#define ENABLED_LABEL					"Enabled"
#define BACKUP_ONE_ENABLED_LABEL		"BackupOneEnabled"
#define BACKUP_TWO_ENABLED_LABEL		"BackupTwoEnabled"
#define EXTERNAL_COMMAND_LINE_LABEL		"ExternalCommandLine"
#define EXIF_MAP_PATH_LABEL				"ExifMapPath"
#define EXIF_MAP_FILE_LABEL				"ExifMapFile"
#define EXIF_TOOL_LABEL					"ExifTool"
#define EXIF_COMMANDLINE_LABEL			"ExifCommandLine"
#define EXIF_FILE_DELIM_LABEL			"ExifFileDelim"
#define TEMPLATE_PATH_LABEL				"TemplatePath"
#define PICTURE_PATH_LABEL				"PicturePath"
#define WORKSPACE_PATH_LABEL			"WorkspacePath"
#define USERSPACE_PATH_LABEL			"UserspacePath"
#define METADATA_PATH_LABEL			"MetadataPath"
#define DRY_RUN_LABEL					"DryRun"
#define QUIET_LABEL						"Quiet"
#define SILENT_LABEL					"Silent"
#define LOG_LEVEL_LABEL					"LogLevel"
#define LIGHTROOM_LABEL					"Lightroom"
#define REMOTE_SERVER_MODE_LABEL		"RemoteServerMode"
#define CONSOLE_LEVEL_LABEL				"ConsoleLevel"
#define FILE_CAT_LABEL				"FileCatalogue"
#define WWW_CAT_LABEL				"WWWCatalogue"
#define EVENTS_ENABLED_LABEL		"EventsEnabled"
#define COMMANDS_ENABLED_LABEL		"CommandsEnabled"
#define COMMANDS_PORT_LABEL			"CommandPort"
#define EVENTS_PORT_LABEL			"EventsPort"
#define EVENTS_ADDRESS_LABEL		"EventsAddress"
#define BACKUP_ONE_LABEL			"BackupOne"
#define BACKUP_TWO_LABEL			"BackupTwo"
#define MASTER_BACKUP_ONE_LABEL			"MasterBackupOnePath"
#define MASTER_BACKUP_TWO_LABEL			"MasterBackupTwoPath"
#define DERIVATIVE_BACKUP_ONE_LABEL			"DerivativeBackupOnePath"
#define DERIVATIVE_BACKUP_TWO_LABEL			"DerivativeBackupTwoPath"
#define WORKSPACE_CHECKOUT_LABEL			"workspacecheckout"
#define USER_LOCAL					"local"
#define USER_ALL					"all"



	class AppOption;
	/**
	* @brief This is the main configuration object. It is used for the
	* primary configuration options.
	*
	*/
	class AppConfig 
	{
	
	private:
		friend class AppOption;
		friend class SharedConfig;
		friend class IDXITAppConfig;

		static std::unique_ptr<AppConfig> m_this;

		static bool m_verbose;					//< -v --Verbose
		static bool m_quiet;					//
		static bool m_silent;					// 
		static std::string m_logLevel;			// Logging Level
		static std::string m_consoleLevel;		// Logging level to console
		static bool m_serverMode;				// Server mode On/Off
		static bool m_dry_run;					// Dry run		On/Off
	
		static bool m_eventsOn; // UDP events
		static bool m_serverOn;

		/// Log file path
		static std::string m_logPath;
		/// home path. This is the root path all default paths are made. 
		static std::string m_homePath;
		/// System Path
		static std::string m_systemPath;
		/// This is the path to the crc index database.
		static std::string m_indexPath;
		/// This is the path the history file are stored. These files are used for
		/// the history of the changes made to the archive.  
		static std::string m_historyPath;
		
		/// External Exif tool
		static std::string m_ExternalExifTool;
		/// External Exif Command line
		static std::string m_ExternalCommandLine;
		/// This is path to the Exif Map files. For example the Exiftool map
		/// that maps exiftool keywords to Simple Archive keywords.
		
		/// This is the temp file path.
		static std::string m_tempPath;
		/// This is the archive repository file path.
		

		static std::string m_configPath;
		
		
		
	

		static int m_tcpPortNum;
		static int m_udpPortNum;
		static std::string m_udpAddress;

		

		static bool locations(const char* path);

	public:
		AppConfig() noexcept {}
		~AppConfig() {}
		void settup();

		static bool setDefaultLocations();

		static AppConfig &get() {
			static AppConfig    instance; // Guaranteed to be destroyed.
											  // Instantiated on first use.
			return instance;
		}
		
		
		/// user definable
		const char *getTempPath();
		/// @brief Gets tools path
		const char *getToolsPath();
		/// @brief Gets the hooks path
		/// user definable
		/// 
		
		/// @brief Gets log file path
		const char *getLogPath();
		/// System path
		const char *getSystemPath();
		/// @brief Gets the path to the crc index database.
		const char *getIndexPath();
		/// @brief Gets the path the history file are stored. These files are used for
		/// @brief the history of the changes made to the archive.
		const char *getHistoryPath();
		

		const char *getConfigPath();
		/// @brief Gets home path. This is the root path all default paths are made.
		const char *getHomePath();
	
		bool isDryRun() const;
		bool isSilent() const;
		bool isQuiet() const;
		bool isVerbose() const;
		

		

		const char *getLogLevel();
		const char *getConsoleLevel();
		
		

		bool isServerMode() {
			return m_serverMode;
		}

		bool isEventsOn();
		int eventPort();
		const char *eventAddress();
		bool isServerOn();
		int serverPort();

		std::string toString();
		std::string toXMLString();
		
		
		void isServerOn(bool on);
		void setServerPort(int port);
		
		bool validHomePath();
	};

	

	class SharedConfig : public AppConfigBase {
		

		ConfigBlock &getRoot() { // The root will always exist 
			std::shared_ptr<ConfigBlock> configBlock = find(ROOT_BLOCK)->second;
			return (*configBlock);
		}

		std::shared_ptr<ConfigBlock> getLogging() { // The root will always exist 
			
			auto p = find(LOGGING_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(LOGGING_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getNetwork() { // The root will always exist 
			
			auto p = find(NETWORK_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(NETWORK_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getFolders() { // The root will always exist 
			auto p = find(FOLDERS_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(FOLDERS_BLOCK)->second;
			return configBlock;
		}

		

		bool setGeneral(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, GENERAL_BLOCK);
		}

		bool setLogging(const char* name, std::string& value, std::string& defaultValue) {
			return setConfigBlock(name, value, defaultValue, LOGGING_BLOCK);
		}

		bool setSystemFolders(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, FOLDERS_BLOCK);
		}

		bool setExternalExifTool(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, EXIFTOOL_BLOCK);
		}

		bool setMaster(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, MASTER_BLOCK);
		}
	
		bool setNetwork(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, NETWORK_BLOCK);
		}

		std::shared_ptr<ConfigBlock> getSystemFolders() {
			return getConfigBlocks(SYSTEM_FOLDERS);
		}


	public:

		/// Sets home path. This is the root path all default paths are made. 
		void setHomePath(const char *homePath);
		void setWorkspacePath(const char *path);

		//void setMasterPath(const char *path);

		void setDryRun(bool dryRun);
		void setQuiet(bool quiet);
		void setSilent(bool silent);
		void setVerbose(bool verbose);
		
		
		
		void setSourcePath(const char *path);
		void setLogLevel(const char *logLevel);
		void setConsoleLevel(const char *logLevel);
		void setEventsOn(bool evt);
		void setEventPort(int port);
		void setEventAddress(const char *address);
		

		SharedConfig() noexcept {};

		/// @brief Initalises the config object
		//bool init(const char *homePath = nullptr);
		
		void fileBasedValues(const char *homePath, const char* tmpPath);
	};

	class IDXITAppConfig : public SharedConfig {
	public:
		IDXITAppConfig() = default;
		
	};

	class AdminConfiguration : public SharedConfig {

	public:
		AdminConfiguration() = default;
		
	};

