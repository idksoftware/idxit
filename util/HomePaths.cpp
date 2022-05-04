#include "HomePaths.h"
#include <string>
#include <vector>
#include "SAUtils.h"
#include "AppPaths.h"


bool HomePathsBase::m_first = true;
std::string HomePathsBase::m_allUsersHomeEnvironmentPath;
std::string HomePathsBase::m_myselfHomeEnvironmentPath;
std::string HomePathsBase::m_allusersHomeDefaultPath;
std::string HomePathsBase::m_myselfHomeDefaultPath;
std::string HomePathsBase::m_homeDrive;
std::string HomePathsBase::m_homePathEnv;
std::string HomePathsBase::m_relativeRoot = "\\Idxit";


std::vector<HomePathsBase*> IdxItHome::m_list;
std::string IdxItHome::m_home;
std::string IdxItHome::m_primaryPath;

void IdxItHome::initHomePaths() {
	
	

	
}



const std::string& IdxItHome::getIdxItHome() {
	return m_home;
}



bool IdxItHome::init() {

	bool res = true;
	for (auto i = m_list.begin(); i != m_list.end(); i++) {
		auto hp = *i;
		if (hp->init() == false) {
			m_error = HPError::initaliseError;
			res = false;
		}
	}

	return res;
}

bool IdxItHome::checkAndMakePaths() {

	bool res = true;
	for (auto i = m_list.begin(); i != m_list.end(); i++) {
		auto hp = *i;
		if (hp->checkAndMakePath() == false) {
			m_error = HPError::initaliseError;
			res = false;
		}
	}
	m_primaryPath = m_home;
	checkAndMakePath();
	return res;
}

bool IdxItHome::checkAndMakePath()
{
	if (SAUtils::DirExists(m_primaryPath.c_str()) == false) {
		if (SAUtils::mkDir(m_primaryPath.c_str()) == false) {
			return false;
		}
	}
}
/*
 * Linux
 *
 * All User
 *
 * /opt Optional application software packages
 * /etc/opt Configuration files for add-on packages that are stored in /opt
 * /var Variable files: files whose content is expected to continually change during normal operation of the system, such as logs, spool files, and temporary e-mail files.
 * /var/cache Application cache data. Such data are locally generated as a result of time-consuming I/O or calculation. The application must be able to regenerate or restore the data. The cached files can be deleted without loss of data.
 * /var/opt Variable data from add-on packages that are stored in /opt.
 *
 * Local user
 *Archive folder – $HOME/.local/share/imgarchive

Master folder – $HOME/.local/share/imgarchive/master

Derivative folder – $HOME/.local/share/imgarchive/master

Workspace folder – $HOME/imgarchive/workspace

Picture folder – $HOME/imgarchive/workspace

WWW image folder – $HOME/imgarchive/workspace
 *
 */

bool IdxItHome::setArchiveHome()
{
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IDXIT_HOME, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IDXIT_HOME, false);
	// All Users
#ifdef WIN32
	std::string allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("ProgramData");
	allusersHomeDefaultPath += DEFAULT_ALLUSER_HOME_PATH;
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA");
	myselfHomeDefaultPath += DEFAULT_LOCAL_HOME_PATH;
#else
	std::string allusersHomeDefaultPath = LINUX_VAR_OPT;
	allusersHomeDefaultPath += DEFAULT_ALLUSER_HOME_PATH;
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME");
	myselfHomeDefaultPath += DEFAULT_LOCAL_HOME_PATH;
