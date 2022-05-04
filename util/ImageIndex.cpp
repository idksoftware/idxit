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


#include <exception>
#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <streambuf>
#include <algorithm>
#include <filesystem>

#include "Crc64.h"
#include "sha256.h"


#include "cport.h"

#include "ImageIndex.h"
#include "Fileinfo.h"
//#include "SAUtils.h"
#include <stdio.h>

#define BOOST 1

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif



#define TO_HEX(i) (i <= 9 ? '0' + i : 'a' - 10 + i)

	void chartohex3(char *buffer, unsigned short x) {
		buffer[0] = TO_HEX(((x & 0x0F00) >> 8));
		buffer[1] = TO_HEX(((x & 0x00F0) >> 4));
		buffer[2] = TO_HEX((x & 0x000F));
		buffer[3] = '\0';
	}

	void chartohex2(char *buffer, unsigned char x) {
		buffer[0] = TO_HEX(((x & 0x00F0) >> 4));
		buffer[1] = TO_HEX((x & 0x000F));
		buffer[2] = '\0';
	}


	void mkDir(const char *path) {
#ifdef BOOST
		std::filesystem::path data_dir(path);
		if (std::filesystem::is_directory(path) == false) {
			if (std::filesystem::create_directory(path) == false) {
				throw std::exception();
			}
			throw std::exception();
		}
#else
#ifdef _WIN32
		if (_mkdir(path) != 0) {
			throw std::exception();
		}
#else
		mode_t mode = 0777;
		if (mkdir(path, mode) != 0) {
			throw std::exception();
		}
#endif
#endif
	}

	void ifNotDirectoryFail(const char *path, const char *message) {
		std::filesystem::path data_dir(path);
		if (std::filesystem::is_directory(data_dir) == false) {
			throw std::exception(message);
		}
	}

	void makeDirectoryIfNotExists(const char *path) {
		std::filesystem::path data_dir(path);
		if (std::filesystem::is_directory(data_dir) == false) {
			if (std::filesystem::create_directory(data_dir) == false) {
				throw std::exception();
			}
		}
	}

class CacheItem {
	unsigned long m_crc;
	std::string m_sha256;
	std::string m_path;
public:
	CacheItem(unsigned long crc, const char *sha256, const char *path) {
		m_crc = crc;
		m_sha256 = sha256;
		m_path = path;
	}
	unsigned long getCRC() { return m_crc; };
	std::string getm_SHA256() { return m_sha256; };
	std::string getPath() { return m_path; };
};

struct IdxCompare
{
	const std::vector<CacheItem>& target;

	IdxCompare(const std::vector<CacheItem>& target) : target(target) {}

	bool operator()(CacheItem &a, CacheItem &b) const {
		//auto ia = target[a];
		//auto ib = target[b];
		return a.getCRC() < b.getCRC();
	}
};

bool comp(CacheItem a, CacheItem b) { return (a.getCRC() < b.getCRC()); }

class DupCache : public std::vector<CacheItem> {
	
public:
	bool insert(unsigned long crc, const char *md5, const char *path) {
		CacheItem x(crc, md5, path);
		push_back(x);
		std::sort(begin(), end(), IdxCompare(*this));

		//std::cout << "\nDupCache" << '\n';
		//for (size_t i = 0; i < size(); ++i)
			//std::cout << (*this)[i].getCRC() << '\n';

		return true;
	}

	bool isDup(unsigned long crc) {
		CacheItem x(crc, "", "");
		if (std::binary_search(begin(), end(), x, comp)) {
			//std::cout << "Dup found!\n";
			return true;
		} else {
			//std::cout << "Dup not found.\n";
			return false;
		}
		return true;
	}

};

class MirrorImageIndex {
	std::string m_path;
	std::string m_primary;
	std::string m_backup1;
	std::string m_backup2;
	std::string m_Master;
	std::string makeFolders(std::string &pasePath, unsigned long crc);

public:
	MirrorImageIndex(const char *rootPath);
	~MirrorImageIndex() {};
	bool init(const char *rootPath);
	bool process(unsigned long crc);
};


