#include "IndexSpecifications.h"

bool IndexExtentionsFilter::read()
{
	ExtentionsFilterObject extentionsFilterObject;

	if (extentionsFilterObject.setExtentionsFilePath("C:\\ProgramData\\IDK-Software\\idxit\\filter\\userincext.fff") == false) {
		return false;
	}

	if (extentionsFilterObject.init() == false) {
		return false;
	}
	std::vector<std::shared_ptr<ExtentionItem>> list;
	if (extentionsFilterObject.getList(list) == false) {
		return false;
	}
	for (auto ii : list)
	{
		m_filterList.insert(ii->getExt().c_str());
	}
	return true;
}

bool IndexExtentionsFilter::process()
{
	return false;
}


bool IndexSpecifications::insert(std::shared_ptr<IndexFilter> indexFilter)
{
	return false;
}

bool IndexSpecifications::process()
{
	return false;
}

bool IndexSpecifications::read()
{
	return false;
}

bool IndexFolderFilter::process()
{
	return true;
}

bool IndexFolderFilter::read()
{
	FolderFilterObject folderFilterObject;

	if (folderFilterObject.setFolderFilePath("C:\\ProgramData\\IDK-Software\\idxit\\filter\\userincext.fff") == false) {
		return false;
	}

	if (folderFilterObject.init() == false) {
		return false;
	}
	std::vector<std::shared_ptr<FolderItem>> list;
	if (folderFilterObject.getList(list) == false) {
		return false;
	}
	for (auto ii : list)
	{
		m_filterList.insert(ii->getPath().c_str());
	}
	return true;
}
