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

#define MASTERALL_LABEL				"master-all"
#define MASTER_ARCHIVE_LABEL		"master=archive"
#define MASTER_BACKUP1_LABEL		"master=backup1"
#define MASTER_BACKUP2_LABEL		"master=backup2"

#define DERIVATIVEALL_LABEL			"derivative-all"
#define DERIVATIVE_ARCHIVE_LABEL	"master=archive"
#define DERIVATIVE_BACKUP1_LABEL	"master=backup1"
#define DERIVATIVE_BACKUP2_LABEL	"master=backup2"

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

namespace simplearchive {

	class AppOptions;
	/**
	* @brief This is the main configuration object. It is used for the
	* primary configuration options.
	*
	*/
	class AppConfig 
	{
	
	private:
		friend class AppOptions;
		friend class SharedConfig;
		friend class SIAARCConfig;

		static std::unique_ptr<AppConfig> m_this;

		static bool m_verbose;					//< -v --Verbose
		static bool m_quiet;					//
		static bool m_silent;					// 
		static bool m_sql_on;					// Sqlite2 On/Off
		static bool m_file_cat_on;				// File cat On/Off 
		static bool m_www_cat_on;				// WWW cat On/Off (web site)
		static std::string m_logLevel;			// Logging Level
		static std::string m_consoleLevel;		// Logging level to console
		static bool m_lightroom;				// Lightroom intergration On/Off
		static bool m_serverMode;				// Server mode On/Off
		static bool m_dry_run;					// Dry run		On/Off
		static bool m_masterBackup1Enabled;	    // Backup 1 Enabled
		static bool m_masterBackup2Enabled;	    // Backup 2 Enabled
		static bool m_derivativeBackup1Enabled;	// Backup 1 Enabled
		static bool m_derivativeBackup2Enabled;	// Backup 2 Enabled
		static bool m_workspaceEnabled;			// Workspace Enabled
		static bool m_externalExifToolEnabled;	// External Exif Tool Enabled
		
		static bool m_masterViewEnabled;
		static bool m_masterViewFullSizeOn;
		static bool m_masterViewPreview1On;    // Previews Enabled
		static bool m_masterViewThumbnailOn;
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
		static std::string m_ExifMapPath;
		static std::string m_ExifMapFile;
		static std::string m_ExifFileDelim;
		/// This is the path to the template files.
		static std::string m_templatePath;
		/// This is the temp file path.
		static std::string m_tempPath;
		/// This is the archive repository file path.
		static std::string m_userspacePath;
		static std::string m_workspacePath;
		static std::string m_userMetadataPath;
		static std::string m_picturePath;
		static std::string m_wwwImagePath;

		static std::string m_masterPath;
		static std::string m_derivativePath;
		
		
		static std::string m_lightroomPath;
		static std::string m_sourcePath;

		static std::string m_configPath;
		
		static std::string m_hookPath;
		static std::string m_toolsPath;
		static std::string m_sqlDatabasePath;
		static std::string m_backupDestinationPath;
		
		static long m_backupMediaSize;
		
		static ExifDateTime m_fromDate;
		static bool m_isFromDate;
		static ExifDateTime m_toDate;
		static bool m_isToDate;

		static std::string m_masterBackup1;
		static std::string m_masterBackup2;
		static std::string m_derivativeBackup1;
		static std::string m_derivativeBackup2;
		

		static int m_tcpPortNum;
		static int m_udpPortNum;
		static std::string m_udpAddress;

		static std::string m_filenaming;

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
		
		/// @brief Gets the source path.
		const char *getSourcePath();
		/// @brief Gets the archive path.
		/// user definable
		const char *getWorkspacePath();
		const char *getUserMetadataPath();
		/// @brief Gets the Derivative archive path
		/// user definable
		const char *getDerivativePath();
		/// @brief Gets the temp file path.
		/// user definable
		const char *getTempPath();
		/// @brief Gets tools path
		const char *getToolsPath();
		/// @brief Gets the hooks path
		/// user definable
		/// 
		const char *getHookPath();
		/// @brief Gets the path to the metadata template files.
		const char *getTemplatePath();
		/// @brief Gets log file path
		const char *getLogPath();
		/// System path
		const char *getSystemPath();
		/// @brief Gets the path to the crc index database.
		const char *getIndexPath();
		/// @brief Gets the path the history file are stored. These files are used for
		/// @brief the history of the changes made to the archive.
		const char *getHistoryPath();
		/// @brief gets external Exif tool
		const char *getExternalExifTool();
		/// @brief gets external Command line
		const char *getExternalCommandLine();

