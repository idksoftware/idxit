#include "GroupFile.h"

bool IdxFile::read(const char* datafile) {
	std::string text;
	m_last = -1;
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	bool res = true;
	while (std::getline(file, text)) {
		std::shared_ptr<IdxFileItem> item = std::make_shared<IdxFileItem>(text.c_str());
		if (item->isError()) {
			res = false;
		}
		uint64_t fullidx = item->getIdx();
		int fileidx = (unsigned int)fullidx & 0xFF;

		if (fileidx < 0 || fileidx > 255) {
			return false;
		}
		this->list[fileidx] = item;
		if (m_last < fileidx) {
			m_last = fileidx;
		}
	}
	file.close();
	return res;
}