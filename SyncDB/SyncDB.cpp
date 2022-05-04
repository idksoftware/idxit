// SyncDB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Sstream>
#include "tinyxml.h"
#include "CIDKDate.h"
#include "ImageIndex.h"

static const char *dateFormat = "%Y:%m:%d %H:%M:%S";
class FileInfoItem
{
	std::string m_name;
	std::string m_path;
	unsigned long m_crc;
	std::string m_mediaType;
	std::string m_md5;
	unsigned long m_size;
	CIDKDate m_modTime;
	CIDKDate m_createTime;

public:
	FileInfoItem(const char *name, const char *path, const char *crc, const char *mediaType,
		const char *md5, const char *size, const char *modTime, const char *createTime) :
		m_name(name),
		m_path(path),
		
		m_mediaType(mediaType),
		m_md5(md5)
		
		
		//m_modTime(modTime),
		//m_createTime(createTime)
	{
		if (modTime != nullptr && modTime[0] != '\0') {
			m_modTime = modTime;
		}
		if (createTime != nullptr && createTime[0] != '\0') {
			m_createTime = createTime;
		}
		m_crc = atoi(crc);
		m_size = atoi(size);
	};

	std::string toString() {
		std::stringstream s;
		s << "===========================\n";
		s << "Name: " << m_name << '\n';
		s << "Path: " << m_path << '\n';
		s << "Crc: " << m_crc << '\n';
		s << "MediaType: " << m_mediaType << '\n';
		s << "Md5: " << m_md5 << '\n';
		s << "Size: " << m_size << '\n';
		s << "ModTime: " << m_modTime.Format(dateFormat) << '\n';
		s << "CreateTime: " << m_createTime.Format(dateFormat) << '\n';
		return s.str();
	}

	std::string &getName() { return m_name; };
	std::string &getPath() { return  m_path; };
	unsigned long getCrc() { return m_crc; };
	std::string  &getMediaType() { return m_mediaType; }
	std::string  getMd5() { return m_md5; }
	unsigned long  getSize() { return m_size; }
	CIDKDate  &getModTime() { return m_modTime; }
	CIDKDate  &getCreateTime() { return m_createTime; }
};



class XmlReader {
	std::string m_path;
	bool what = true;
	std::shared_ptr <std::vector<std::shared_ptr<FileInfoItem>>> m_list;
public:
	XmlReader(const char *configPath) :
		m_list(std::make_shared<std::vector<std::shared_ptr<FileInfoItem>>>()),
		m_path(configPath) {};
	XmlReader() {};
	bool loadFile();
	std::shared_ptr <std::vector<std::shared_ptr<FileInfoItem>>> getList() {
		return m_list;
	}
};


bool XmlReader::loadFile() {

	TiXmlDocument document(m_path.c_str());
	bool loadOkay = document.LoadFile();
	if (loadOkay)
	{
		//printf("\n%s:\n", "config.xml");
		//dump_to_stdout(&doc); // defined later in the tutorial
	}
	TiXmlElement* root = document.FirstChildElement("FileSet");
	if (!root) {
		return false;
	}
	for (TiXmlElement* file = root->FirstChildElement(); file != NULL; file = file->NextSiblingElement())
	{
		std::string nameText;
		std::string pathText;
		std::string crcText;
		std::string md5Text;
		std::string sizeText;
		std::string mediaTypeText;
		std::string modText;
		std::string createText;


		TiXmlElement* name = file->FirstChildElement("Name");
		if (name) {
			nameText = name->GetText();

		}
		TiXmlElement* path = file->FirstChildElement("Path");
		if (path) {
			pathText = path->GetText();

		}
		TiXmlElement* crc = file->FirstChildElement("CRC");
		if (crc) {
			crcText = crc->GetText();
		}
		TiXmlElement* md5 = file->FirstChildElement("MD5");
		if (md5) {
			md5Text = md5->GetText();

		}
		TiXmlElement* mediaType = file->FirstChildElement("MediaType");
		if (mediaType) {
			const char *text = mediaType->GetText();
			if (text != 0) {
				mediaTypeText = text;

			}

		}
		TiXmlElement* size = file->FirstChildElement("Size");
		if (size) {
			sizeText = size->GetText();

		}
		TiXmlElement* modTime = file->FirstChildElement("ModTime");
		if (modTime) {
			modText = modTime->GetText();

		}
		TiXmlElement* createTime = file->FirstChildElement("CreateTime");
		if (createTime) {
			createText = createTime->GetText();

		}
		std::shared_ptr<FileInfoItem> fileInfoItem = std::make_shared<FileInfoItem>(nameText.c_str(), pathText.c_str(), crcText.c_str(),
			mediaTypeText.c_str(), md5Text.c_str(), sizeText.c_str(), modText.c_str(), createText.c_str());
		//std::string s = fileInfoItem->toString();
		m_list->push_back(fileInfoItem);
		//std::cout << s;
	}

	return true;
}

bool addItem(ImageIndex &imageIndex, FileInfoItem &item) {
	return imageIndex.add(item.getName().c_str(), ((unsigned long)item.getCrc()), item.getMd5().c_str(), item.getPath().c_str(), 0, item.getPath().c_str());
}

int main()
{

	XmlReader xmlReader("C:\\temp\\test\\sync.xml");
	bool loadOkay = xmlReader.loadFile();

	if (loadOkay)
	{
		printf("\n%s:\n", "config.xml");
		//dump_to_stdout(&doc); // defined later in the tutorial
	}

	ImageIndex imageIndex;
	if (imageIndex.init("G:\\") == false) {
		return false;
	}
	
	std::shared_ptr <std::vector<std::shared_ptr<FileInfoItem>>> list = xmlReader.getList();
	for (auto i = list->begin(); i != list->end(); i++) {
		std::shared_ptr<FileInfoItem> item = *i;
		
		std::string s = item->toString();
		std::cout << s << '\n';
		if (addItem(imageIndex, *item) == false) {
			std::cout << "failed" << '\n';
		}
	}
}

