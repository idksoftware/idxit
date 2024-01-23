#pragma once

#include <string>
#include "Filters.h"
#include "IgnoreList.h"

class IndexInfo
{

	std::string m_volumeName;
	std::string m_volumeNameGID;
	std::string m_disk_serialINT;
	std::string m_fileSystemNameBuffer;
	std::string m_devicename;
	std::string m_hostname;
	std::string m_sourcePath;
	std::string m_idxPath;

	FileFilter *m_incFileFilter;
	FolderFilter *m_incFolderFilter;
	bool m_incFileFilterOn;
	bool m_incFolderFilterOn;
	FileFilter *m_excFileFilter;
	FolderFilter *m_excFolderFilter;
	bool m_excFileFilterOn;
	bool m_excFolderFilterOn;
	IgnoreList *m_sysIgnoreList;
	IgnoreList *m_usersysIgnoreList;
	IgnoreList *m_userIgnoreList;
	bool m_sysIgnoreOn;
	bool m_usersysIgnoreOn;
	bool m_userIgnoreOn;
	bool m_scanHidden;

	uint64_t m_filesToBeCompleted;
	uint64_t m_foldersToBeCompleted;
	uint64_t m_sizeToBeCompleted;

	uint64_t m_filesCompleted;
	uint64_t m_foldersCompleted;
	uint64_t m_sizeCompleted;

	uint64_t m_included;
	uint64_t m_excluded;


public:

	IndexInfo() = default;
	~IndexInfo() = default;

	void setSourcePath(std::string s) { m_sourcePath = s; };
	void setIdxPath(std::string s) { m_idxPath = s; };

	void setVolumeName(std::string s) { m_volumeName = s; };
	void setVolumeNameGID(std::string s) { m_volumeNameGID = s; };
	void setDiskSerialINT(std::string s) { m_disk_serialINT = s; };
	void setFileSystemNameBuffer(std::string s) { m_fileSystemNameBuffer = s; };

	void setDevicename(std::string s) { m_devicename = s; };
	void setHostname(std::string s) { m_hostname = s; };

	void setIncFileFilter(FileFilter& v) { m_incFileFilter = &v; };
	void setIncFolderFilter(FolderFilter& v) { m_incFolderFilter = &v; };

	void setIncFileFilterOn(bool b) { m_incFileFilterOn = b; };
	void setIncFolderFilterOn(bool b) { m_incFolderFilterOn = b; };

	void setExcFileFilter(FileFilter& v) { m_excFileFilter = &v; };
	void setExcFolderFilter(FolderFilter& v) { m_excFolderFilter  = &v; };

	void setExcFileFilterOn(bool b) { m_excFileFilterOn = b; };
	void setExcFolderFilterOn(bool b) { m_excFolderFilterOn = b; };

	void setSysIgnoreList(IgnoreList& v) { m_sysIgnoreList = &v; };
	void setUsersysIgnoreList(IgnoreList& v) { m_usersysIgnoreList = &v; };
	void setUserIgnoreList(IgnoreList& v) { m_userIgnoreList = &v; };

	void setSysIgnoreOn(bool b) { m_sysIgnoreOn = b; };
	void setUsersysIgnoreOn(bool b) { m_usersysIgnoreOn; };
	void setUserIgnoreOn(bool b) { m_userIgnoreOn = b; };

	void setScanHidden(bool b) { m_scanHidden = b; };

	void setFilesToBeCompleted(uint64_t v) { m_filesToBeCompleted = v; };
	void setFoldersToBeCompleted(uint64_t v) { m_foldersToBeCompleted = v; };
	void setSizeToBeCompleted(uint64_t v) { m_sizeToBeCompleted = v; };

	void setFilesCompleted(uint64_t v) { m_filesCompleted = v; };
	void setFoldersCompleted(uint64_t v) { m_foldersCompleted = v; };
	void setSizeCompleted(uint64_t v) { m_sizeCompleted = v; };
	
	void setIncluded(uint64_t v) { m_included = v; };
	void setExcluded(uint64_t v) { m_excluded = v; };

	std::string getSourcePath() { return m_sourcePath; };
	std::string getIdxPath() { return m_idxPath; };

	std::string getVolumeName() { return m_volumeName; };
	std::string getVolumeNameGID() { return m_volumeNameGID; };
	std::string getDiskSerialINT() { return m_disk_serialINT; };
	std::string getFileSystemNameBuffer() { return m_fileSystemNameBuffer; };

	std::string getDevicename() { return m_devicename; };
	std::string getHostname() { return m_hostname; };

	FileFilter& getIncFileFilter() { return *m_incFileFilter; };
	FolderFilter& getIncFolderFilter() {  return *m_incFolderFilter; };
	bool getIncFileFilterOn() {  return m_incFileFilterOn; };
	bool getIncFolderFilterOn() {  return m_incFolderFilterOn; };
	FileFilter& getExcFileFilter() {  return *m_excFileFilter; };
	FolderFilter& getExcFolderFilter() {  return *m_excFolderFilter; };
	bool getExcFileFilterOn() {  return m_excFileFilterOn; };
	bool getExcFolderFilterOn() {  return m_excFolderFilterOn; };
	IgnoreList& getSysIgnoreList() {  return *m_sysIgnoreList; };
	IgnoreList& getUsersysIgnoreList() {  return *m_usersysIgnoreList; };
	IgnoreList& getUserIgnoreList() {  return *m_userIgnoreList; };
	bool getSysIgnoreOn() {  return m_sysIgnoreOn; };
	bool getUsersysIgnoreOn() {  return m_usersysIgnoreOn; };
	bool getUserIgnoreOn() {  return m_userIgnoreOn; };
	bool getScanHidden() {  return m_scanHidden; };

	uint64_t getNoFilesToBeCompleted() { return m_filesToBeCompleted; };
	uint64_t getNoFoldersToBeCompleted() {  return m_foldersToBeCompleted; };
	uint64_t getSizeToBeCompleted() { return m_sizeToBeCompleted; };

	uint64_t getNoFilesCompleted() { return m_filesCompleted; };
	uint64_t getNoFoldersCompleted() { return m_foldersCompleted; };
	uint64_t getSizeCompleted() { return m_sizeCompleted; };


	uint64_t getNoIncluded() {  return m_included; };
	uint64_t getNoExcluded() {  return m_excluded; };

};