class DataContainer : public std::vector<std::string> {

public:
	~DataContainer() {
		
	}
};

class DupDataFile {
	std::unique_ptr<DataContainer> m_dataContainer;
	bool compare(std::string c1, std::string c2);
	void sorted();
	// Used to write file
	std::string m_filePath;
public:
	DupDataFile();
	virtual ~DupDataFile();
	bool read(const char *datafile);
	bool write();
	bool write(const char *datafile);
	bool add(const char *name, unsigned long crc, const char *md5, const char *path, const char *version, const char *orginal);
	int find(unsigned long crc);
	std::string findData(unsigned long crc);
	bool IsEmpty() {
		return m_dataContainer->empty();
	}
	void insert(unsigned long crc, std::string &row);
	std::string& getAt(int pos);
	void putAt(int pos, std::string item);
};

DupDataFile::DupDataFile() : m_dataContainer(std::make_unique<DataContainer>()) {}

DupDataFile::~DupDataFile() {
}

bool DupDataFile::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	m_filePath = datafile;
	while (file.getline(text, 100)) {
		m_dataContainer->push_back(text);
	}
	file.close();

	return true;
}

bool DupDataFile::write() {
	return write(m_filePath.c_str());
}

bool DupDataFile::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<std::string>::iterator i = m_dataContainer->begin(); i != m_dataContainer->end(); i++) {
		std::string name = *i;
		file << name + '\n';
		//printf("%s", name.c_str());
	}
	file.close();
	return true;
}
bool DupDataFile::add(const char *name, unsigned long crc, const char *md5, const char *path, const char *version, const char *orginal) {
	//char c_crc[9];
	std::string crcStr; // = c_crc;
	sprintf_p(crcStr, "%.8x", crc);
	//std::string crcStr = c_crc;
	std::string nameStr = name;
	std::string md5Str = md5;
	std::string row(crcStr + ':' + nameStr + ':' + md5Str + ':' + path + ':' + version + ':' + orginal);
	if (find(crc) != -1) {
		return false; // found
	}
	insert(crc, row);
	return true;
}

std::string& DupDataFile::getAt(int pos) {
	return m_dataContainer->at(pos);
}

void DupDataFile::putAt(int pos, std::string item) {
	(*m_dataContainer)[pos] = item;
}

void DupDataFile::insert(unsigned long crc, std::string& row) {
	bool found = false;
	int pos = 0;
	for (std::vector<std::string>::iterator i = m_dataContainer->begin(); i != m_dataContainer->end(); i++) {
		std::string data = *i;
		//printf("%s", data.c_str());
		size_t delim1 = data.find_first_of(':');
		std::string crcStr = data.substr(0,delim1);
		unsigned long newcrc = std::stoul(crcStr.c_str(), NULL, 16);
		if (newcrc > crc) {
			found = true;
			break;
		}
		pos++;
	}
	if (!found) {
		m_dataContainer->push_back(row);
	} else {
		m_dataContainer->insert(m_dataContainer->begin()+(pos), row);
	}

}

void DupDataFile::sorted() {
	//std::sort(m_dataContainer->begin(), m_dataContainer->end(), compare);
}
bool DupDataFile::compare(std::string c1, std::string c2) {
	return true;
}

std::string DupDataFile::findData(unsigned long crc) {
	int pos = 0;
	std::string data;
	if (m_dataContainer->size() == 0) {
		return data;
	}
	for (std::vector<std::string>::iterator i = m_dataContainer->begin(); i != m_dataContainer->end(); i++) {

		data = *i;
		//printf("%s", data.c_str());
		size_t delim1 = data.find_first_of(':');
		std::string crcStr = data.substr(0, delim1);
		unsigned long newcrc = std::stoul(crcStr.c_str(), NULL, 16);
		if (newcrc == crc) {
			/* crc only
			DupData dupData(data.c_str());
			if (dupData.getMd5().compare(md5) == 0) {
			return pos;
			}
			*/
			return data;
		}
		pos++;
	}

	return data;
}

