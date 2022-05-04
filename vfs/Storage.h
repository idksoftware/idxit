#pragma once
#include <cstdint>
#include <string>

class FileInfo;
class Storage
{
	static std::string m_path;
	static uint64_t m_sequence;
public:
	Storage();
	~Storage();
	static void setPath(const char *path) {
		m_path = path;
	}
	void add(FileInfo &fileInfo);
	
};

