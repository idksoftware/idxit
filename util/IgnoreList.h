#pragma once
#include <vector>
#include <memory>
#include <string>

class IgnoreFilter
{
	std::string m_filter;
public:
	IgnoreFilter(const char* filter) : m_filter(filter) {}
	bool match();
};

class IgnoreList : public std::vector<std::shared_ptr<IgnoreFilter>>
{
public:
	IgnoreList() = default;
	bool read(const char* file);
	bool match(const char *path);
};

