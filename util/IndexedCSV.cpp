#include "IndexedCSV.h"

#include "CDate.h"
#include "FileInfo.h"
#include "IndexInfo.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

/*
#define DB_FILENAME			"Name"
#define DB_FILEPATH			"Path"
#define DB_CRC				"CRC"
#define DB_MD5				"MD5"
#define DB_MEDIA_TYPE		"MediaType"
#define DB_SIZE				"Size"
#define DB_MOD_TIME			"ModTime"
#define DB_CREATE_TIME		"CreateTime"
*/










ICSVFileInfoWriter::ICSVFileInfoWriter(const char* path) {
	m_path = path;

	m_file.open(path);
	
}

ICSVFileInfoWriter::~ICSVFileInfoWriter() {
	
	m_file.close();
}

static const char* dateFormat = "%Y:%m:%d %H:%M:%S";

/*
[001F:015D:0008:0010:0012:0034:0008:0008]IDCFile.cpp|C:\Development\idxit\idxit|...
*/


class ICSVLine {
	std::vector<std::string> m_list;
public:
	ICSVLine() = default;
	~ICSVLine() = default;
	void add(std::string str);
	std::string process();
};

void ICSVLine::add(std::string str)
{
	m_list.push_back(str);
}

std::string ICSVLine::process()
{
	bool first = true;
	std::ostringstream text;
	int index_offset = 2 + (5 * m_list.size() - 1);
	int body_len = 0;
	int item_count = 0;
	text << "[";
	for (auto i : m_list) {
		int len = i.length();
		if (first) {
			first = false;
			text << std::setw(4) << std::setfill('0') << std::hex << index_offset;
		}
		else {
			text << ':';
			text << std::setw(4) << std::setfill('0') << std::hex << index_offset + body_len + item_count;	
		}
		body_len += len;
		item_count++;
	}
	text << "]";
	first = true;
	for (auto i : m_list) {
		if (first) {
			first = false;
		}
		else {
			text << '|';
		}
		text << i;
	}
	return text.str();
}


bool ICSVFileInfoWriter::add(FileInfo& fileInfo) {


	CIDKDate modDate = fileInfo.getModTime();
	std::string mod = modDate.Format(dateFormat);
	CIDKDate createDate = fileInfo.getCreateTime();
	std::string create = createDate.Format(dateFormat);
	/*
	m_xmlFile << "<File>\n"
		<< writeTag(DB_FILENAME, fileInfo.getName())
		<< writeTag(DB_FILEPATH, fileInfo.getPath().string())
		<< writeTag(DB_CRC, std::to_string(fileInfo.getCrc()))
		<< writeTag(DB_MD5, fileInfo.getSha256())
		<< writeTag(DB_MEDIA_TYPE, fileInfo.getMediaType())
		<< writeTag(DB_SIZE, std::to_string(fileInfo.getSize()))
		<< writeTag(DB_MOD_TIME, mod.c_str())
		<< writeTag(DB_CREATE_TIME, create.c_str())
		<< "</File>\n";
	*/
	ICSVLine icsvLine;
	icsvLine.add(fileInfo.getName());
	icsvLine.add(fileInfo.getPath().string());
	icsvLine.add(std::to_string(fileInfo.getCrc()));
	icsvLine.add(fileInfo.getSha256());
	if (fileInfo.getMediaType().length() == 0) {
		icsvLine.add("unknown");
	}
	else {
		icsvLine.add(fileInfo.getMediaType());
	}
	icsvLine.add(std::to_string(fileInfo.getSize()));
	icsvLine.add(mod.c_str());
	icsvLine.add(create.c_str());
	std::string out = icsvLine.process();
	/*
	printf("%s\n", out.c_str());
	ICSVLineReader icsvLineReader(out.c_str());
	printf("%s\n", icsvLineReader.getAt(0).c_str());
	printf("%s\n", icsvLineReader.getAt(1).c_str());
	printf("%s\n", icsvLineReader.getAt(2).c_str());
	printf("%s\n", icsvLineReader.getAt(3).c_str());
	printf("%s\n", icsvLineReader.getAt(4).c_str());
	printf("%s\n", icsvLineReader.getAt(5).c_str());
	printf("%s\n", icsvLineReader.getAt(6).c_str());
	printf("%s\n", icsvLineReader.getAt(7).c_str());
	printf("%s\n", icsvLineReader.getAt(8).c_str());
	*/
	m_file << out << std::endl;
	return true;
}

