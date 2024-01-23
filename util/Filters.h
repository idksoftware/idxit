#pragma once

#pragma once

#include <vector>
#include "SAUtils.h"


class FileFilter : public std::vector<std::string> {
public:
	bool match(std::string& ext) {
		if (ext.empty()) {
			return false;
		}
		for (auto i = begin(); i != end(); i++) {

			if (SAUtils::isEquals(*i, ext)) {
				return true;
			}
		}
		return false;
	}

};

class FolderFilter : public std::vector<std::string> {
public:
	bool match(std::string& folder) {
		for (auto i = begin(); i != end(); i++) {
			// May need a switch if folder is not case sensitive
			if (i->compare(folder) == 0) {
				return true;
			}
		}
		return false;
	}

};