		const char *getExifMapPath();
		const char *getExifMapFile();
		const char *getExifFileDelim();
		const char *getConfigPath();
		/// @brief Gets home path. This is the root path all default paths are made.
		const char *getHomePath();
		const char *getUserspacePath();
		const char *getPicturePath();
		const char *getWWWImagePath();
		const char *getDatabasePath();
		// This is for media backups
		const char *getBackupDestinationPath();
		bool isSQL();
		/// @brief Gets the Master archive path
		/// user definable
		const char *getMasterPath();
		const char *getLightroomPath();

		const char *getMasterBackup1();
		const char *getMasterBackup2();

		const char* getDerivativeBackup1();
		const char* getDerivativeBackup2();

		


		

		void setBackupDestinationPath(const char *path);
		long getBackupMediaSize();
		void setBackupMediaSize(const char *path);

		ExifDateTime &getFromDate();
		bool isFromDateSet();
		void setFromDate(const char *dateStr);

		ExifDateTime &getToDate();
		bool isToDateSet();
		void setToDate(const char *dateStr);

		bool isDryRun() const;
		bool isSilent() const;
		bool isQuiet() const;
		bool isVerbose() const;
		bool isMasterBackup1Enabled() { return m_masterBackup1Enabled; };
		bool isMasterBackup2Enabled() { return m_masterBackup2Enabled; };
		bool isDerivativeBackup1Enabled() { return m_derivativeBackup1Enabled; };
		bool isDerivativeBackup2Enabled() { return m_derivativeBackup2Enabled; };
		bool isMasterCatalogueEnabled();
		bool isMasterWWWCatalogueEnabled();

		bool isWorkspaceEnabled() { return m_workspaceEnabled; }
		/// @brief Gets path to the Exif Map files. For example the Exiftool map
		/// that maps exiftool keywords to Simple Archive keywords.
		bool isExternalExifToolEnabled() { return m_externalExifToolEnabled; }

		const char *getLogLevel();
		const char *getConsoleLevel();
		const char *getFilenaming();

		bool isLightroom() {
			return m_lightroom;
		}

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
		/// Sets tools path
		void setToolsPath(const char *toolsPath);
		
		
		bool validWorkspacePath();
		bool validSourcePath();
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

		std::shared_ptr<ConfigBlock> getMaster() { // The root will always exist 
			auto p = find(BACKUP_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(MASTER_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getDerivative() { // The root will always exist
			auto p = find(BACKUP_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(DERIVATIVE_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getBackup() { // The root will always exist
			auto p = find(BACKUP_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(BACKUP_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getExifTool() { // The root will always exist 
			auto p = find(EXIFTOOL_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(EXIFTOOL_BLOCK)->second;
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

		bool setDerivative(const char* name, std::string& value, std::string& defaultValue) {
			return setConfigBlock(name, value, defaultValue, DERIVATIVE_BLOCK);
		}

		bool setNetwork(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, NETWORK_BLOCK);
		}

		std::shared_ptr<ConfigBlock> getMasterArchive() {
			return getConfigBlocks(MASTER_ARCHIVE);
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
		void setArchivePath(const char *path);
		
		void setMasterPath(const char *path);
		void setDerivativePath(const char *path);
		void setPicturePath(const char *path);
		void setBackup1(const char *path);
		void setBackup2(const char *path);
		void setSourcePath(const char *path);
		void setLogLevel(const char *logLevel);
		void setConsoleLevel(const char *logLevel);
		void setEventsOn(bool evt);
		void setEventPort(int port);
		void setEventAddress(const char *address);
		void setBackupDestinationPath(const char *str);
		void setBackupMediaSize(const char *str);
		void setFromDate(const char *str);
		void setToDate(const char *str);

		SharedConfig() noexcept {};

		/// @brief Initalises the config object
		//bool init(const char *homePath = nullptr);
		
		void fileBasedValues(const char *homePath, const char* tmpPath);
	};

	class SIAARCConfig : public SharedConfig {
	public:
		SIAARCConfig() = default;
		bool getLightroom() {
			return AppConfig::m_lightroom;
		}
		void setLightroom() {
			AppConfig::m_lightroom = true;
		}
		void setServerModeON() {
			AppConfig::m_serverMode = true;
		}
	};

	class AdminConfiguration : public SharedConfig {

	public:
		AdminConfiguration() = default;
		
	};


}
