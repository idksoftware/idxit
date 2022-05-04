#pragma once
#include <string>
#include <cctype>
#include <sstream>
#include <iomanip>


enum class RenameBase {
	Unknown,
	Base10,
	Base16,
	Base32,
	Base64,
};


enum class RenameType {
	Unknown,
	Sequence,
	Date,
	Readabledate,
	NoRename
};

class ImageSequence {
	
protected:
	int32_t m_int{ 0 };
	std::string m_str;
	std::string m_name;
	std::string m_path;
	RenameBase m_renameBase{ RenameBase::Base32 };
	uint16_t fileCount(const char* path);

	void padDec(std::string &str, const size_t num)
	{
		if (num > str.size())
			str.insert(0, num - str.size(), '0');
	}

	void padToBase32(std::string &str, const size_t num)
	{
		if (num > str.size())
			str.insert(0, num - str.size(), 'A');
	}

	bool isNumber(const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && std::isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	}

	virtual std::string toBase10();
	virtual std::string toBase16();
	virtual std::string toBase32();
	virtual std::string toBase64();

public:
	ImageSequence() = default;
	virtual ~ImageSequence() = default;
	virtual const char *toSequence() = 0;
	
	void setBase(RenameBase base)
	{
		m_renameBase = base;
	}
};



class NumberSequence : public ImageSequence {

protected:
	
public:
	NumberSequence(int32_t dec) {
		m_int = dec;  
	};
	NumberSequence(std::string& encoded) {
		m_str = encoded;
	}
	~NumberSequence() = default;
	const char *toSequence();
	uint32_t toDecimal();
};

class DateSequence : public ImageSequence {
	int m_year;
	int m_month;
	int m_day;
	int getNumber(const std::string& filenane);
	std::string toString();

protected:
	
	uint16_t days_from_civil(int y, unsigned m, unsigned d);
public:
	DateSequence(int y, int m, int d, const char* path);
	DateSequence(std::string& encoded);
	~DateSequence() = default;
	const char *toSequence();
	int toDecimal();
	int toDate();
};




class ReadabledateSequence : public ImageSequence {
	int m_year;
	int m_month;
	int m_day;
	int getNumber(const std::string& filenane);
	std::string toString();

protected:

	virtual std::string toBase10() override;
	virtual std::string toBase16() override;
	virtual std::string toBase32() override;
	virtual std::string toBase64() override;

public:
	ReadabledateSequence(int y, int m, int d, const char* path);
	ReadabledateSequence(std::string& encoded);
	~ReadabledateSequence() = default;
	const char* toSequence();
	int toDecimal();
	int toDate();
};


/*
Unknown,
UseDate,
UseNumber,
NoRename,
*/
class NoRenameSequence : public ImageSequence {
	std::string m_path;
	std::string m_file;
	std::string m_result;
	int m_max{ 0 };
protected:
	virtual std::string toBase10() override;
	virtual std::string toBase16() override;
	virtual std::string toBase32() override;
	virtual std::string toBase64() override;
	std::string fileExtention(const char* f);
	std::string fileNoExtention(const char* f);
	std::string fileNoRenameSeq(const char* f);
public:
	NoRenameSequence(const char* p);
		
	~NoRenameSequence() = default;
	const char *toSequence();
};
