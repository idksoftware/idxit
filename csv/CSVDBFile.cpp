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

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 
#endif

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <streambuf>
#include "SAUtils.h"
#include "CSVDBFile.h"
#include "CSVArgs.h"
#include "ArchivePath.h"
#include "pathcontroller.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {


/*
IndexRow::IndexRow(MTTableSchema &tableSchema) : MTRow(tableSchema) {}
IndexRow::IndexRow(int idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
	const char *md5, const char *uuid, int version, ExifDate &date, int m_dbidx, MTTableSchema &tableSchema) : MTRow(tableSchema) {
	ExifDateTime edt(date.toString().c_str());
	columnAt(static_cast<int>(PrimaryIndexIndex::PI_SEQUENCEID_IDX)).set(idx);
	columnAt(static_cast<int>(PrimaryIndexIndex::PI_SHORTIMAGEPATH_IDX)).set(imagePath);
	columnAt(static_cast<int>(PrimaryIndexIndex::PI_FILENAME_IDX)).set(name);
	columnAt(static_cast<int>(PrimaryIndexIndex::PI_FILESIZE_IDX)).set(size);
	columnAt(static_cast<int>(PrimaryIndexIndex::PI_CRC_IDX)).set(crc);
	columnAt(static_cast<int>(PrimaryIndexIndex::PI_MD5_IDX)).set(md5);
	columnAt(static_cast<int>(PrimaryIndexIndex::PI_UUID_IDX)).set(uuid);
	columnAt(static_cast<int>(PrimaryIndexIndex::PI_VERSION_IDX)).set(version);
	columnAt(static_cast<int>(PrimaryIndexIndex::PI_DATEADDED_IDX)).set(edt);
	columnAt(static_cast<int>(PrimaryIndexIndex::PI_DATABASEID_IDX)).set(m_dbidx);
}


PrimaryIndexSchema PrimaryIndexRow::m_tableSchema;
MasterIndexSchema MasterIndexRow::m_tableSchema;
DerivativeIndexSchema DerivativeIndexRow::m_tableSchema;

PrimaryIndexRow PrimaryIndexTable::m_indexRow;
MasterIndexRow MasterIndexTable::m_indexRow;
DerivativeIndexRow DerivativeIndexTable::m_indexRow;

PrimaryIndexRow::PrimaryIndexRow() : IndexRow(m_tableSchema) {}
PrimaryIndexRow::PrimaryIndexRow(int idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
	const char *md5, const char *uuid, int version, ExifDate &date, int dbidx) : IndexRow(idx, imagePath, name, size, crc,
		md5, uuid, version, date, dbidx, m_tableSchema) {};
PrimaryIndexRow::PrimaryIndexRow(const MTRow &row) : IndexRow(row, m_tableSchema) {};

MasterIndexRow::MasterIndexRow() : IndexRow(m_tableSchema) {}
MasterIndexRow::MasterIndexRow(int idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
	const char *md5, const char *uuid, int version, ExifDate &date, int dbidx) : IndexRow(idx, imagePath, name, size, crc,
		md5, uuid, version, date, dbidx, m_tableSchema) {};
MasterIndexRow::MasterIndexRow(const MTRow &row) : IndexRow(row, m_tableSchema) {};

DerivativeIndexRow::DerivativeIndexRow() : IndexRow(m_tableSchema) {}
DerivativeIndexRow::DerivativeIndexRow(int idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
	const char *md5, const char *uuid, int version, ExifDate &date, int dbidx) : IndexRow(idx, imagePath, name, size, crc,
		md5, uuid, version, date, dbidx, m_tableSchema) {};
DerivativeIndexRow::DerivativeIndexRow(const MTRow &row) : IndexRow(row, m_tableSchema) {};
*/


ImageInfo::ImageInfo(uint64_t idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
	const char *sha, const char *uuid, int version, ExifDate &date, uint64_t dbidx)
{
	m_idx = idx;
	m_shortImagePath = imagePath;
	m_imagePath = CSVDBFile::getYear(imagePath);
	m_imagePath += '/';
	m_imagePath += imagePath;
	m_name = name;
	m_crc = crc;
	m_size = size;
	m_sha = sha;
	m_uuid = uuid;
	m_version = version;
	m_dateArchived = date;
	m_dbidx = dbidx;
}

