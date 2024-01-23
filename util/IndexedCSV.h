#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


class FileInfo;
class FolderInfo;
class IndexInfo;


class ICSVFileInfoWriter {
	std::ofstream m_file;
	std::string m_path;
public:
	ICSVFileInfoWriter(const char* path);
	~ICSVFileInfoWriter();
	bool add(FileInfo& fileInfo);
	//bool add(FolderInfo& folderInfo);
	//bool add(std::filesystem::path m_path);
	std::ofstream &getFileStream() { return m_file; };

	bool WriteHeader(IndexInfo& indexInfo);
	bool WriteFooter(IndexInfo& indexInfo);
};

class ICSVLineReader {
	std::string m_line;
	int m_size{ -1 };
	int m_offsetSize{ 0 };
public:
	ICSVLineReader(const char* line);
	~ICSVLineReader();
	int getSize();
	std::string getAt(int idx);
	
};
