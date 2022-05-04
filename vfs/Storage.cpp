#include "Storage.h"
#include "FileInfo.h"
#include <fstream>
#include <filesystem>
#include "cport.h"
#include "CSVArgs.h"




#define TO_HEX(i) (i <= 9 ? '0' + i : 'a' - 10 + i)

void chartohex6(char *buffer, uint64_t x) {
	buffer[0] = (char)TO_HEX(((x & 0xF00000) >> 20));
	buffer[1] = (char)TO_HEX(((x & 0x0F0000) >> 16));
	buffer[2] = (char)TO_HEX(((x & 0x00F000) >> 12));
	buffer[3] = (char)TO_HEX(((x & 0x000F00) >> 8));
	buffer[4] = (char)TO_HEX(((x & 0x0000F0) >> 4));
	buffer[5] = (char)TO_HEX((x  & 0x00000F));
	buffer[6] = '\0';
}

void chartohex8(char *buffer, uint64_t x) {
	buffer[0] = (char)TO_HEX(((x & 0xF0000000) >> 28));
	buffer[1] = (char)TO_HEX(((x & 0x0F000000) >> 24));
	buffer[2] = (char)TO_HEX(((x & 0x00F00000) >> 20));
	buffer[3] = (char)TO_HEX(((x & 0x000F0000) >> 16));
	buffer[4] = (char)TO_HEX(((x & 0x0000F000) >> 12));
	buffer[5] = (char)TO_HEX(((x & 0x00000F00) >> 8));
	buffer[6] = (char)TO_HEX(((x & 0x000000F0) >> 4));
	buffer[7] = (char)TO_HEX((x  & 0x0000000F));
	buffer[8] = '\0';
}

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

	std::filesystem::path data_dir(path);
	if (std::filesystem::is_directory(path) == false) {
		if (std::filesystem::create_directory(path) == false) {
			throw std::exception();
		}
		throw std::exception();
	}
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

typedef  std::vector<std::string> DataContainer;



class DataFile {
	std::unique_ptr<DataContainer> m_dataContainer;
	bool compare(std::string c1, std::string c2);
	void sorted();
	// Used to write file
	std::string m_filePath;
public:
	DataFile();
	virtual ~DataFile();
	bool read(const char *datafile);
	bool write();
	bool write(const char *datafile);
	bool add(const char *name, unsigned long crc, const char *md5, const char *path, const char *version, uint64_t sequence);
	int find(uint64_t sequence);
	std::string findData(uint64_t sequence);
	bool IsEmpty() {
		return m_dataContainer->empty();
	}
	void insert(uint64_t sequence, std::string &row);
	std::string& getAt(int pos);
	void putAt(int pos, std::string item);
};

DataFile::DataFile() : m_dataContainer(std::make_unique<DataContainer>()) {}

DataFile::~DataFile() {
}

