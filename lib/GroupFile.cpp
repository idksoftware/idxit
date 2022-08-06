#include <fstream>
#include "CSVArgs.h"
#include "GroupFile.h"
#include "CLogger.h"


namespace simplearchive {

	bool GroupFile::read(const char* datafile) {

		CLogger& logger = CLogger::getLogger();

		std::string text;

		std::ifstream file(datafile);
		if (file.is_open() == false) {
			return false;
		}
		logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Processing Group file: %s", datafile);
		CSVArgs csvArgs(':');
		bool res = true;
		while (std::getline(file, text)) {
			std::shared_ptr<GroupItem> item = std::make_shared<GroupItem>();
			csvArgs.process(text);
			item->m_ext = csvArgs.at(0);
			item->m_mediaType = csvArgs.at(1);
			item->m_discription = csvArgs.at(2);
			m_list.push_back(item);
			logger.log(LOG_COMPLETED, CLogger::Level::STATUS, "Ext: %s - Included", item->m_ext.c_str());
		}
		file.close();
		return res;
	}

};