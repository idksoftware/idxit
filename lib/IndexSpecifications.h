#pragma once
#include <set>
#include <string>

class IndexFilters {
	std::set<std::string> m_included;
	std::set<std::string> m_excluded;
public:
	IndexFilters() = default;
	~IndexFilters() = default;
	virtual bool process() = 0;
	virtual bool read() = 0;
};

class ExtentionsFilter : public IndexFilters {
public:
	ExtentionsFilter() = default;
	~ExtentionsFilter() = default;
	bool process() override;
};

class FolderFilter : public IndexFilters {
public:
	FolderFilter() = default;
	~FolderFilter() = default;
	bool process() override;
};

class OSExtentionsFilter : public IndexFilters {
public:
	OSExtentionsFilter() = default;
	~OSExtentionsFilter() = default;
	bool process() override;
};

class OSFolderFilter : public IndexFilters {
public:
	OSFolderFilter() = default;
	~OSFolderFilter() = default;
	bool process() override;
};



bool ExtentionsFilter::process()
{
	return true;
}

bool FolderFilter::process()
{
	return true;
}

bool OSExtentionsFilter::process()
{
	return true;
}

bool OSFolderFilter::process()
{
	return true;
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

