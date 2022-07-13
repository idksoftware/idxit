#include <fstream>
#include "CSVArgs.h"
#include "GroupFile.h"

bool GroupFile::read(const char* datafile) {
	std::string text;
	
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	CSVArgs csvArgs(':');
	bool res = true;
	while (std::getline(file, text)) {
		std::shared_ptr<GroupItem> item = std::make_shared<GroupItem>();
		csvArgs.process(text);
		item->m_ext = csvArgs.at(0);
		item->m_mediaType = csvArgs.at(1);
		item->m_discription = csvArgs.at(2);
	}
	file.close();
	return res;
}