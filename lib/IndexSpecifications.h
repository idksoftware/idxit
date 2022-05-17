
#pragma once

/*
** Filters
Filters allow you to include or exclude items from your index.These items will be types of
files or folders found in the storage media.You may want to filter for some items i.e.include items,
or filter out or ignore i.e.exclude others.

For example; you may be looking for all your photographs on an external drive.However that contains
at least two system folders $recycle, $sys.You will wish to exclude those foldersand addition
you will want to include all the types of files that contain photographs.

** Identifying items
Items to be included or excluded will be files and folders found in the storage media.Files and
Folders can be identified by there full path.However files will be rarely identified in this way
but most likely files will be identified by file type.File types are denoted extension.

** Identified by full path
Identified by full path is used in a number of cases for example; the windows system folder will
normally excluded from the indexing search for example.Other common folders to excluded are a
group of system folders such as Program filesand temp folders etc.

** File Types

Folder Names
Folders can be identified by name.A common folder name will be “.icon” as windows will
create these by defaultand need to be excluded from the index.



** Special Filters
Special filters help you simplify common filtering problems that arise due to say indexing the system
disk where some files or folders need to be handled in a special way such as excluding all user
folders except the current user for example.

** Default Filters
Default filters help you simplify common filtering tasks such as excluding all system types or folders.

** Filters defined by environment.
*/

#include <set>
#include <string>
#include "ExtentionsFilterFile.h"
#include "FolderFilterFile.h"

class IndexFilter {
protected:
	
public:
	IndexFilter() = default;
	~IndexFilter() = default;
	virtual bool process() = 0;
	virtual bool read() = 0;
};

class IndexExtentionsFilter : public IndexFilter, public ExtentionsFilterObject {
protected:
	std::set<std::string> m_filterList;
	bool m_included{ false };
public:
	IndexExtentionsFilter() = default;
	~IndexExtentionsFilter() = default;
	virtual bool process();
	virtual bool read();
};

class IndexFolderFilter : IndexFilter, public FolderFilterObject {
protected:
	std::set<std::string> m_filterList;
	bool m_included{ false };
public:
	IndexFolderFilter() = default;
	~IndexFolderFilter() = default;
	virtual bool process();
	virtual bool read();
};


class UserExtentionsFilter : public IndexExtentionsFilter {
	
public:
	UserExtentionsFilter() = default;
	~UserExtentionsFilter() = default;
	
};

class UserFolderFilter : public IndexFolderFilter {
public:
	UserFolderFilter() = default;
	~UserFolderFilter() = default;
	
};

class OSExtentionsFilter : public IndexExtentionsFilter {
public:
	OSExtentionsFilter() = default;
	~OSExtentionsFilter() = default;
	
};

class OSFolderFilter : public IndexFolderFilter {
public:
	OSFolderFilter() = default;
	~OSFolderFilter() = default;

};





class IndexSpecifications
{
	std::vector<std::shared_ptr<IndexFilter>> m_indexFilters;
public:
	IndexSpecifications() = default;
	~IndexSpecifications() = default;
	bool insert(std::shared_ptr<IndexFilter> indexFilter);
	bool process();
	bool read();
};

