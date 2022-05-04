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
std::string HomePathsBase::m_relativeRoot = "\\ImgArchive";


std::vector<HomePathsBase*> ImgArchiveHome::m_list;
std::string ImgArchiveHome::m_imgArchiveHome;
std::string ImgArchiveHome::m_primaryPath;

void ImgArchiveHome::initHomePaths() {
	
	HomePathsBase* path = &(MasterPath::getObject());
	m_list.push_back(path);
	path = &(MasterBackupOnePath::getObject());
	m_list.push_back(path);
	path = &(MasterBackupTwoPath::getObject());
	m_list.push_back(path);
	path = &(DerivativePath::getObject());
	m_list.push_back(path);
	path = &(DerivativeBackupOnePath::getObject());
	m_list.push_back(path);
	path = &(DerivativeBackupTwoPath::getObject());
	m_list.push_back(path);
	path = &(UserspacePath::getObject());
	m_list.push_back(path);
	path = &(WorkspacePath::getObject());
	m_list.push_back(path);
	path = &(PicturePath::getObject());
	m_list.push_back(path);
	path = &(WWWImagePath::getObject());
	m_list.push_back(path);
	path = &(UserMetadataPath::getObject());
	m_list.push_back(path);

	m_primaryPath = m_imgArchiveHome;
	m_primaryPath += "\\pi";
}



const std::string& ImgArchiveHome::getImgArchiveHome() {
	return m_imgArchiveHome;
}

const std::string& ImgArchiveHome::getPrimaryPath() {
	return m_primaryPath;
}

bool ImgArchiveHome::init() {

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

bool ImgArchiveHome::checkAndMakePaths() {

	bool res = true;
	for (auto i = m_list.begin(); i != m_list.end(); i++) {
		auto hp = *i;
		if (hp->checkAndMakePath() == false) {
			m_error = HPError::initaliseError;
			res = false;
		}
	}
	checkAndMakePath();
	return res;
}

bool ImgArchiveHome::checkAndMakePath()
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

bool ImgArchiveHome::setArchiveHome()
{
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, false);
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
		m_imgArchiveHome = allUsersHomeEnvironmentPath;
	}
	else if (myselfHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::LocalEnv;
		m_found = true;
		m_imgArchiveHome = myselfHomeEnvironmentPath;
	}
	else if (SAUtils::DirExists(allusersHomeDefaultPath.c_str()) == true) {
		m_imgArchiveHome = allusersHomeDefaultPath;
		m_type = HomePathType::AllUsers;
		m_found = true;

	}
	else if (SAUtils::DirExists(myselfHomeDefaultPath.c_str()) == true) {
		m_imgArchiveHome = myselfHomeDefaultPath;
		m_type = HomePathType::UserOnly;
		m_found = true;

	}
	else {
		m_error = HPError::NotFound;
		return false;
	}

	if (m_type == HomePathType::SystemEnv) {
		if (SAUtils::DirExists(m_imgArchiveHome.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}

	}
	else if (m_type == HomePathType::LocalEnv) {

		if (SAUtils::DirExists(m_imgArchiveHome.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}

	}
	m_valid = true;
	return true;
}

std::string ImgArchiveHome::errorStr()
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
		return "Path not at default and IMGARCHIVE_HOME not set";
	case HPError::Unknown:				// Unknown state
	default:
		break;
	}
	return "Unknown error";
}



bool HomePathsBase::loadEnv() {
	
	HomePathsBase::m_allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, true);
	HomePathsBase::m_myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, false);
	// All Users
	HomePathsBase::m_allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("ProgramData");
	HomePathsBase::m_myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA");

	HomePathsBase::m_homeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
	HomePathsBase::m_homePathEnv = SAUtils::GetPOSIXEnv("HOMEPATH");

	return true;
}

