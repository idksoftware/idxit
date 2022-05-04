#pragma once
#include <string>

class Base10 {
	std::string m_str;
	int32_t m_int;
	void padToBase10(std::string& str, const size_t num)
	{
		if (num > str.size())
			str.insert(0, num - str.size(), '0');
	}
public:
	Base10() = default;
	virtual ~Base10() = default;
	const char* toBase10(long dec, int padding = 0);
	static uint32_t toDecimal(std::string encoded);
	const char* getBase10() const {
		return m_str.c_str();
	}
};



class Base16 {
	std::string m_str;
	int32_t m_int;
	void padToBase16(std::string& str, const size_t num)
	{
		if (num > str.size())
			str.insert(0, num - str.size(), '0');
	}
public:
	Base16() = default;
	virtual ~Base16() = default;
	const char* toBase16(long dec, int padding = 0);
	static int toDecimal(std::string encoded);
	const char* getBase16() const {
		return m_str.c_str();
	}
};


