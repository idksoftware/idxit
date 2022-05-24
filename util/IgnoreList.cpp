#include "IgnoreList.h"
#include <fstream>
#include <iostream>
#include <string>


bool IgnoreList::read(const char* fullpath)
{
	clear();

	std::ifstream file(fullpath);
	if (file.is_open() == false) {
		//ErrorCode::setErrorCode(IMGA_ERROR::OPEN_ERROR);
		return false;
	}

	for (std::string line; std::getline(file, line);) {
		if (line.length() > 0) {
			
		}
		else {
			break;
		}
	}


	file.close();
	
	return true;
}

bool IgnoreList::match(const char* path)
{
	return true;
}



bool IgnoreFilter::match()
{
	return true;
}