bool HomePathsBase::baseSysInit(std::string &path, const char *relativePath, const char *enviromentPath)
{

	path = ImgArchiveHome::getImgArchiveHome();
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

/**
	Master Path
*/

bool MasterPath::init()
{
	return baseSysInit(m_path, MASTER_PATH, IMGA_MASTER);
}

std::string MasterPath::m_path;

const std::string& MasterPath::get()
{
	return m_path;
}

bool MasterPath::setPath(const char* p)
{
	m_path = p;
	return true;
}

/**
	Master backup 1 Path
*/

bool MasterBackupOnePath::init()
{
	baseEnableInit(MasterBackupOnePath::m_enabled, false, IMGA_MASTER_BACKUP1_ENABLE);
	return baseSysInit(m_path, DEFAULT_MASTER_BACKUP1_PATH, IMGA_MASTER_BACKUP1);
}

std::string MasterBackupOnePath::m_path;
bool MasterBackupOnePath::m_enabled = false;

const std::string& MasterBackupOnePath::get()
{
	return m_path;
}

bool MasterBackupOnePath::setPath(const char* p)
{
	m_path = p;
	return true;
}

/**
	Master backup 2 Path
*/

bool MasterBackupTwoPath::init()
{
	baseEnableInit(MasterBackupTwoPath::m_enabled, false, IMGA_MASTER_BACKUP2_ENABLE);
	return baseSysInit(m_path, DEFAULT_MASTER_BACKUP2_PATH, IMGA_MASTER_BACKUP2);
}

std::string MasterBackupTwoPath::m_path;
bool MasterBackupTwoPath::m_enabled = false;

const std::string& MasterBackupTwoPath::get()
{
	return m_path;
}

bool MasterBackupTwoPath::setPath(const char* p)
{
	m_path = p;
	return true;
}

/**
	Derivative Path
*/

bool DerivativePath::init()
{
	return baseSysInit(m_path, DERIVATIVE_PATH, IMGA_DERIVATIVE);
}

std::string DerivativePath::m_path;


const std::string& DerivativePath::get()
{
	return m_path;
}


bool DerivativePath::setPath(const char* p)
{
	m_path = p;
	return true;
}

/**
	DerivativeBackupOne Path
*/

bool DerivativeBackupOnePath::init()
{
	baseEnableInit(DerivativeBackupOnePath::m_enabled, false, IMGA_DERIVATIVE_BACKUP1_ENABLE);
	return baseSysInit(m_path, DEFAULT_DERIVATIVE_BACKUP1_PATH, IMGA_DERIVATIVE_BACKUP1);
}

std::string DerivativeBackupOnePath::m_path;
bool DerivativeBackupOnePath::m_enabled = false;

const std::string& DerivativeBackupOnePath::get()
{
	return m_path;
}

bool DerivativeBackupOnePath::setPath(const char* p)
{
	m_path = p;
	return true;
}

/**
	DerivativeBackupTwo Path
*/

bool DerivativeBackupTwoPath::init()
{
	baseEnableInit(DerivativeBackupTwoPath::m_enabled, false, IMGA_DERIVATIVE_BACKUP2_ENABLE);
	return baseSysInit(m_path, DEFAULT_DERIVATIVE_BACKUP2_PATH, IMGA_DERIVATIVE_BACKUP2);
}

std::string DerivativeBackupTwoPath::m_path;
bool DerivativeBackupTwoPath::m_enabled = false;

const std::string& DerivativeBackupTwoPath::get()
{
	return m_path;
}

bool DerivativeBackupTwoPath::setPath(const char* p)
{
	m_path = p;
	return true;
}

std::string UserspacePath::m_path;
std::string UserspacePath::m_hidden;

bool UserspacePath::m_autoView = true;

bool UserspacePath::init()
{
	return baseUserInit(m_path, DEFAULT_USERSPACE_PATH, IMGA_USERSPACE);	
}

const std::string& UserspacePath::get()
{
	return m_path;
}

bool UserspacePath::autoViewOn()
{
	return m_autoView;
}

bool UserspacePath::checkAndMakePath()
{
	if (SAUtils::DirExists(m_path.c_str()) == false) {
		if (SAUtils::mkDir(m_path.c_str()) == false) {
			return false;
		}
	}
	m_hidden = m_path;
	m_hidden += "/.imga";
	if (SAUtils::DirExists(m_hidden.c_str()) == false) {
		if (SAUtils::mkDir(m_hidden.c_str()) == false) {
			return false;
		}
	}
	if (SAUtils::setHidden(m_hidden.c_str()) == false) {
		return false;
	}
}

bool UserspacePath::setPath(const char* p)
{
	m_path = p;
	return true;
}



/**
	Workspace Path
*/

std::string WorkspacePath::m_path;
std::string WorkspacePath::m_hidden;
bool WorkspacePath::m_autoView = true;
bool WorkspacePath::m_autoCheckout = false;

bool WorkspacePath::init()
{
	baseEnableInit(WorkspacePath::m_autoView, true, IMGA_WORKSPACE_ENABLE);
	baseEnableInit(WorkspacePath::m_autoCheckout, true, IMGA_WORKSPACE_CHKOUT);
	return baseUserInit(m_path, DEFAULT_WORKSPACE_PATH, IMGA_WORKSPACE);
}

const std::string& WorkspacePath::get()
{
	return m_path;
}

bool WorkspacePath::autoViewOn()
{
	return m_autoView;
}

bool WorkspacePath::autoCheckout()
{
	return m_autoCheckout;
}

bool WorkspacePath::setPath(const char* p)
{
	m_path = p;
	return true;
}

bool WorkspacePath::checkAndMakePath()
{
	if (m_autoView) {
		if (SAUtils::DirExists(m_path.c_str()) == false) {
			if (SAUtils::mkDir(m_path.c_str()) == false) {
				return false;
			}
		}
		m_hidden = m_path;
		m_hidden += "/.imga";
		if (SAUtils::DirExists(m_path.c_str()) == false) {
			if (SAUtils::mkDir(m_path.c_str()) == false) {
				return false;
			}
		}
		if (SAUtils::setHidden(m_hidden.c_str()) == false) {
			return false;
		}
	}

}

/*
* UserMetadata
*/
std::string UserMetadataPath::m_path;
bool UserMetadataPath::m_autoView = true;


bool UserMetadataPath::init()
{
	//baseEnableInit(UserMetadataPath::m_autoView, false, IMGA_WORKSPACE_ENABLE);
	return baseUserInit(m_path, DEFAULT_METADATA_PATH, IMGA_METADATA);
}

const std::string& UserMetadataPath::get()
{
	return m_path;
}

bool UserMetadataPath::autoViewOn()
{
	return m_autoView;
}

bool UserMetadataPath::checkAndMakePath()
{
	std::string hiddenUser = UserspacePath::getHidden();
	if (SAUtils::DirExists(hiddenUser.c_str()) == false) {
		if (SAUtils::mkDir(hiddenUser.c_str()) == false) {
			return false;
		}
	}
	m_path = hiddenUser;
	m_path += "/metadata";
	if (SAUtils::DirExists(m_path.c_str()) == false) {
		if (SAUtils::mkDir(m_path.c_str()) == false) {
			return false;
		}
	}
	if (SAUtils::setHidden(m_path.c_str()) == false) {
		return false;
	}
}

bool UserMetadataPath::setPath(const char* p)
{
	m_path = p;
	return true;
}


/**
	Picture Path
*/

std::string PicturePath::m_path;
std::string PicturePath::m_hidden;
bool PicturePath::m_autoView = false;

bool PicturePath::init()
{
	baseEnableInit(PicturePath::m_autoView, true, IMGA_WORKSPACE_ENABLE);
	return baseUserInit(m_path, DEFAULT_PICTURE_PATH, IMGA_PICTURE);
}

const std::string& PicturePath::get()
{
	return m_path;
}

bool PicturePath::checkAndMakePath()
{
	if (m_autoView) {
		if (SAUtils::DirExists(m_path.c_str()) == false) {
			if (SAUtils::mkDir(m_path.c_str()) == false) {
				return false;
			}
		}
		m_hidden = m_path;
		m_hidden += "/.imga";
		if (SAUtils::DirExists(m_path.c_str()) == false) {
			if (SAUtils::mkDir(m_path.c_str()) == false) {
				return false;
			}
		}
		if (SAUtils::setHidden(m_path.c_str()) == false) {
			return false;
		}
	}
}


bool PicturePath::setPath(const char* p)
{
	m_path = p;
	return true;
}

bool PicturePath::autoViewOn()
{
	return m_autoView;
}
/**
	WWWImage Path
*/

std::string WWWImagePath::m_path;
std::string WWWImagePath::m_hidden;
bool WWWImagePath::m_autoView = false;

bool WWWImagePath::init()
{
	baseEnableInit(WWWImagePath::m_autoView, false, IMGA_WWWIMAGE_ENABLE);
	return baseUserInit(m_path, DEFAULT_WWWIMAGE_PATH, IMGA_WWWIMAGE);
}

const std::string& WWWImagePath::get()
{
	return m_path;
}

bool WWWImagePath::checkAndMakePath()
{
	if (m_autoView) {
		if (SAUtils::DirExists(m_path.c_str()) == false) {
			if (SAUtils::mkDir(m_path.c_str()) == false) {
				return false;
			}
		}
		m_hidden = m_path;
		m_hidden += "/.imga";
		if (SAUtils::DirExists(m_path.c_str()) == false) {
			if (SAUtils::mkDir(m_path.c_str()) == false) {
				return false;
			}
		}
	}
}

bool WWWImagePath::setPath(const char* p)
{
	m_path = p;
	return true;
}

bool WWWImagePath::autoViewOn()
{
	return m_autoView;
}