ImageInfo::~ImageInfo() {
	//printf("ImageInfo deleted\n");
}



class IdxFileItem {
	uint64_t m_idx;
	std::string m_imagePath;
	std::string m_name;
	uint64_t m_crc;
	std::string m_sha;
	std::string m_uuid;
	uint64_t m_size;
	int m_version;
	ExifDate m_dateArchived;
	uint64_t m_dbLink;
	bool m_error{ true };
public:
	IdxFileItem(const char *text) {
		CSVArgs csvArgs(':');
		csvArgs.process(text);
		if (csvArgs.size() < 9) {
			return;
		}
		m_error = false;
		std::string idxStr = csvArgs.at(0);

		//m_idx = strtol(idxStr.c_str(), NULL, 16);
		m_idx = strtol(csvArgs.at(0).c_str(), 0, 10);
		m_imagePath = csvArgs.at(1);
		m_name = csvArgs.at(2);
		m_size = strtoll(csvArgs.at(3).c_str(), 0, 10);
		m_crc = strtoll(csvArgs.at(4).c_str(), 0, 10);
		m_sha = csvArgs.at(5);
		m_uuid = csvArgs.at(6);
		m_version = strtol(csvArgs.at(7).c_str(), 0, 10);
		std::string dateStr = csvArgs.at(8);
		ExifDate date(csvArgs.at(8).c_str());
		m_dateArchived = date;
		if (csvArgs.size() > 9) {
			// Database link id found
			m_dbLink = strtol(csvArgs.at(9).c_str(), 0, 10);
		}

	}
	IdxFileItem(uint64_t idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
					const char * sha, const char *uuid, int version, ExifDate &date, uint64_t dbLink = -1) {
		m_idx = idx;
		m_imagePath = imagePath;
		m_name = name;
		m_crc = crc;
		m_size = size;
		m_sha = sha;
		m_uuid = uuid;
		m_version = version;
		m_dateArchived = date;
		m_dbLink = dbLink;
		m_error = false;
	}

	std::unique_ptr<ImageInfo> getImageInfo() {

		std::unique_ptr<ImageInfo> imageInfo = std::make_unique<ImageInfo>(m_idx, m_imagePath.c_str(), m_name.c_str(), m_crc,
									m_size, m_sha.c_str(), m_uuid.c_str(), m_version, m_dateArchived, m_dbLink);
		return imageInfo;
	}

	uint64_t getIdx() const {
		return m_idx;
	}

	void setIdx(int idx) {
		m_idx = idx;
	}

	const std::string& getImagePath() const {
		return m_imagePath;
	}

	void setImagePath(const char *imagePath) {
		m_imagePath = imagePath;
	}

	const std::string& getName() const {
		return m_name;
	}

	void setName(const std::string& name) {
		m_name = name;
	}

	uint64_t getSize() const {
		return m_size;
	}

	uint64_t getCrc() const {
		return m_crc;
	}

	const std::string& getSha() const {
		return m_sha;
	}

	const std::string& getUuid() const {
		return m_uuid;
	}

	int getVersion() const {
		return m_version;
	}

	void setVersion(int version) {
		m_version = version;
	}

	ExifDate& getDateArchived() {
		return m_dateArchived;
	}

	uint64_t getDBLink() const {
		return m_dbLink;
	}

	bool isError() { return m_error; };
};

class IdxFile {
	bool compare(std::string c1, std::string c2);
	void sorted();
	bool insert(uint64_t idx, const char *imagePath, const char *name, uint64_t size,
			uint64_t crc, const char * sha, const char *uuid, int version, ExifDate &date, uint64_t dbIdx);

	std::shared_ptr<IdxFileItem> list[256];
	int m_last;
public:
	IdxFile() {
		m_last = 0;
		for (int i = 0; i < 256; i++) {
			list[i] = 0;
		}
	};
	/*
	virtual ~IdxFile() {
		for (int i = 0; i < 256; i++) {
			if (list[i] != 0) {
				delete list[i];
			}
		}
	};
	*/
	bool read(const char *datafile);
	bool write(const char *datafile);
	bool update(uint64_t idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
			const char *sha, const char *uuid, int version, ExifDate &date, uint64_t dbidx);
	std::shared_ptr<IdxFileItem> find(uint64_t idx);
	uint64_t findLast();

};