int DupDataFile::find(unsigned long crc) {
	int pos = 0;
	if (m_dataContainer->size() == 2) {
		int debug = 2;
		//DEBUG_PRINT("%d", debug);
	}
	if (m_dataContainer->size() == 0) {
		return -1;
	}
	for (std::vector<std::string>::iterator i = m_dataContainer->begin(); i != m_dataContainer->end(); i++, pos++) {

		std::string data = *i;
		//printf("%s", data.c_str());
		size_t delim1 = data.find_first_of(':');
		std::string crcStr = data.substr(0,delim1);
		unsigned long newcrc = std::stoul(crcStr.c_str(), NULL, 16);
		if (newcrc == crc) {
			CSVArgs csvArgs(':');
			csvArgs.process(data);
			/* crc only
			DupData dupData(data.c_str());
			if (dupData.getMd5().compare(md5) == 0) {
				return pos;
			}
			*/
			/*
			int i = csvArgs.size();
			if (csvArgs.size() <= 3) {
				// Not archived
				return ((int)-1);
			}
			*/
			return pos;
		}
	}

	return ((int)-1);
}

ImageIndex::ImageIndex() : m_dupCache(std::make_unique<DupCache>())
{
}

ImageIndex::~ImageIndex() {
	// TODO Auto-generated destructor stub  !std::filesystem::exists( "myfile.txt"
}

bool ImageIndex::init(const char *path) {
	std::filesystem::path data_dir(path);
	
	if (std::filesystem::is_directory(data_dir) == false) {
		
		throw std::exception("De-duplication database not found");
		
		return false;
	}
	m_dbpath = path;
	return true;
}

/*
std::string get_file_contents(const char *filename)
{

	
	int count = 0;
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize((size_t)in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}
*/

bool ImageIndex::add(const FileInfo &fileInfo) {
	
	std::string pathStr = fileInfo.getPath().string();
	
#ifdef _WIN32
#define SEP "\\"
#else
#define SEP "/"
#endif
	

	//std::string filename = pathStr.substr(pathStr.find_last_of("/") + 1);
	std::string filename = fileInfo.getName();
	return add(filename.c_str(), fileInfo.getCrc(), fileInfo.getSha256().c_str(), fileInfo.getPath().string().c_str(), 0, 0);
}



bool ImageIndex::add(const FileInfo &fileinfo, const char *orginalName) {
	return add(fileinfo.getName().c_str(), fileinfo.getCrc(), fileinfo.getSha256().c_str(), fileinfo.getPath().string().c_str(), 0, orginalName);
}




bool ImageIndex::isDupInCache(unsigned long crc) {
	return m_dupCache->isDup(crc);
}



bool ImageIndex::add2DupCache(const FileInfo &fileinfo) {
	return m_dupCache->insert(fileinfo.getCrc(), fileinfo.getSha256().c_str(), fileinfo.getName().c_str());
}

bool ImageIndex::add(const char *name, uint64_t crc, const char *sha256, const char *path, int version, const char *orginal) {
	if (add(name, crc, sha256, path, version, m_dbpath.c_str(), orginal) == false) {
		return false;
	}
	/*
	if (m_mirrorImageIndex->process(crc) == false) {
		return false;
	}
	*/
	return true;
}

bool ImageIndex::add(const char *name, uint64_t crc, const char *md5, const char *imagePath, int version, const char *rootPath, const char *orginal) {
	char hexStr[3];
	unsigned char data[4];
	data[0] = (unsigned char)crc & 0xFF;
	data[1] = (unsigned char)(crc >> 8) & 0xFF;
	data[2] = (unsigned char)(crc >> 2 * 8) & 0xFF;
	data[3] = (unsigned char)(crc >> 3 * 8) & 0xFF;

	std::string dbpath = rootPath;
	//printf("%x %x %x %x : ", m_data[3], m_data[2], m_data[1], m_data[0]);
	
	chartohex2(hexStr, data[3]);
	std::string path = dbpath + '/' + hexStr;
	makeDirectoryIfNotExists(path.c_str());
	
	chartohex2(hexStr, data[2]);
	path = path + '/' + hexStr;
	makeDirectoryIfNotExists(path.c_str());

	DupDataFile dupDataFile;
	
	chartohex2(hexStr, data[1]);
	path = path + '/' + hexStr;
	if (std::filesystem::exists(path.c_str()) == true) {
		if (dupDataFile.read(path.c_str()) == false) {
			throw std::exception();
		}

	}
	std::string ver = std::to_string(version);
	if (dupDataFile.add(name, crc, md5, imagePath, ver.c_str(), orginal) == false) {
		return false;
	}
	if (dupDataFile.write(path.c_str()) == false) {
		throw std::exception();
	}
	return true;
}