bool ICSVFileInfoWriter::WriteHeader(IndexInfo& indexInfo)
{

	CDate date = CDate::timeNow();

	


	std::ofstream& file = m_file;

	file << "[IndexInfo]" << std::endl;
	file << "    [TimeDate]" << std::endl;
	file << "        Time: " << date.getHour() << ':' << date.getMin() << ':' << date.getSec() << std::endl;
	file << "        Date: " << date.getDay() << '/' << date.getMonth() << '/' << date.getYear() << std::endl;
	file << "    [Device]" << std::endl;
	file << "        Devicename: " << indexInfo.getDevicename() << std::endl;
	file << "        Hostname: " << indexInfo.getHostname() << std::endl;
	file << "    [Volume]" << std::endl;
	file << "        Volume: " << indexInfo.getVolumeName() << std::endl;
	file << "        VolumeName: " << indexInfo.getVolumeName() << std::endl;
	file << "        VolumeGUID: " << indexInfo.getVolumeNameGID() << std::endl;
	file << "        SerialNumnber: " << std::hex
		<< indexInfo.getDiskSerialINT() << std::endl;
	file << "        FileSystem: " << indexInfo.getFileSystemNameBuffer() << std::endl;

	file << "    [FilePaths]" << std::endl;
	file << "        SourcePath: " << indexInfo.getSourcePath() << std::endl;
	file << "        IndexPath:  " << indexInfo.getIdxPath() << std::endl;
	file << "    [Filters]" << std::endl;
	file << "        [IncludeGroupFilter]" << std::endl;
	file << "            FilterOn: " << ((indexInfo.getIncFileFilterOn()) ? "On" : "Off") << std::endl;
	if (indexInfo.getIncFileFilterOn() == true) {

		for (auto i : indexInfo.getIncFileFilter()) {
			file << "                Ext: " << i << std::endl;
		}
	}
	file << "        [ExcludeGroupFilter]" << std::endl;
	file << "            FilterOn: " << ((indexInfo.getExcFileFilterOn()) ? "On" : "Off") << std::endl;
	if (indexInfo.getExcFileFilterOn() == true) {
		for (auto i : indexInfo.getExcFileFilter()) {
			file << "                Ext: " << i << std::endl;
		}
	}
	file << "        [ExcludeFolderFilter]" << std::endl;
	file << "            FilterOn: " << ((indexInfo.getExcFolderFilterOn()) ? "On" : "Off") << std::endl;

	if (indexInfo.getExcFolderFilterOn() == true) {
		for (auto i : indexInfo.getExcFolderFilter()) {
			file << "Folder: " << i << std::endl;
		}
	}
	file << "    [Ignore]" << std::endl;
	file << "        [SystemIgnoreList]" << std::endl;
	file << "            IgnoreOn: " << ((indexInfo.getSysIgnoreOn()) ? "On" : "Off") << std::endl;

	if (indexInfo.getSysIgnoreOn() == true) {
		for (auto i : indexInfo.getSysIgnoreList()) {
			file << "                IgnoreItem: " << i << std::endl;
		}
	}

	file << "        [UserSystemIgnoreList]" << std::endl;
	file << "            IgnoreOn: " << ((indexInfo.getUsersysIgnoreOn()) ? "On" : "Off") << std::endl;

	if (indexInfo.getUsersysIgnoreOn() == true) {
		for (auto i : indexInfo.getUsersysIgnoreList()) {
			file << "                IgnoreItem: " << i << std::endl;
		}
	}

	file << "        [UserIgnoreList]" << std::endl;
	file << "            IgnoreOn: " << ((indexInfo.getUserIgnoreOn()) ? "On" : "Off") << std::endl;

	if (indexInfo.getUserIgnoreOn() == true) {
		for (auto i : indexInfo.getUserIgnoreList()) {
			file << "                IgnoreItem: " << i << std::endl;
		}
	}
	file << "    [Misc]" << std::endl;
	file << "        ScanHidden: " << ((indexInfo.getScanHidden()) ? "On" : "Off") << std::endl;
	file << "[Files]" << std::endl;

	return true;
}

bool ICSVFileInfoWriter::WriteFooter(IndexInfo& indexInfo)
{
	std::ofstream& file = m_file;

	file << "[Summary]" << std::endl;
	file << "        FilesProcessed: " << std::dec << indexInfo.getNoFilesCompleted() << std::endl;
	file << "        FoldersProcessed: " << indexInfo.getNoFoldersCompleted() << std::endl;
	file << "        BytesProcessed: " << indexInfo.getSizeCompleted() << " (" << SAUtils::humanSize(indexInfo.getSizeCompleted()) << ')' << std::endl;

	file << "        FilesIncluded: " << indexInfo.getNoIncluded() << std::endl;
	file << "        FilesExcluded: " << indexInfo.getNoExcluded() << std::endl;

	bool completed = true;
	if (indexInfo.getNoFilesToBeCompleted() != indexInfo.getNoFilesCompleted())
	{
		file << "        FilesNotProcessed: " << (indexInfo.getNoFilesToBeCompleted() - indexInfo.getNoFilesCompleted()) << std::endl;
		completed = false;
	}
	if (indexInfo.getNoFoldersToBeCompleted() != indexInfo.getNoFoldersCompleted())
	{
		file << "        FolderssNotProcessed: " << (indexInfo.getNoFoldersToBeCompleted() - indexInfo.getNoFoldersCompleted()) << std::endl;
		completed = false;
	}
	if (indexInfo.getSizeToBeCompleted() != indexInfo.getSizeCompleted())
	{
		file << "        BytesNotProcessed: " << (indexInfo.getSizeToBeCompleted() - indexInfo.getSizeCompleted()) << " (" << SAUtils::humanSize((indexInfo.getSizeToBeCompleted() - indexInfo.getSizeCompleted())) << ')' << std::endl;
		completed = false;
	}

	file << "        Scan: " << (completed ? "Completed Ok" : "Failed") << std::endl;

	return true;
}

ICSVLineReader::ICSVLineReader(const char* line)
{
	m_line = line;
}

ICSVLineReader::~ICSVLineReader()
{
}

int ICSVLineReader::getSize()
{
	if (m_size == -1) {
		m_offsetSize = m_line.find_first_of(']');
		int temp = m_offsetSize;
		//temp -= 1;
		m_size = temp / 5;
	}
	return m_size;
}

std::string ICSVLineReader::getAt(int idx)
{
	int size = getSize();
	if (idx > size - 1) {
		return std::string();
	}
	int indexPos = 1 + (idx * 5);
	std::string indexOffSet = m_line.substr(indexPos, 4);
	int offset = std::stoi(indexOffSet, 0, 16);
	int endOffSet = m_line.find_first_of('|', offset);
	std::string itemStr = m_line.substr(offset, endOffSet - offset);
	return itemStr;
}
