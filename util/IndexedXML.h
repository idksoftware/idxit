#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


class FileInfo;
class FolderInfo;
class IndexInfo;

class XMLWriter {
protected:

	static std::string writeTag(const char* tag, const std::string& value);
	static std::string writeTextTag(const char* tag, const std::string& value);
public:
	XMLWriter();
	virtual ~XMLWriter();


};

class IXMLFileInfoWriter :public XMLWriter {
	std::ofstream m_file;
	std::string m_path;
public:
	IXMLFileInfoWriter(const char* path);
	~IXMLFileInfoWriter();
	bool add(FileInfo& fileInfo);
	//bool add(FolderInfo& folderInfo);
	//bool add(std::filesystem::path m_path);
	std::ofstream &getFileStream() { return m_file; };

	bool WriteHeader(IndexInfo& indexInfo);
	bool WriteFooter(IndexInfo& indexInfo);
};

class IXMLLineReader {
	std::string m_line;
	int m_size{ -1 };
	int m_offsetSize{ 0 };
public:
	IXMLLineReader(const char* line);
	~IXMLLineReader();
	int getSize();
	std::string getAt(int idx);
	
};
