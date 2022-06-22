#include "IgnoreList.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

const std::string WHITESPACE = " \n\r\t";

std::string ltrim(const std::string& s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string& s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string& s) {
	return rtrim(ltrim(s));
}

bool IgnoreList::read(const char* fullpath)
{
	clear();

	std::ifstream file(fullpath);
	if (file.is_open() == false) {
		//ErrorCode::setErrorCode(IMGA_ERROR::OPEN_ERROR);
		return false;
	}
	int lineNo = 0;
	for (std::string line; std::getline(file, line);) {
		lineNo++;
		if (line.length() > 0) {
			const std::string delims(" \t");
			std::string::size_type pos = line.find_first_not_of(delims);
			if (line[pos] == '#') {
				continue;
			}
			pos = line.find_first_of('#');
			line = line.substr(0, pos);
			line = trim(line);
			std::shared_ptr<IgnorePath> item = std::make_shared<IgnorePath>(line.c_str(), lineNo, fullpath);
			push_back(item);
		}

	}


	file.close();
	
	return true;
}

bool IgnoreList::match(const char* path)
{
	for (auto i : *this) {
		if (i->match(path) == true) {
			return true;
		}
	}
	return true;
}



bool IgnoreFilter::match()
{
	return true;
}