#endif
	// Looking the HKEY_LOCAL_MACHINE first
	if (allUsersHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::SystemEnv;	// System Environment set
		m_found = true;
		m_home = allUsersHomeEnvironmentPath;
	}
	else if (myselfHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::LocalEnv;
		m_found = true;
		m_home = myselfHomeEnvironmentPath;
	}
	else if (SAUtils::DirExists(allusersHomeDefaultPath.c_str()) == true) {
		m_home = allusersHomeDefaultPath;
		m_type = HomePathType::AllUsers;
		m_found = true;

	}
	else if (SAUtils::DirExists(myselfHomeDefaultPath.c_str()) == true) {
		m_home = myselfHomeDefaultPath;
		m_type = HomePathType::UserOnly;
		m_found = true;

	}
	else {
		m_error = HPError::NotFound;
		return false;
	}

	if (m_type == HomePathType::SystemEnv) {
		if (SAUtils::DirExists(m_home.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}

	}
	else if (m_type == HomePathType::LocalEnv) {

		if (SAUtils::DirExists(m_home.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}

	}
	m_valid = true;
	return true;
}

std::string IdxItHome::errorStr()
{
	std::string errStr;
	switch (m_error) {
	case HPError::Ok:					// OK
		return "Ok no error";
	case HPError::initaliseError:
		return "Initalise error";
	case HPError::CannotLocatePath:	// Cannot locate path at default or HOME if set
		return "Cannot locate path at default or HOME if set";
	case HPError::NotFound:			// Path not at default and HOME not set 
		return "Path not at default and IDXIT_HOME not set";
	case HPError::Unknown:				// Unknown state
	default:
		break;
	}
	return "Unknown error";
}



bool HomePathsBase::loadEnv() {
	
	HomePathsBase::m_allUsersHomeEnvironmentPath = SAUtils::GetEnv(IDXIT_HOME, true);
	HomePathsBase::m_myselfHomeEnvironmentPath = SAUtils::GetEnv(IDXIT_HOME, false);
	// All Users
	HomePathsBase::m_allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("ProgramData");
	HomePathsBase::m_myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA");

	HomePathsBase::m_homeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
	HomePathsBase::m_homePathEnv = SAUtils::GetPOSIXEnv("HOMEPATH");

	return true;
}

bool HomePathsBase::baseSysInit(std::string &path, const char *relativePath, const char *enviromentPath)
{

	path = IdxItHome::getIdxItHome();
	path += relativePath;
	m_type = HomePathType::Default;
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(enviromentPath, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(enviromentPath, false);
	// Looking the HKEY_LOCAL_MACHINE first
	if (allUsersHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::SystemEnv;	// System Environment set
		path = allUsersHomeEnvironmentPath;

	}
	else if (myselfHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::LocalEnv;
		path = myselfHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(path.c_str()) == false) {
		m_error = HPError::CannotLocatePath;
		return false;

	}

	m_valid = true;
	return true;
}

bool HomePathsBase::baseUserInit(std::string& path, const char* relativePath, const char* enviromentPath)
{

	std::string homeDrive = m_homeDrive;
	std::string homePath = m_homePathEnv;

	m_found = true;
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(enviromentPath, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(enviromentPath, false);

	path = homeDrive + homePath + m_relativeRoot + relativePath;
	m_type = HomePathType::Default;
	// Looking the HKEY_LOCAL_MACHINE first
	if (allUsersHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::SystemEnv;	// System Environment set
		path = allUsersHomeEnvironmentPath;
		m_changed = true;
	}
	else if (myselfHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::LocalEnv;
		path = myselfHomeEnvironmentPath;
		m_changed = true;
	}
	/*
	else if (SAUtils::DirExists(path.c_str()) == false) {
		m_error = HPError::CannotLocatePath;
		return false;
	}
	*/
	m_valid = true;
	return true;
}

void HomePathsBase::baseEnableInit(bool &option, bool defaultOption, const char* enviromentOption)
{

	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentOption = SAUtils::GetEnv(enviromentOption, true);
	std::string myselfHomeEnvironmentOption = SAUtils::GetEnv(enviromentOption, false);

	option = defaultOption;
	
	// Looking the HKEY_LOCAL_MACHINE first
	if (allUsersHomeEnvironmentOption.empty() == false) {
		
		BoolOption opt = SAUtils::isTrueFalse(allUsersHomeEnvironmentOption);
		option = (BoolOption::True == opt);
	}
	else if (myselfHomeEnvironmentOption.empty() == false) {
		
		BoolOption opt = SAUtils::isTrueFalse(myselfHomeEnvironmentOption);
		option = (BoolOption::True == opt);
	}
}