class MirrorIdxDB {
	std::string m_path;
	std::string m_primary;
	std::string m_backup1;
	std::string m_backup2;
	std::string m_master;
	std::string makeFolders(std::string &pasePath, uint64_t idx);

public:
	MirrorIdxDB();

	void setPath(const char *s) {
		m_path = s;
	}

	~MirrorIdxDB() {};
	bool process(unsigned int idx);
};

std::string CSVDBFile::getYear(const char *path) {
	std::string fpath = path;
	return fpath.substr(0, 4);
}

CSVDBFile::CSVDBFile() {};
	

CSVDBFile::~CSVDBFile()
{}

void CSVDBFile::setPath(const char *s) {
	m_dbpath = s;
	//m_mirrorIdxDB = std::make_unique<MirrorIdxDB>();
	//m_mirrorIdxDB->setPath(s);
}

bool CSVDBFile::insert(uint64_t idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
	const char *sha, const char *uuid, int version, ExifDate &date, uint64_t dbidx) {

	if (insert(idx, imagePath, name, size, crc, sha, uuid, version, date, m_dbpath.c_str(), dbidx) == false) {
		return false;
	}
	
	return true;
}

bool CSVDBFile::insert(uint64_t idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
	const char *sha, const char *uuid, int version, ExifDate &date, const char*rootPath, uint64_t dbIndex) {

	char hexStr[4];
	//unsigned int idx = indx - 1;
	m_data[0] = (uint16_t)idx & 0xFF;
	m_data[1] = (uint16_t)(idx >> 8) & 0xFFF;
	m_data[2] = (uint16_t)(idx >> (12 + 8)) & 0xFFF;
	std::string dbpath = rootPath;
	if (SAUtils::DirExists(dbpath.c_str()) == false) {
		return false;
	}
	//printf("%x: %x %x %x\n",idx,  m_data[2], m_data[1], m_data[0]);

	SAUtils::chartohex3(hexStr, m_data[2]);
	std::string path = dbpath + '/' + hexStr;
	//printf("%s\n",path.c_str());
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	//SAUtils::sprintf(tmppath, "%.3x.csv", m_data[1]);
	SAUtils::chartohex3(hexStr, m_data[1]);
	path = path + '/' + hexStr + ".csv";
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (SAUtils::FileExists(path.c_str()) == true) {
		if (idxFile.read(path.c_str()) == false) {
			throw std::exception();
		}

	}

	std::string imagepath = imagePath;
	size_t pos = imagepath.find_first_of('/');
	if (pos == std::string::npos) {
		if (PathController::validateYYMMDD(imagepath.c_str()) == false) {
			return false;
		}
	}
	else {
		imagepath = imagepath.substr(5, imagepath.length() - 5);
		if (PathController::validateYYMMDD(imagepath.c_str()) == false) {
			return false;
		}
	}
	if (idxFile.update(idx, imagepath.c_str(), name, size, crc, sha, uuid, version, date, dbIndex) == false) {
		return false;
	}
	if (idxFile.write(path.c_str()) == false) {
		return false;
	}
	
	return true;
}

uint64_t CSVDBFile::getMaxIndex() {
	
	char hexStr[4];
	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		return false;
	}
	// Folders
	m_data[2] = (uint16_t)getMaxDirIndex(m_dbpath);
	SAUtils::chartohex3(hexStr, m_data[2]);
	std::string path = m_dbpath + '/' + hexStr;
	if (SAUtils::DirExists(path.c_str()) == false) {
		// if empty and getMaxIndex returned zero no sequence numbers. so create
		if (m_data[2] == 0) {
			SAUtils::mkDir(path.c_str());
		}
		else {
			throw std::exception();
		}
	}
	// files
	//printf("%s\n", path.c_str());
	m_data[1] = (uint16_t)getMaxDirIndex(path);
	SAUtils::chartohex3(hexStr, m_data[1]);
	path = path + '/' + hexStr + ".csv";
	if (SAUtils::FileExists(path.c_str()) == false) {
		if (m_data[1] != 0) {
			throw std::exception();
		}
	}
	uint64_t last = -1;
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (idxFile.read(path.c_str()) == false) {
		// first one
		last = 0;
	} else {
		last = idxFile.findLast();
	}

	return last;

}

