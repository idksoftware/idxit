#include <windows.h>
#include "ScanVisitor.h"
#include "IndexInfo.h"

uint64_t ScanVisitor::m_files = 0;
uint64_t ScanVisitor::m_folders = 0;
uint64_t ScanVisitor::m_size = 0;

uint64_t ScanVisitor::m_included = 0;
uint64_t ScanVisitor::m_excluded = 0;

bool ScanVisitor::m_excFileFilterOn;
bool ScanVisitor::m_excFolderFilterOn;

bool ScanVisitor::m_incFileFilterOn;
bool ScanVisitor::m_incFolderFilterOn;

bool ScanVisitor::m_scanHidden;

bool ScanVisitor::m_sysIgnoreOn;
bool ScanVisitor::m_usersysIgnoreOn;

std::string ScanVisitor::m_sourcePath;


bool ScanVisitor::VolumeInformation(std::ofstream& file)
{
	std::string volume = "D:\\";
	char volumeName[MAX_PATH + 1];
	char volumeNameGID[MAX_PATH + 1];
	char fileSystemNameBuffer[MAX_PATH + 1];
	DWORD disk_serialINT;

	GetVolumeNameForVolumeMountPoint(volume.c_str(), volumeNameGID, MAX_PATH);



	file << "    [Volume]" << std::endl;
	if (!GetVolumeInformation(volume.c_str(), volumeName, MAX_PATH, &disk_serialINT, NULL,
		NULL, fileSystemNameBuffer, MAX_PATH))
	{
		file << "        Failed: " << GetLastError() << std::endl;
		return 1;
	}
	file << "        Volume: " << volume << std::endl;
	file << "        VolumeName: " << volumeName << std::endl;
	file << "        VolumeGUID: " << volumeNameGID << std::endl;
	file << "        SerialNumnber: " << std::hex
		<< disk_serialINT << std::endl;
	file << "        FileSystem: " << fileSystemNameBuffer << std::endl;

	return 0;
}


bool ScanVisitor::WriteHeader()
{

	m_indexInfo.setSourcePath(m_sourcePath);
	//m_indexInfo.setIdxPath(m_idxPath);
	

	m_indexInfo.setIncFileFilterOn(m_incFileFilterOn);
	m_indexInfo.setIncFileFilter(*m_incFileFilter);
	m_indexInfo.setExcFileFilterOn(m_excFileFilterOn);
	m_indexInfo.setExcFileFilter(*m_excFileFilter);

	m_indexInfo.setIncFolderFilterOn(m_incFolderFilterOn);
	m_indexInfo.setIncFolderFilter(*m_incFolderFilter);
	m_indexInfo.setExcFolderFilterOn(m_excFolderFilterOn);
	m_indexInfo.setExcFolderFilter(*m_excFolderFilter);

	m_indexInfo.setSysIgnoreOn(m_sysIgnoreOn);
	m_indexInfo.setSysIgnoreList(*m_sysIgnoreList);

	m_indexInfo.setUsersysIgnoreOn(m_usersysIgnoreOn);
	m_indexInfo.setUsersysIgnoreList(*m_usersysIgnoreList);

	//m_indexInfo.setUserIgnoreOn(m_userIgnoreOn);
	m_indexInfo.setUserIgnoreList(*m_userIgnoreList);

	m_indexInfo.setScanHidden(m_scanHidden);
	
	m_indexInfo.setFilesToBeCompleted(m_files);
	m_indexInfo.setFoldersToBeCompleted(m_folders);
	m_indexInfo.setSizeToBeCompleted(m_size);

	m_indexInfo.setIncluded(m_included);
	m_indexInfo.setExcluded(m_excluded);
	
	m_icsvFileInfoWriter->WriteHeader(m_indexInfo);

	return true;
}

bool ScanVisitor::WriteFooter()
{
	m_icsvFileInfoWriter->WriteFooter(m_indexInfo);
	return true;
}

//std::shared_ptr<XMLFileInfoWriter> ScanVisitor::m_xmlFileInfoWriter = nullptr;
//std::string ScanVisitor::m_sourcePath;
//std::string ScanVisitor::m_idxPath;
std::shared_ptr<ICSVFileInfoWriter> ScanVisitor::m_icsvFileInfoWriter = nullptr;
std::shared_ptr<XMLFileInfoWriter> ScanVisitor::m_xmlFileInfoWriter = nullptr;
std::shared_ptr<Storage> ScanVisitor::m_storage = nullptr;
std::shared_ptr<FileFilter> ScanVisitor::m_incFileFilter = nullptr;
//bool ScanVisitor::m_incFileFilterOn = false;
std::shared_ptr<FolderFilter> ScanVisitor::m_incFolderFilter = nullptr;
//bool ScanVisitor::m_incFolderFilterOn = false;
std::shared_ptr<FileFilter> ScanVisitor::m_excFileFilter = nullptr;
//bool ScanVisitor::m_excFileFilterOn = false;
std::shared_ptr<FolderFilter> ScanVisitor::m_excFolderFilter = nullptr;
//bool ScanVisitor::m_excFolderFilterOn = false;
std::shared_ptr<IgnoreList> ScanVisitor::m_sysIgnoreList = nullptr;
//bool ScanVisitor::m_sysIgnoreOn = true;
std::shared_ptr<IgnoreList> ScanVisitor::m_usersysIgnoreList = nullptr;
//bool ScanVisitor::m_usersysIgnoreOn = true;
std::shared_ptr<IgnoreList> ScanVisitor::m_userIgnoreList = nullptr;
//bool ScanVisitor::m_userIgnoreOn = true;

//bool ScanVisitor::m_scanHidden = false;
IndexInfo ScanVisitor::m_indexInfo;