bool DataFile::read(const char *datafile) {
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

bool DataFile::write() {
	return write(m_filePath.c_str());
}

bool DataFile::write(const char *datafile) {
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
bool DataFile::add(const char *name, unsigned long crc, const char *md5, const char *path, const char *version, uint64_t sequence) {
	//char c_crc[9];
	std::string crcStr; // = c_crc;
	sprintf_p(crcStr, "%.8x", crc);
	//std::string crcStr = c_crc;
	std::string nameStr = name;
	std::string md5Str = md5;
	std::string sequenceStr;
	sprintf_p(sequenceStr, "%.8x", sequence);
	std::string row(sequenceStr + ':' + crcStr + ':' + md5Str + ':' + nameStr + ':' + version + ':' + path);
	if (find(sequence) != -1) {
		return false; // found
	}
	insert(sequence, row);
	return true;
}

std::string& DataFile::getAt(int pos) {
	return m_dataContainer->at(pos);
}

void DataFile::putAt(int pos, std::string item) {
	(*m_dataContainer)[pos] = item;
}

void DataFile::insert(uint64_t sequence, std::string& row) {
	bool found = false;
	int pos = 0;
	for (std::vector<std::string>::iterator i = m_dataContainer->begin(); i != m_dataContainer->end(); i++) {
		std::string data = *i;
		//printf("%s", data.c_str());
		size_t delim1 = data.find_first_of(':');
		std::string sequenceStr = data.substr(0, delim1);
		unsigned long newsequence = std::stoul(sequenceStr.c_str(), NULL, 16);
		if (newsequence > sequence) {
			found = true;
			break;
		}
		pos++;
	}
	if (!found) {
		m_dataContainer->push_back(row);
	}
	else {
		m_dataContainer->insert(m_dataContainer->begin() + (pos), row);
	}

}

void DataFile::sorted() {
	//std::sort(m_dataContainer->begin(), m_dataContainer->end(), compare);
}
bool DataFile::compare(std::string c1, std::string c2) {
	return true;
}

std::string DataFile::findData(uint64_t sequence) {
	int pos = 0;
	std::string data;
	if (m_dataContainer->size() == 0) {
		return data;
	}
	for (std::vector<std::string>::iterator i = m_dataContainer->begin(); i != m_dataContainer->end(); i++) {

		data = *i;
		//printf("%s", data.c_str());
		size_t delim1 = data.find_first_of(':');
		std::string sequenceStr = data.substr(0, delim1);
		unsigned long newcrc = std::stoul(sequenceStr.c_str(), NULL, 16);
		/* needs sorting out
		if (newcrc == crc) {
			// crc only
			//DupData dupData(data.c_str());
			//if (dupData.getMd5().compare(md5) == 0) {
			//return pos;
			//}
			
			return data;
		}
		*/
		pos++;
	}

	return data;
}

int DataFile::find(uint64_t sequence) {
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
		std::string crcStr = data.substr(0, delim1);
		unsigned long newcrc = std::stoul(crcStr.c_str(), NULL, 16);
		if (newcrc == sequence) {
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


std::string Storage::m_path = "";
uint64_t Storage::m_sequence = 0x79864399;

Storage::Storage()
{
}


Storage::~Storage()
{
}

void Storage::add(FileInfo &fileInfo) {
	m_sequence++;
	char hexStr[9];
	uint16_t data[3];
	data[0] = (uint16_t)m_sequence & 0xFF; // 8 bits
	data[1] = (uint16_t)(m_sequence >> 8) & 0xFFF;
	data[2] = (uint16_t)(m_sequence >> 5 * 4) & 0xFFF;
	
	uint64_t folderNo = (uint64_t)(m_sequence >> 8);
	chartohex6(hexStr, folderNo);
	std::string folderName = hexStr;

	std::string dbpath = m_path;
	//printf("%x %x %x %x %x \n", m_sequence, data[3], data[2], data[1], data[0]);

	chartohex3(hexStr, data[2]);
	std::string path = dbpath + '/' + hexStr;
	makeDirectoryIfNotExists(path.c_str());

	chartohex3(hexStr, data[1]);
	path = path + '/' + hexStr;
	makeDirectoryIfNotExists(path.c_str());
	chartohex8(hexStr, m_sequence);
	std::string folderPath = path;
	path = path + '/' + hexStr;
	
	printf("%I64x %s\n", m_sequence, path.c_str());
	try {
		std::filesystem::copy_file(fileInfo.getPath(), path.c_str(),
									std::filesystem::copy_options::overwrite_existing);
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		printf("File copy error: %s\n", e.what());
	}

	//std::filesystem::copy_file(fileInfo.getPath(), path.c_str(), std::filesystem::copy_option::fail_if_exists);
	folderPath += '/';
	chartohex6(hexStr, (m_sequence >> 8));
	folderPath += hexStr;
	DataFile dataFile;

	if (std::filesystem::exists(folderPath.c_str()) == true) {
		if (dataFile.read(folderPath.c_str()) == false) {
			throw std::exception();
		}

	}
	
	std::string ver = std::to_string(0);
	if (dataFile.add(fileInfo.getName().c_str(), fileInfo.getCrc(), fileInfo.getSha256().c_str(),
		fileInfo.getPath().string().c_str(), ver.c_str(), m_sequence) == false) {
		return;
	}
	if (dataFile.write(folderPath.c_str()) == false) {
		throw std::exception();
	}
	
	return;
}