uint64_t CSVDBFile::getNextIndex(uint64_t current) {
	char hexStr[4];
	m_data[0] = (unsigned int)current & 0xFF;
	m_data[1] = (unsigned int)(current >> 8) & 0xFFF;
	m_data[2] = (unsigned int)(current >> (12 + 8)) & 0xFFF;

	SAUtils::chartohex3(hexStr, m_data[3]);
	std::string path = m_dbpath + '/' + hexStr;
	if (SAUtils::DirExists(path.c_str()) == false) {
		// if empty and getMaxIndex returned zero no sequence numbers. so create
		if (m_data[3] == 0) {
			SAUtils::mkDir(path.c_str());
		}
		else {
			throw std::exception();
		}
	}
	// files
	//printf("%s\n", path.c_str());
	m_data[2] = (uint16_t)getMaxDirIndex(path);
	SAUtils::chartohex3(hexStr, m_data[2]);
	path = path + '/' + hexStr + ".csv";
	if (SAUtils::FileExists(path.c_str()) == false) {
		if (m_data[2] != 0) {
			throw std::exception();
		}
	}
	uint64_t last = -1;
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (idxFile.read(path.c_str()) == false) {
		last = 0;
	} else {
		last = idxFile.findLast();
	}
	++last;
	return last;
}

uint64_t CSVDBFile::getNextIndex() {
	uint64_t last = getMaxIndex();
	last++;
	return last;
}

uint64_t CSVDBFile::getMaxDirIndex(std::string &path) {

	int max = 0;

	FileList_Ptr filelist = SAUtils::getFiles(path.c_str());
	for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string name = *i;
		if (!(name.compare(".")) || !(name.compare(".."))) {
			continue;
		}
		//printf("%s", name.c_str());
		int idx = strtol(name.c_str(), NULL, 16);
		if (idx > max) {
			max = idx;
		}
	}

	return max;
}

std::unique_ptr<ImageInfo> CSVDBFile::getItemAt(uint64_t idx) {
	char hexStr[4];

	m_data[0] = (unsigned int)idx & 0xFF;
	m_data[1] = (unsigned int)(idx >> 8) & 0xFFF;
	m_data[2] = (unsigned int)(idx >> (12 + 8)) & 0xFFF;

	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		throw std::exception();
	}
	
	SAUtils::chartohex3(hexStr, m_data[2]);
	std::string path = m_dbpath + '/' + hexStr;
	//printf("%s\n",path.c_str());
	if (SAUtils::DirExists(path.c_str()) == false) {
		throw std::exception();
	}
	SAUtils::chartohex3(hexStr, m_data[1]);
	path = path + '/' + hexStr + ".csv";
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (SAUtils::FileExists(path.c_str()) == false) {
		throw std::exception();
	}
	if (idxFile.read(path.c_str()) == false) {
		throw std::exception();
	}
	std::shared_ptr<IdxFileItem> item;

	if ((item = idxFile.find(idx)) == 0) {
		throw std::exception();
	}
	std::unique_ptr<ImageInfo> ii = item->getImageInfo();
	return ii;


}

uint64_t CSVDBFile::findSize(uint64_t idx) {
	char hexStr[4];

	m_data[0] = (unsigned int)idx & 0xFF;
	m_data[1] = (unsigned int)(idx >> 8) & 0xFFF;
	m_data[2] = (unsigned int)(idx >> (12 + 8)) & 0xFFF;

	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		return 0;
	}
	//printf("%x: %x %x %x\n",idx,  m_data[2], m_data[1], m_data[0]);

	
	SAUtils::chartohex3(hexStr, m_data[2]);
	std::string path = m_dbpath + '/' + hexStr;
	//printf("%s\n",path.c_str());
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			return 0;
		}
	}
	
	SAUtils::chartohex3(hexStr, m_data[1]);
	path = path + '/' + hexStr + ".csv";
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (SAUtils::FileExists(path.c_str()) == true) {
		if (idxFile.read(path.c_str()) == false) {
			return 0;
		}
		std::shared_ptr<IdxFileItem> item;

		if ((item = idxFile.find(idx)) == 0) {
			return 0;
		}
		//printf("%d ", item->getIdx());
		return item->getSize();
	}
	return -1;
}

