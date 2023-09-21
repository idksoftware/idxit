#pragma once
#include <vector>
#include <memory>
#include <string>
#include "IgnorePath.h"


class IgnoreFilter
{
	std::string m_filter;
	int m_lineNo{ 0 };
	std::string m_fileName;
public:
	IgnoreFilter(const char* filter, int line, const char* file)
		: m_filter(filter), m_lineNo(line), m_fileName(file) {}
	bool match();
};

class IgnoreList : public std::vector<std::shared_ptr<IgnorePath>>
{
public:
	IgnoreList() = default;
	bool read(const char* file);
	bool match(const char* path);
};

