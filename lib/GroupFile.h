#pragma once

#include <string>
#include <vector>

class GroupFile {
	//bool compare(std::string c1, std::string c2);
	//void sorted();
	//bool insert(uint64_t idx, const char* imagePath, const char* name, uint64_t size,
	//	uint64_t crc, const char* sha, const char* uuid, int version, ExifDate& date, uint64_t dbIdx);

	std::vector<std::string> m_list;
public:
	GroupFile() = default;
		
	
	virtual ~GroupFile() = default;
		
	bool read(const char* datafile);
	bool write(const char* datafile);
	//bool update(uint64_t idx, const char* imagePath, const char* name, uint64_t size, uint64_t crc,
	//	const char* sha, const char* uuid, int version, ExifDate& date, uint64_t dbidx);
	//std::shared_ptr<IdxFileItem> find(uint64_t idx);
	//uint64_t findLast();

};