const char* CSVDBFile::findPath(uint64_t idx) {
	char hexStr[4];

	m_data[0] = (unsigned int)idx & 0xFF;
	m_data[1] = (unsigned int)(idx >> 8) & 0xFFF;
	m_data[2] = (unsigned int)(idx >> (12 + 8)) & 0xFFF;

	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		return 0;
	}
	//printf("%x: %x %x %x\n",idx,  m_data[2], m_data[1], m_data[0]);
	SAUtils::chartohex3(hexStr, m_data[2]);
	std::string path = m_dbpath + '/' + hexStr;
	//printf("%s\n",path.c_str());
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			return 0;
		}
	}
	
	SAUtils::chartohex3(hexStr, m_data[1]);
	path = path + '/' + hexStr + ".csv";
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (SAUtils::FileExists(path.c_str()) == true) {
		if (idxFile.read(path.c_str()) == false) {
			return 0;
		}
		std::shared_ptr<IdxFileItem> item;

		if ((item = idxFile.find(idx)) == 0) {
			return 0;
		}
		//printf("%d ", item->getIdx());
		return item->getImagePath().c_str();
	}
	return 0;
}

MirrorIdxDB::MirrorIdxDB() {
	
	if (ArchivePath::isMasterEnabled() == true) {
		m_master = ArchivePath::getMaster().getIdxDBPath();
	}
	if (ArchivePath::isMasterBackup1Enabled() == true) {
		m_backup1 = ArchivePath::getMasterBackup1().getIdxDBPath();

	}
	if (ArchivePath::isMasterBackup2Enabled() == true) {
		m_backup2 = ArchivePath::getMasterBackup2().getIdxDBPath();;
	}
	
}



std::string MirrorIdxDB::makeFolders(std::string &basePath, uint64_t idx) {
	char hexStr[4];

	unsigned int data[3];
	data[0] = (unsigned int)idx & 0xFF;
	data[1] = (unsigned int)(idx >> 8) & 0xFFF;
	data[2] = (unsigned int)(idx >> (12 + 8)) & 0xFFF;
	std::string dbpath = basePath;
	if (SAUtils::DirExists(dbpath.c_str()) == false) {
		throw std::exception();
	}
	//printf("%x: %x %x %x\n",idx,  m_data[2], m_data[1], m_data[0]);
	SAUtils::chartohex3(hexStr, data[2]);
	std::string path = dbpath + '/' + hexStr;
	//printf("%s\n",path.c_str());
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	SAUtils::chartohex3(hexStr, data[1]);
	path = path + '/' + hexStr + ".csv";
	return path;
}
bool MirrorIdxDB::process(unsigned int idx) {

	std::string source = makeFolders(m_path, idx);
	/*
	if (ArchivePath::isMasterEnabled() == true) {
		std::string fullPath = makeFolders(m_Master, idx);
		if (SAUtils::copy(source.c_str(), fullPath.c_str()) == false) {
			return false;
		}
	}
	*/
	
	if (ArchivePath::isMasterBackup1Enabled() == true) {
		std::string fullPath = makeFolders(m_backup1, idx);
		if (SAUtils::copy(source.c_str(), fullPath.c_str()) == false) {
			return false;
		}
	}
	if (ArchivePath::isMasterBackup2Enabled() == true) {
		std::string fullPath = makeFolders(m_backup2, idx);
		if (SAUtils::copy(source.c_str(), fullPath.c_str()) == false) {
			return false;
		}
	}
	
	return true;
}

/********************************/

