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

#include "StagingManager.h"
#include <time.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include"SAUtils.h"

namespace simplearchive {

static int m_count = 0;
StagingManager::StagingManager(const char *root, const char *name, const char *stagingFolder) {
	m_root = root;
	m_name = name;
	m_stagingFolder = stagingFolder;
	m_time = 0;
}

StagingManager::~StagingManager() {
	// TODO Auto-generated destructor stub
}

bool StagingManager::put(const char *path) {
	std::string fullpath = m_root;
	fullpath += '/';
	fullpath += path;
	//printf("%s\n",fullpath.c_str());
	std::string stage = m_stagingFolder;
	stage += '/';
	std::string number = unqueNumger();
	stage += number;
	stage += ".dat";
	if (txtFile(number, path) == false) {
		return false;
	}
	if (SAUtils::copy(fullpath.c_str(), stage.c_str()) == false) {
		return false;
	}
	if (SAUtils::verify(fullpath.c_str(), stage.c_str()) == false) {
		return false;
	}
	return true;
}

std::string StagingManager::unqueNumger() {
	time_t timeCur = 0;
	time(&timeCur);
	if (m_time == timeCur) {
		m_count++;
	}
	else {
		m_time = timeCur;
		m_count = 0;
	}
	std::ostringstream sstream;
	sstream <<  m_time << std::setw(4) << std::hex << std::setfill('0')   << m_count;
	std::string tmp = sstream.str();
	return tmp;
}

bool StagingManager::txtFile(std::string &number, const char *path) {

	std::string filename = m_stagingFolder;
	filename += '/';
	filename += number;
	filename += ".txt";

	std::ofstream txtfile(filename.c_str());
	txtfile << number << ':' << path;
	txtfile.close();
	if (readTxtFile(number,path) == false) {
		return false;
	}
	return true;

}
bool StagingManager::readTxtFile(std::string &number, const char *path) {
	char text[1024];
	std::string filename = m_stagingFolder;
	filename += '/';
	filename += number;
	filename += ".txt";

	std::ifstream txtfile(filename.c_str());
	if (txtfile.is_open() == false) {
		return false;
	}
	std::string number1;
	std::string path1;
	while (txtfile.getline(text, 1024)) {
		std::string data = text;
		int delim1 = data.find_first_of(":");
		number1 = data.substr(0,delim1);
		path1 = data.substr(delim1+1, data.length());
	}
	txtfile.close();
	if (number1.compare(number) != 0) {
		return false;
	}
	if (path1.compare(path) != 0) {
		return false;
	}
	return true;
}

} /* namespace simplearchive */
