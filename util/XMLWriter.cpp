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

#include "XMLWriter.h"
#include "CIDKDate.h"
#include "FileInfo.h"
#include <sstream>
#include <iostream>
#include <fstream>

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

std::string XMLWriter::writeTag(const char *tag, const std::string& value) {
	std::ostringstream xml;
	if (!value.empty()) {
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	} else {
		xml << "<" << tag << "/>\n";
	}
	return xml.str();
}





XMLFileInfoWriter::XMLFileInfoWriter(const char *path) {
	m_path = path;

	m_xmlFile.open(path);
	m_xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
	m_xmlFile << "<FileSet>\n";
}

XMLFileInfoWriter::~XMLFileInfoWriter() {
	m_xmlFile << "</FileSet>\n";
	m_xmlFile.close();
}

static const char *dateFormat = "%Y:%m:%d %H:%M:%S";

bool XMLFileInfoWriter::add(FileInfo &fileInfo) {


	CIDKDate modDate = fileInfo.getModTime();
	std::string mod = modDate.Format(dateFormat);
	CIDKDate createDate = fileInfo.getCreateTime();
	std::string create = createDate.Format(dateFormat);
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

	return true;
}

