#include "IndexedXML.h"

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

#define DB_FILENAME			"Name"
#define DB_FILEPATH			"Path"
#define DB_CRC				"CRC"
#define DB_MD5				"MD5"
#define DB_MEDIA_TYPE		"MediaType"
#define DB_SIZE				"Size"
#define DB_MOD_TIME			"ModTime"
#define DB_CREATE_TIME		"CreateTime"




XMLWriter::XMLWriter() {
	// TODO Auto-generated constructor stub

}

XMLWriter::~XMLWriter() {
	// TODO Auto-generated destructor stub
}

std::string XMLWriter::writeTag(const char* tag, const std::string& value) {
	std::ostringstream xml;
	if (!value.empty()) {
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	}
	else {
		xml << "<" << tag << "/>\n";
	}
	return xml.str();
}

std::string XMLWriter::writeTextTag(const char* tag, const std::string& value) {
	std::ostringstream xml;
	if (!value.empty()) {
		xml << "<" << tag << "><![CDATA[" << value << "]]></" << tag << ">\n";
	}
	else {
		xml << "<" << tag << "/>\n";
	}
	return xml.str();
}


IXMLFileInfoWriter::IXMLFileInfoWriter(const char* path) {
	m_path = path;

	m_file.open(path);
	m_file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
	m_file << "<FileSet>\n";
}

IXMLFileInfoWriter::~IXMLFileInfoWriter() {
	m_file << "</FileSet>\n";
	m_file.close();
}

static const char* dateFormat = "%Y:%m:%d %H:%M:%S";

/*
[001F:015D:0008:0010:0012:0034:0008:0008]IDCFile.cpp|C:\Development\idxit\idxit|...
*/


class IXMLLine {
	std::vector<std::string> m_list;
public:
	IXMLLine() = default;
	~IXMLLine() = default;
	void add(std::string str);
	std::string process();
};

void IXMLLine::add(std::string str)
{
	m_list.push_back(str);
}

std::string IXMLLine::process()
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



 

bool IXMLFileInfoWriter::add(FileInfo& fileInfo) {


	CIDKDate modDate = fileInfo.getModTime();
	std::string mod = modDate.Format(dateFormat);
	CIDKDate createDate = fileInfo.getCreateTime();
	std::string create = createDate.Format(dateFormat);
	std::string mediaType;
	std::string crc;
	if (fileInfo.getMediaType().length() == 0) {
		mediaType = "unknown";
	}
	else {
		mediaType = fileInfo.getMediaType();
	}
	if (fileInfo.getCrc() == 0) {
		crc = "";
	}
	else {
		crc = fileInfo.getMediaType();
	}
	m_file << "<File>\n"
		<< writeTextTag(DB_FILENAME, fileInfo.getName())
		<< writeTextTag(DB_FILEPATH, fileInfo.getPath().string())
		<< writeTag(DB_CRC, crc)
		<< writeTag(DB_MD5, fileInfo.getSha256())
		
		<< writeTag(DB_MEDIA_TYPE, fileInfo.getMediaType())
		<< writeTag(DB_SIZE, std::to_string(fileInfo.getSize()))
		<< writeTag(DB_MOD_TIME, mod.c_str())
		<< writeTag(DB_CREATE_TIME, create.c_str())
		<< "</File>\n";
	return true;
}

