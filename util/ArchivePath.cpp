/*
 * ArchivePath.cpp
 *
 * The IdxIt consists of two main archives Master
 *  Created on: 23 Nov 2015
 *      Author: FergusonI
 */

#include "SAUtils.h"
#include "ArchivePath.h"
#include "ErrorCode.h"
#include "CLogger.h"

namespace simplearchive {

#undef FILECODE
#define FILECODE ARCHIVEPATH_CPP

ArchivePath& ArchivePath::getArchivePath() {
	static ArchivePath INSTANCE;
	return INSTANCE;
}



bool ArchivePath::settupRelative(std::string &yyyymmddStr) {

	std::string yearStr = yyyymmddStr.substr(0, 4);
	
	std::string workspaceYearPath = m_pathToWorkspace + '/' + yearStr;
	if (SAUtils::DirExists(workspaceYearPath.c_str()) == false) {
		SAUtils::mkDir(workspaceYearPath.c_str());
	}
	m_workspaceYyyymmddPath = workspaceYearPath + '/' + yyyymmddStr;

	if (SAUtils::DirExists(m_workspaceYyyymmddPath.c_str()) == false) {
		SAUtils::mkDir(m_workspaceYyyymmddPath.c_str());
	}
	m_workspaceMetadataPath = m_workspaceYyyymmddPath + WORKSPACE_METADATA_PATH;
	if (SAUtils::DirExists(m_workspaceMetadataPath.c_str()) == false) {
		SAUtils::mkDir(m_workspaceMetadataPath.c_str());
	}
	return true;
}

/*
class PrimaryIndex {
	std::string m_ImageIndexPath;
	std::string m_idxDBPath;
	std::string m_logPath;
	std::string m_metadataPath;
public:
*/
void PrimaryIndexPath::setRepositoryPath(std::string &pathToRepository) {
	m_pathToRepository = pathToRepository;
}

bool PrimaryIndexPath::settup() {

	CLogger &logger = CLogger::getLogger();

	std::string temp = m_pathToRepository;
	if (SAUtils::DirExists(temp.c_str()) == false) {
		SAUtils::mkDir(temp.c_str());
	}

	m_ImageIndexPath = m_pathToRepository + IMAGEID_PATH;
	m_idxDBPath = m_pathToRepository + INDEX_PATH;
	m_historyPath = m_pathToRepository + HISTORY_PATH;
	m_metadataPath = m_pathToRepository + METADATA_PATH;
	m_CSVDatabasePath = m_pathToRepository + CSVDB_PATH;
	m_CheckoutStatusPath = m_pathToRepository + CHKSTATUS_PATH;

	if (SAUtils::DirExists(m_ImageIndexPath.c_str()) == false) {
		if (SAUtils::mkDir(m_ImageIndexPath.c_str()) == false) {
			return false;
		}
	}

	if (SAUtils::DirExists(m_idxDBPath.c_str()) == false) {
		if (SAUtils::mkDir(m_idxDBPath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid index path: \"%s\"?", m_idxDBPath.c_str());
			return false;
			
		}
	}

	if (SAUtils::DirExists(m_historyPath.c_str()) == false) {
		if (SAUtils::mkDir(m_historyPath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid History path: \"%s\"?", m_historyPath.c_str());
			return false;
		}
	}

	if (SAUtils::DirExists(m_metadataPath.c_str()) == false) {
		if (SAUtils::mkDir(m_metadataPath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid metadata path: \"%s\"?", m_metadataPath.c_str());
			return false;
		}
	}

	if (SAUtils::DirExists(m_CSVDatabasePath.c_str()) == false) {
		if (SAUtils::mkDir(m_CSVDatabasePath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid CSV database path: \"%s\"?", m_CSVDatabasePath.c_str());
			return false;
		}
	}
	if (SAUtils::DirExists(m_CheckoutStatusPath.c_str()) == false) {
		if (SAUtils::mkDir(m_CheckoutStatusPath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid checkout status path: \"%s\"?", m_CheckoutStatusPath.c_str());
			return false;
		}
	}
	return true;
}

bool RepositoryPath::settup() {
	CLogger &logger = CLogger::getLogger();
	
	// Master Archive
	std::string dataFolder = m_pathToRepository;
	if (SAUtils::DirExists(dataFolder.c_str()) == false) {
		if (SAUtils::mkDir(dataFolder.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid path to repository: \"%s\"?", m_pathToRepository.c_str());
			return false;
		}
	}

	std::string systemFolder = m_pathToRepository;
	systemFolder += SYSTEM_PATH;
	if (SAUtils::DirExists(systemFolder.c_str()) == false) {
		if (SAUtils::mkDir(systemFolder.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid path to repository system folder: \"%s\"?", systemFolder.c_str());
			return false;
		}

	}
	if (m_metadataPath.empty() == true) {
		m_metadataPath = systemFolder + METADATA_PATH;
		m_idxDBPath = systemFolder + IMAGEID_PATH;
		m_journalPath = systemFolder + JOURNAL_PATH;
		m_historySystemPath = systemFolder + HISTORY_PATH;
		m_DBPath = systemFolder + DATABASE_PATH;
		m_ImageIndexPath = systemFolder + INDEX_PATH;

		if (SAUtils::DirExists(m_metadataPath.c_str()) == false) {
			if (SAUtils::mkDir(m_metadataPath.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid metadata path: \"%s\"?", m_metadataPath.c_str());
				return false;
			}
		}
		/*
		if (SAUtils::DirExists(m_ImageIndexPath.c_str()) == false) {
			if (SAUtils::mkDir(m_ImageIndexPath.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid image index path: \"%s\"?", m_ImageIndexPath.c_str());
				return false;
			}
		}
		*/
		/*
		if (SAUtils::DirExists(m_idxDBPath.c_str()) == false) {
			if (SAUtils::mkDir(m_idxDBPath.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid m_index DB Path: \"%s\"?", m_idxDBPath.c_str());
				return false;
			}
		}
		*/
		/*
		if (SAUtils::DirExists(m_historyLogPath.c_str()) == false) {
			if (SAUtils::mkDir(m_historyLogPath.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid history log path: \"%s\"?", m_historyLogPath.c_str());
				return false;
			}
		}
		*/
		if (SAUtils::DirExists(m_DBPath.c_str()) == false) {
			if (SAUtils::mkDir(m_DBPath.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid DB Path: \"%s\"?", m_DBPath.c_str());
				return false;
			}
		}
		
	}
	return true;
}

	

bool RepositoryPath::settupRelative(std::string &yyyymmddStr) {

	m_yearStr = yyyymmddStr.substr(0, 4);
	m_yyyymmddStr = yyyymmddStr;

	m_relpath = m_yearStr + '/' + yyyymmddStr;

		
	m_yearStrPath = m_pathToRepository + '/' + m_yearStr;
	if (SAUtils::DirExists(m_yearStrPath.c_str()) == false) {
		if (SAUtils::mkDir(m_yearStrPath.c_str()) == false) {
		return false;
			}

	}

	m_yyyymmddStrPath = m_yearStrPath + '/' + m_yyyymmddStr;
	if (SAUtils::DirExists(m_yyyymmddStrPath.c_str()) == false) {
		if (SAUtils::mkDir(m_yyyymmddStrPath.c_str()) == false) {
			return false;
		}

	}
	m_dataPath = m_yyyymmddStrPath + MASTER_IMAGE_PATH;
	m_metadataPath = m_yyyymmddStrPath + METADATA_PATH;

	if (SAUtils::DirExists(m_dataPath.c_str()) == false) {
		if (SAUtils::mkDir(m_dataPath.c_str()) == false) {
			return false;
		}

	}
	if (SAUtils::DirExists(m_metadataPath.c_str()) == false) {
		if (SAUtils::mkDir(m_metadataPath.c_str()) == false) {
			return false;
		}

	}
	
	return true;
}





bool RepositoryPath::isEnabled() {
	return m_enabled;
}

void RepositoryPath::setImageName(const char *imageName) {
	m_imageName = imageName;
}

const std::string& RepositoryPath::getImageName() const {
	return m_imageName;
}

const std::string& RepositoryPath::getYYYYMMDD() const {
	return m_yyyymmddStr;
}

const std::string& RepositoryPath::getYYYYMMDDPath() const {
	return m_yyyymmddStrPath;
}

const std::string& RepositoryPath::getRelativePath() const {
	return m_relpath;
}

std::string RepositoryPath::getDataPath() {
	return m_dataPath;
}

std::string RepositoryPath::getCatalog() const {
	return m_catalog;
}

void RepositoryPath::setCatalog(std::string& catalog) {
	m_catalog = catalog;
}

std::string RepositoryPath::getImageHistory() const {
	return m_historyImagePath;
}

std::string RepositoryPath::getSystemHistory() const {
	return m_historySystemPath;
}

std::string RepositoryPath::getCSVDatabasePath() const {
	return m_DBPath;
}

void RepositoryPath::setImageHistory(std::string& history) {
	m_historyImagePath = history;
}

std::string RepositoryPath::getJournalPath() const {
	return m_journalPath;
}

void RepositoryPath::setJournalPath(std::string& journalPath) {
	m_journalPath = journalPath;
}

std::string RepositoryPath::getMetadataPath() const {
	return m_metadataPath;
}

void RepositoryPath::setMetadataPath(std::string& metadataPath) {
	m_metadataPath = metadataPath;
}

std::string RepositoryPath::getIdxDBPath() const {
	return m_idxDBPath;
}

void RepositoryPath::setIdxDBPath(std::string& idxDBPath) {
	m_idxDBPath = idxDBPath;
}

std::string RepositoryPath::getImageIdxPath() const {
	return m_ImageIndexPath;
}

void RepositoryPath::setEnabled(bool b) {
	m_enabled = b;
}
	
void RepositoryPath::setRepositoryPath(std::string &pathToRepository) {
	m_pathToRepository = pathToRepository;
//	m_enabled = true;
}
	
std::string  RepositoryPath::getRepositoryPath() const {
	return m_pathToRepository;	
}
	

std::string ArchivePath::m_pathToHome;
std::string ArchivePath::m_pathToWorkspace;
std::string ArchivePath::m_mainMetadataPath;
std::string ArchivePath::m_userMetadataPath;
std::string ArchivePath::m_mainDupsPath;
std::string ArchivePath::m_mainHistory;
//static ArchivePath *m_master;
std::string ArchivePath::m_pathToMaster;

std::string ArchivePath::m_workspaceYyyymmddPath;
std::string ArchivePath::m_workspaceMetadataPath;

RepositoryPath ArchivePath::m_master;
RepositoryPath ArchivePath::m_derivative;
RepositoryPath ArchivePath::m_masterBackupPath[2];
RepositoryPath ArchivePath::m_derivativeBackupPath[2];
PrimaryIndexPath ArchivePath::m_primaryIndex;

//std::string ArchivePath::m_backup1;
//std::string ArchivePath::m_backup2;


bool ArchivePath::isMasterEnabled() {
	return m_master.isEnabled();
}

bool ArchivePath::isDerivativeEnabled() {
	return m_derivative.isEnabled();
}

std::string ArchivePath::getMasterBackup1Path() {
	return m_masterBackupPath[0].getRepositoryPath();
}

std::string ArchivePath::getMasterBackup2Path() {
	return m_masterBackupPath[1].getRepositoryPath();
}

void ArchivePath::setMasterBackup1Path(std::string &pathToBackup) {
	m_masterBackupPath[0].setRepositoryPath(pathToBackup);
}

void ArchivePath::setMasterBackup2Path(std::string &pathToBackup) {
	m_masterBackupPath[1].setRepositoryPath(pathToBackup);
}

std::string ArchivePath::getDerivativeBackup1Path()
{
	return m_derivativeBackupPath[0].getRepositoryPath();
}

void ArchivePath::setDerivativeBackup1Path(std::string& pathToBackup1)
{
	m_derivativeBackupPath[0].setRepositoryPath(pathToBackup1);
}

std::string ArchivePath::getDerivativeBackup2Path()
{
	return m_derivativeBackupPath[1].getRepositoryPath();
}

void ArchivePath::setDerivativeBackup2Path(std::string& pathToBackup2)
{
	m_derivativeBackupPath[1].setRepositoryPath(pathToBackup2);
}

void ArchivePath::setMasterBackup1Enabled(bool b) {
	m_masterBackupPath[0].setEnabled(b);
}
void ArchivePath::setMasterBackup2Enabled(bool b) {
	m_masterBackupPath[1].setEnabled(b);
}

void ArchivePath::setDerivativeBackup1Enabled(bool b)
{
	m_derivativeBackupPath[0].setEnabled(b);
}

void ArchivePath::setDerivativeBackup2Enabled(bool b)
{
	m_derivativeBackupPath[1].setEnabled(b);
}

bool ArchivePath::isMasterBackup1Enabled() {
	return m_masterBackupPath[0].isEnabled();
}
bool ArchivePath::isMasterBackup2Enabled() {
	return m_masterBackupPath[1].isEnabled();
}

bool ArchivePath::isDerivativeBackup1Enabled()
{
	return m_derivativeBackupPath[0].isEnabled();
}

bool ArchivePath::isDerivativeBackup2Enabled()
{
	return m_derivativeBackupPath[1].isEnabled();
}


RepositoryPath& ArchivePath::getMasterBackup1() {
	return m_masterBackupPath[0];
}

RepositoryPath& ArchivePath::getMasterBackup2() {
	return m_masterBackupPath[1];
}

RepositoryPath& ArchivePath::getDerivativeBackup1() {
	return m_derivativeBackupPath[0];
}

RepositoryPath& ArchivePath::getDerivativeBackup2() {
	return m_derivativeBackupPath[1];
}

RepositoryPath& ArchivePath::getMaster() {
	return m_master;
}

RepositoryPath& ArchivePath::getDerivative() {
	return m_derivative;
}

PrimaryIndexPath& ArchivePath::getPrimaryIndex() {
	return m_primaryIndex;
}

std::string ArchivePath::getPathToHome() {
	return m_pathToHome;
}

void ArchivePath::setPathToHome(std::string &pathToHome) {
	m_pathToHome = pathToHome;
}

std::string ArchivePath::getMainDupsPath() {
	return m_mainDupsPath;
}

void ArchivePath::setMainDupsPath(std::string &mainDupsPath) {
	m_mainDupsPath = mainDupsPath;
}

std::string ArchivePath::getMainHistory() {
	return m_mainHistory;
}

void ArchivePath::setMainHistory(std::string &mainHistory) {
	m_mainHistory = mainHistory;
}


void ArchivePath::setMainMetadataPath(std::string &mainMetadataPath) {
	m_mainMetadataPath = mainMetadataPath;
}

std::string ArchivePath::getMasterPath() {
	return m_master.getRepositoryPath();
}

void ArchivePath::setMasterPath(std::string &pathToMaster) {
	m_master.setRepositoryPath(pathToMaster);
	m_master.setEnabled(true);
}
	
void ArchivePath::setPathToUserMetadata(std::string& userMetadataPath) {
	m_userMetadataPath = userMetadataPath;
}

std::string ArchivePath::getPathToUserMetadata() {
	return m_userMetadataPath;
}



std::string ArchivePath::getWorkspaceYyyymmddPath() {
	return m_workspaceYyyymmddPath;
}

std::string ArchivePath::getWorkspaceMetadataPath() {
	return m_workspaceMetadataPath;
}


std::string ArchivePath::getDerivativePath() {
	return m_derivative.getRepositoryPath();
}


void ArchivePath::setDerivativePath(std::string &path) {
	m_derivative.setRepositoryPath(path);
	m_derivative.setEnabled(true);
}


/*
static ArchivePath* ArchivePath::getMaster() const {
	return m_master;
}

void ArchivePath::setMaster(static ArchivePath* master) {
	m_master = master;
}
*/

std::string ArchivePath::getMasterCatalog() {
	return m_master.getCatalog();
}

void ArchivePath::setMasterCatalog(std::string &masterCatalog) {
	m_master.setCatalog(masterCatalog);
}

std::string ArchivePath::getIndexHistory() {
	return m_primaryIndex.getHistoryPath();
}


std::string ArchivePath::getMasterJournalPath() {
	return m_master.getJournalPath();
}

void ArchivePath::setMasterJournalPath(std::string &masterJournalPath) {
	m_master.setJournalPath(masterJournalPath);
}

std::string ArchivePath::getMasterMetadataPath() {
	return m_master.getMetadataPath();
}

void ArchivePath::setMasterMetadataPath(std::string &masterMetadataPath) {
	m_master.setMetadataPath(masterMetadataPath);
}

std::string ArchivePath::getIdxDBPath() {
	return m_master.getIdxDBPath();
}

void ArchivePath::setIdxDBPath(std::string &idxDBPath) {
	m_master.setIdxDBPath(idxDBPath);
}

} // simplearchive
