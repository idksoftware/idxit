#pragma once
#include <vector>
#include <memory>
#include <string>

class IgnoreFilter
{
	bool m_isFile;
	std::string m_filter;
public:
	IgnoreFilter(const char* filter, bool isFile) : m_isFile(isFile), m_filter(filter) {}
	bool match();
};

class IgnoreList : public std::vector<std::shared_ptr<IgnoreFilter>>
{
public:
	IgnoreList() = default;
	bool read(const char* file);
	bool match(const char *path);
};

