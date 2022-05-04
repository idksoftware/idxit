#include "WorkspaceFiles.h"
#include "SAUtils.h"
#include <iostream>
#include <fstream>

std::string WorkspaceFiles::m_workspaceRoot;


WorkspaceFiles::WorkspaceFiles(const char* szAddress)
{
	m_address = szAddress;
	
}

bool WorkspaceFiles::process()
{
	std::string year = m_address.substr(0, 4);
	std::string fullPath = m_workspaceRoot;
	fullPath += '/';
	fullPath += year;
	fullPath += '/';
	fullPath += m_address;
	std::string ctlPath = fullPath;
	ctlPath += "/.imga";
	FileList_Ptr list =SAUtils::getFiles(fullPath.c_str());
	
	std::string chgedFilePath = ctlPath;
	chgedFilePath += "/chged.dat";

	std::ofstream chgedFile(chgedFilePath.c_str());
	chgedFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	if (!chgedFile.is_open()) {
		return false;
	}
	for (auto i = list->begin(); i != list->end(); i++) {
		if (i->compare(".") == 0 || i->compare("..") == 0 || i->compare(".imga") == 0) {
			continue;
		}
		std::string fullFilePath = fullPath;
		fullFilePath += '/';
		fullFilePath += i->c_str();
		chgedFile << *i << ':' << SAUtils::modTime(fullFilePath.c_str()) << std::endl;
	}
	chgedFile.close();
	
	return true;
}

void WorkspaceFiles::setWorkspaceRoot(char* path)
{
	m_workspaceRoot = path;
}