DupDataFile_Ptr ImageIndex::findDupDataFile(unsigned long crc) {
	return findDupDataFile(crc, m_dbpath.c_str());
}

DupDataFile_Ptr ImageIndex::findDupDataFile(unsigned long crc, const char *rootPath) {
	
	char hexStr[3];
	unsigned char data[4];
	data[0] = (unsigned char)crc & 0xFF;
	data[1] = (unsigned char)(crc >> 8) & 0xFF;
	data[2] = (unsigned char)(crc >> 2 * 8) & 0xFF;
	data[3] = (unsigned char)(crc >> 3 * 8) & 0xFF;

	//printf("%x %x %x %x : ", m_data[3], m_data[2], m_data[1], m_data[0]);
	chartohex2(hexStr, data[3]);
	
	std::string dbpath = rootPath;
	std::string path = dbpath + '/' + hexStr;
	makeDirectoryIfNotExists(path.c_str());
		
	
	chartohex2(hexStr, data[2]);
	path = path + '/' + hexStr;
	makeDirectoryIfNotExists(path.c_str());
	
	DupDataFile_Ptr dupDataFile = std::make_unique<DupDataFile>();
	
	chartohex2(hexStr, data[1]);
	path = path + '/' + hexStr;
	makeDirectoryIfNotExists(path.c_str());
	return dupDataFile;
}

bool ImageIndex::IsDup(unsigned long crc) {
	// this is a possable memory leek
	std::unique_ptr<DupDataFile> pDupDataFile(findDupDataFile(crc));
	//DupDataFile *dupDataFile = findDupDataFile(crc);
	if (pDupDataFile->IsEmpty() == true) {
		return false;
	}
	if (pDupDataFile->find(crc) < 0) {
		return false;	// not found
	}
	return true;
}

std::string ImageIndex::FindDup(unsigned long crc) {
	// this is a possable memory leek
	std::string data;
	std::unique_ptr<DupDataFile> pDupDataFile(findDupDataFile(crc));
	//DupDataFile *dupDataFile = findDupDataFile(crc);
	if (pDupDataFile->IsEmpty() == true) {
		//delete dupDataFile;
		return data;
	}
	
	data = pDupDataFile->findData(crc);
	if (data.empty()) {
		return data;	// not found
	}
	return data;
}

bool ImageIndex::updatePath(unsigned long crc, const char *path, int version) {
	
	if (updatePath(crc, path, version, m_dbpath.c_str()) == false) {
		return false;
	}
	/*
	if (m_mirrorImageIndex->process(crc) == false) {
		return false;
	}
	*/
	return true;
}



bool ImageIndex::updatePath(unsigned long crc, const char *path, int version, const char *root) {
	// this is a possable memory leek
	std::unique_ptr<DupDataFile> pDupDataFile(findDupDataFile(crc, root));
	//DupDataFile *dupDataFile = findDupDataFile(crc);
	if (pDupDataFile->IsEmpty() == true) {
		//delete dupDataFile;
		return false;
	}
	int pos = pDupDataFile->find(crc);
	if (pos < 0) {
		return false;
	}
	std::string& data = pDupDataFile->getAt(pos);
	data += ':';
	data += path;
	data += ':';
	data += std::to_string(version);
	//pDupDataFile->putAt(pos, data);
	if (pDupDataFile->write() == false) {
		throw std::exception();
	}
	

	return true;
}


