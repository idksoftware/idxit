#pragma once
#include <set>
#include <string>
#include "ExtentionsFilterFile.h"

class IndexFilters {
	std::set<std::string> m_included;
	std::set<std::string> m_excluded;
public:
	IndexFilters() = default;
	~IndexFilters() = default;
	virtual bool process() = 0;
	virtual bool read() = 0;
};

class ExtentionsFilter : public IndexFilters { //}; , public ExtentionsFilterObject{
public:
	ExtentionsFilter() = default;
	~ExtentionsFilter() = default;
	bool process() override;
	bool read() override;
};

class FolderFilter : public IndexFilters {
public:
	FolderFilter() = default;
	~FolderFilter() = default;
	bool process() override;
	bool read() override;
};

class OSExtentionsFilter : public IndexFilters { //}; , public ExtentionsFilterObject{
public:
	OSExtentionsFilter() = default;
	~OSExtentionsFilter() = default;
	bool process() override;
	bool read() override;
};

class OSFolderFilter : public IndexFilters {
public:
	OSFolderFilter() = default;
	~OSFolderFilter() = default;
	bool process() override;
	bool read() override;
};



bool ExtentionsFilter::process()
{
	return true;
}

inline bool ExtentionsFilter::read()
{
	return false;
}

bool FolderFilter::process()
{
	return true;
}

inline bool FolderFilter::read()
{
	return false;
}

bool OSExtentionsFilter::process()
{
	return true;
}

inline bool OSExtentionsFilter::read()
{
	return false;
}

bool OSFolderFilter::process()
{
	return true;
}

inline bool OSFolderFilter::read()
{
	return false;
}

class IndexSpecifications
{
	ExtentionsFilter m_extentions;
	FolderFilter m_folderFilter;
	OSExtentionsFilter m_osExtentions;
	OSFolderFilter m_osFolderFilter;
public:
	IndexSpecifications() = default;
	~IndexSpecifications() = default;

};