bool IdxFile::read(const char *datafile) {
	std::string text;
	m_last = -1;
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	bool res = true;
	while (std::getline(file, text)) {
		std::shared_ptr<IdxFileItem> item = std::make_shared<IdxFileItem>(text.c_str());
		if (item->isError()) {
			res = false;
		}
		uint64_t fullidx = item->getIdx();
		int fileidx = (unsigned int)fullidx & 0xFF;

		if (fileidx < 0 || fileidx > 255) {
			return false;
		}
		this->list[fileidx] = item;
		if (m_last < fileidx) {
			m_last = fileidx;
		}
	}
	file.close();
	return res;
}

bool IdxFile::write(const char *datafile) {
	std::ofstream file;
	file.open(datafile, std::ios_base::app);
	if (file.is_open() == false) {
		return false;
	}
	for (int i = 0; i < 256; i++) {
		std::shared_ptr<IdxFileItem> item = list[i];
		if (item == 0) {
			continue;
		}

		//file << std::hex  << item->getIdx() << ':' << item->getImagePath() + '\n';
		ExifDate &date = item->getDateArchived();
		std::string dateStr = date.toFileString();
		std::stringstream str;
		if (item->getDBLink() == -1) {
			str << item->getIdx() << ':' << item->getImagePath() << ':' << item->getName() << ':'
				<< item->getSize() << ':' << item->getCrc() << ':'
				<< item->getSha() << ':' << item->getUuid() << ':'
				<< item->getVersion() << ':' << dateStr.c_str() << '\n';
			file << str.str();
		}
		else {
			str << item->getIdx() << ':' << item->getImagePath() << ':' << item->getName() << ':'
				<< item->getSize() << ':' << item->getCrc() << ':'
				<< item->getSha() << ':' << item->getUuid() << ':'
				<< item->getVersion() << ':' << dateStr.c_str() << ':'
				<< item->getDBLink() << '\n';
			file << str.str();
		}
		//printf("%s", item->getImagePath().c_str());

	}
	file.close();
	return true;
}
bool IdxFile::update(uint64_t idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
		const char *sha, const char *uuid, int version, ExifDate &date, uint64_t dbidx = -1) {
	std::shared_ptr<IdxFileItem> idxItem = 0;
	if ((idxItem = find(idx)) != 0) {
		idxItem->setImagePath(imagePath);
	}
	insert(idx, imagePath, name, size, crc, sha, uuid, version, date, dbidx);
	return true;
}


bool IdxFile::insert(uint64_t idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
									const char *sha, const char *uuid, int version, ExifDate &date, uint64_t dbidx) {

	std::shared_ptr<IdxFileItem> item = std::make_shared<IdxFileItem>(idx, imagePath, name, size, crc, sha, uuid, version, date, dbidx);
	uint64_t fullidx = item->getIdx();
	int posIdx = (unsigned int)fullidx & 0xFF;
	if (posIdx < 0 || posIdx > 255) {
		return false;
	}
	if (m_last < posIdx) {
		m_last = posIdx;
	}
	this->list[posIdx] = item;
	return true;
}


std::shared_ptr<IdxFileItem> IdxFile::find(uint64_t idx) {
	uint64_t fullidx = idx;
	int posIdx = (unsigned int)fullidx & 0xFF;
	if (posIdx < 0 || posIdx > 255) {
			return 0;
	}

	return this->list[posIdx];
}

uint64_t IdxFile::findLast() {
	if (m_last == -1) {
		return -1;
	}
	std::shared_ptr<IdxFileItem> item = list[m_last];
	return item->getIdx();
}



uint64_t ImageInfo::getCrc() const {
	return m_crc;
}

const ExifDate& ImageInfo::getDate() const {
	return m_dateArchived;
}

uint64_t ImageInfo::getIdx() const {
	return m_idx;
}

const std::string& ImageInfo::getShortImagePath() const {
	return m_shortImagePath;
}

const std::string& ImageInfo::getImagePath() const {
	return m_imagePath;
}
const std::string& ImageInfo::getName() const {
	return m_name;
}
const std::string& ImageInfo::getSha() const {
	return m_sha;
}

uint64_t ImageInfo::getSize() const {
	return m_size;
}

const std::string& ImageInfo::getUuid() const {
	return m_uuid;
}

int ImageInfo::getVersion() const {
	return m_version;
}
uint64_t ImageInfo::getDBIdx() const
{
	return m_dbidx;
}
} /* namespace simplearchive */
