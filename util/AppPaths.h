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

#define SYSTEM_PATH					    "/system"


#define HOOKS_PATH						"/hooks"

#define TOOLS_PATH						"/tools"
#define CONFIG_PATH						"/config"

#define HISTORY_PATH					"/history"
#define BACKUPS_PATH					"/backups"

#define TEMP_PATH						"/tmp"
#define LOGS_PATH						"/logs"
#define INDEX_PATH						"/index"		// CRC index

#define DUPS_PATH						"/dups"
#define SQLITEDB_PATH					"/sqldb"

#define LOG_PATH						"/logs"





#define SQLITE3_DB						"IASqlite3.db"

#define DEFAULT_HOME_PATH		   "/IDK-Software/IdxIt"

constexpr auto IDXIT_HOME = "IDXIT_HOME";


constexpr auto BASE_LINUX_BIN_PATH = "/";
constexpr auto LINUX_OPT = "/opt"; // /opt Optional application software packages
constexpr auto LINUX_ETC_OPT = "/etc/opt"; // /etc/opt Configuration files for add-on packages that are stored in /opt
constexpr auto LINUX_VAR = "/var";
// /var Variable files: files whose content is expected to continually change during normal operation of the system, such as logs, spool files, and temporary e-mail files.
constexpr auto LINUX_VAR_CACHE = "/var/cache";
// /var/cache Application cache data. Such data are locally generated as a result of time-consuming I/O or calculation. The application must be able to regenerate or restore the data. The cached files can be deleted without loss of data.
constexpr auto LINUX_VAR_OPT = "/var/opt"; // /var/opt Variable data from add-on packages that are stored in /opt.

constexpr auto DEFAULT_ALLUSER_HOME_PATH = "/IDK-Software/IdxIt";
constexpr auto DEFAULT_ALLUSER_WWWIMAGE_PATH = "/IDK-Software/IdxIt/IAWWWPictures";
constexpr auto DEFAULT_ALLUSER_PICTURE_PATH = "/IdxIt/Pictures";
constexpr auto DEFAULT_ALLUSER_WORKSPACE_PATH = "/IdxIt/Workspace";
constexpr auto DEFAULT_ALLUSER_MASTER_PATH = "/master";
constexpr auto DEFAULT_ALLUSER_DERIVATIVE_PATH = "/derivative";

#ifdef WIN32
constexpr auto DEFAULT_LOCAL_HOME_PATH = "/IdxIt";
constexpr auto DEFAULT_LOCAL_WWWIMAGE_PATH = "/IdxIt/WWWPictures";
constexpr auto DEFAULT_LOCAL_PICTURE_PATH = "/IdxIt/Pictures";
constexpr auto DEFAULT_LOCAL_WORKSPACE_PATH = "/IdxIt/Workspace";
constexpr auto DEFAULT_LOCAL_MASTER_PATH = "/master";
constexpr auto DEFAULT_LOCAL_DERIVATIVE_PATH = "/derivative";
#else
constexpr auto DEFAULT_LOCAL_HOME_PATH = "/.IdxIt";
constexpr auto DEFAULT_LOCAL_WWWIMAGE_PATH = "/IdxIt/WWWPictures";
constexpr auto DEFAULT_LOCAL_PICTURE_PATH = "/IdxIt/Pictures";
constexpr auto DEFAULT_LOCAL_WORKSPACE_PATH = "/IdxIt/Workspace";
constexpr auto DEFAULT_LOCAL_MASTER_PATH = "/.IdxIt/master";
constexpr auto DEFAULT_LOCAL_DERIVATIVE_PATH = "/.IdxIt/derivative";
#endif

//constexpr autoEFAULT_WORKSPACE_PATH = "/IAWorkspace";
//m_sourcePath = tempHomeDrive + tempHomePath + "/Pictures";
//m_hookPath = m_homePath + "/hooks";
