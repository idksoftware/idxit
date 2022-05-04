#pragma once
#include <string>

class UniqueId
{
public:
	UniqueId() = default;
	~UniqueId() = default;
	std::string get();
};