bool IXMLFileInfoWriter::WriteHeader(IndexInfo& indexInfo)
{

	CDate date = CDate::timeNow();

	std::ofstream& file = m_file;

	file << "<IndexInfo>" << std::endl;
	file << "    <TimeDate>" << std::endl;
	file << "        <Time>" << date.getHour() << ':' << date.getMin() << ':' << date.getSec() << "</Time>" << std::endl;
	file << "        <Date>" << date.getDay() << '/' << date.getMonth() << '/' << date.getYear() << "</Date>" << std::endl;
	file << "    </TimeDate>" << std::endl;
	file << "    <Device>" << std::endl;
	file << "        <Devicename>" << indexInfo.getDevicename() << "</Devicename>" <<std::endl;
	file << "        <Hostname>" << indexInfo.getHostname() << "</Hostname>" << std::endl;
	file << "    </Device>" << std::endl;
	file << "    <Volume>" << std::endl;
	file << "        <Volume>" << indexInfo.getVolumeLetter() << "</Volume>" << std::endl;
	file << "        <VolumeName>" << indexInfo.getVolumeName() << "</VolumeName>" << std::endl;
	file << "        <VolumeGUID>" << indexInfo.getVolumeNameGID() << "</VolumeGUID>" << std::endl;
	file << "        <SerialNumnber>" << std::hex
		<< indexInfo.getDiskSerialINT() << "</SerialNumnber>" << std::endl;
	file << "        <FileSystem>" << indexInfo.getFileSystemNameBuffer() << "</FileSystem>" << std::endl;
	file << "    </Volume>" << std::endl;

	file << "    <FilePaths>" << std::endl;
	file << "        <SourcePath><![CDATA[" << indexInfo.getSourcePath() << "]]></SourcePath>" << std::endl;
	file << "        <IndexPath><![CDATA[" << indexInfo.getIdxPath() << "]]></IndexPath>" << std::endl;
	file << "    </FilePaths>" << std::endl;

	file << "    <Filters>" << std::endl;
	file << "        <IncludeGroupFilter>" << std::endl;
	file << "            <FilterOn>" << ((indexInfo.getIncFileFilterOn()) ? "On" : "Off") << "</FilterOn>" << std::endl;
	if (indexInfo.getIncFileFilterOn() == true) {

		for (auto i : indexInfo.getIncFileFilter()) {
			file << "                <Ext>" << i << "</Ext>" << std::endl;
		}
	}
	file << "        </IncludeGroupFilter>" << std::endl;
	file << "        <ExcludeGroupFilter>" << std::endl;
	file << "            <FilterOn>" << ((indexInfo.getExcFileFilterOn()) ? "On" : "Off") << "</FilterOn>" << std::endl;
	if (indexInfo.getExcFileFilterOn() == true) {
		for (auto i : indexInfo.getExcFileFilter()) {
			file << "                <Ext>" << i << std::endl;
		}
	}
	file << "        </ExcludeGroupFilter>" << std::endl;

	file << "        <ExcludeFolderFilter>" << std::endl;
	file << "            <FilterOn>" << ((indexInfo.getExcFolderFilterOn()) ? "On" : "Off") << "</FilterOn>" << std::endl;

	if (indexInfo.getExcFolderFilterOn() == true) {
		for (auto i : indexInfo.getExcFolderFilter()) {
			file << "<Folder>" << i << std::endl;
		}
	}
	file << "        </ExcludeFolderFilter>" << std::endl;

	file << "    <Ignore>" << std::endl;
	file << "        <SystemIgnoreList>" << std::endl;
	file << "            <IgnoreOn>" << ((indexInfo.getSysIgnoreOn()) ? "On" : "Off") << "</IgnoreOn>" << std::endl;

	if (indexInfo.getSysIgnoreOn() == true) {
		for (auto i : indexInfo.getSysIgnoreList()) {
			file << "                <IgnoreItem>" << i << "</IgnoreItem>" << std::endl;
		}
	}
	file << "        </SystemIgnoreList>" << std::endl;

	file << "        <UserSystemIgnoreList>" << std::endl;
	file << "            <IgnoreOn>" << ((indexInfo.getUsersysIgnoreOn()) ? "On" : "Off") << "</IgnoreOn>" << std::endl;

	if (indexInfo.getUsersysIgnoreOn() == true) {
		for (auto i : indexInfo.getUsersysIgnoreList()) {
			file << "                <IgnoreItem>" << i << "</IgnoreItem>" << std::endl;
		}
	}
	file << "        </UserSystemIgnoreList>" << std::endl;

	file << "        <UserIgnoreList>" << std::endl;
	file << "            <IgnoreOn>" << ((indexInfo.getUserIgnoreOn()) ? "On" : "Off") << "</IgnoreOn>" << std::endl;

	if (indexInfo.getUserIgnoreOn() == true) {
		for (auto i : indexInfo.getUserIgnoreList()) {
			file << "                <IgnoreItem>" << i << std::endl;
		}
	}
	file << "        </UserIgnoreList>" << std::endl;
	file << "    </Ignore>" << std::endl;
	file << "    </Filters>" << std::endl;
	file << "    <Misc>" << std::endl;
	file << "        <ScanHidden>" << ((indexInfo.getScanHidden()) ? "On" : "Off") << "</ScanHidden>" << std::endl;
	file << "    </Misc>" << std::endl;
	
	file << "<Files>" << std::endl;

	return true;
}

bool IXMLFileInfoWriter ::WriteFooter(IndexInfo& indexInfo)
{
	std::ofstream& file = m_file;
	
	file << "</Files>" << std::endl;
	file << "<Summary>" << std::endl;
	file << "        <FilesProcessed>" << std::dec << indexInfo.getNoFilesCompleted() << "</FilesProcessed>" << std::endl;
	file << "        <FoldersProcessed>" << indexInfo.getNoFoldersCompleted() << "</FoldersProcessed>" << std::endl;
	file << "        <BytesProcessed>" << indexInfo.getSizeCompleted() << " (" << SAUtils::humanSize(indexInfo.getSizeCompleted()) << ')' << "</BytesProcessed>" << std::endl;

	file << "        <FilesIncluded>" << indexInfo.getNoIncluded() << "</FilesIncluded>" << std::endl;
	file << "        <FilesExcluded>" << indexInfo.getNoExcluded() << "</FilesExcluded>" << std::endl;

	bool completed = true;
	if (indexInfo.getNoFilesToBeCompleted() != indexInfo.getNoFilesCompleted())
	{
		file << "        <FilesNotProcessed>" << (indexInfo.getNoFilesToBeCompleted() - indexInfo.getNoFilesCompleted()) << "</FilesNotProcessed>" << std::endl;
		completed = false;
	}
	if (indexInfo.getNoFoldersToBeCompleted() != indexInfo.getNoFoldersCompleted())
	{
		file << "        <FolderssNotProcessed>" << (indexInfo.getNoFoldersToBeCompleted() - indexInfo.getNoFoldersCompleted()) << "</FolderssNotProcessed>" << std::endl;
		completed = false;
	}
	if (indexInfo.getSizeToBeCompleted() != indexInfo.getSizeCompleted())
	{
		file << "        <BytesNotProcessed>" << (indexInfo.getSizeToBeCompleted() - indexInfo.getSizeCompleted()) << " (" << SAUtils::humanSize((indexInfo.getSizeToBeCompleted() - indexInfo.getSizeCompleted())) << ')' << "</BytesNotProcessed>" << std::endl;
		completed = false;
	}

	file << "        <Scan>" << (completed ? "Completed Ok" : "Failed") << "</Scan>" << std::endl;
	file << "</Summary>" << std::endl;
	file << "</IndexInfo>" << std::endl;
	return true;
}

IXMLLineReader::IXMLLineReader(const char* line)
{
	m_line = line;
}

IXMLLineReader::~IXMLLineReader()
{
}

int IXMLLineReader::getSize()
{
	if (m_size == -1) {
		m_offsetSize = m_line.find_first_of(']');
		int temp = m_offsetSize;
		//temp -= 1;
		m_size = temp / 5;
	}
	return m_size;
}

std::string IXMLLineReader::getAt(int idx)
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
