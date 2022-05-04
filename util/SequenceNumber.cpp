#include "SequenceNumber.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#include <filesystem>
#include "Base16.h"
#include "Base32.h"
#include "Base64.h"


namespace fs = std::filesystem;


using FileList = std::vector<std::string>;
using FileList_Ptr = std::unique_ptr<FileList>;

std::string getFilenameNoExt(const std::string& file)
{
	size_t sp = file.find_last_of("/\\");

	std::string name;
	if (sp != -1)
	{
		name = file.substr(++sp);
	}
	const size_t ep = file.find_last_of('.');
	name = file.substr(0, ep);
	return name;
}

std::string getFilenameNoExt(const char* file)
{
	std::string filestr = file;
	return getFilenameNoExt(filestr);
}


std::string ImageSequence::toBase10()
{
	Base10 base;
	m_str = base.toBase10(m_int, 8);
	return m_str;
};

std::string ImageSequence::toBase16()
{
	Base16 base;
	m_str = base.toBase16(m_int, 8);
	return m_str;
};

std::string ImageSequence::toBase32()
{
	Base32 base;
	m_str = base.toBase32(m_int, 7);
	return m_str;
};

std::string ImageSequence::toBase64()
{
	Base64 base;
	m_str = base.tobase64(m_int, 6);
	return m_str;
};



const char * NumberSequence::toSequence()
{
	m_str.clear();
	switch (m_renameBase) {
	case RenameBase::Base10:
		m_str = toBase10();
		break;
	case RenameBase::Base16:
		m_str = toBase16();
		break;
	case RenameBase::Base32:
		m_str = toBase32();
		break;
	case RenameBase::Base64:
		m_str = toBase64();
		break;
	}
	return m_str.c_str();
}

uint32_t NumberSequence::toDecimal()
{
	uint32_t num = 0;
	switch (m_renameBase) {
	case RenameBase::Base10:
	{
		Base10 base;
		num = base.toDecimal(m_str);
		break;
	}
	case RenameBase::Base16:
	{
		Base10 base;
		num = base.toDecimal(m_str);
		break;
	}
	case RenameBase::Base32:
	{
		Base10 base;
		num = base.toDecimal(m_str);
		break;
	}
	case RenameBase::Base64:
	{
		Base10 base;
		num = base.toDecimal(m_str);
		break;
	}
	default:
		num = 0;
	}
	m_str.clear();
	return num;
}


/*
std::string DateSequence::toBase10()
{
	return std::string();
};

std::string DateSequence::toBase16()
{
	return std::string();
};

std::string DateSequence::toBase32()
{
	return std::string();
};

std::string DateSequence::toBase64()
{
	return std::string();
};
*/

DateSequence::DateSequence(int y, int m, int d, const char* path)
{
	m_year = y;
	m_month = m;
	m_day = d;
	m_path = path;
}

DateSequence::DateSequence(std::string& encoded)
{
}

std::string DateSequence::toString() {
	std::stringstream s;
	s << std::setw(2) << std::setfill('0') << m_year;
	s << std::setw(2) << std::setfill('0') << m_month;
	s << std::setw(2) << std::setfill('0') << m_day;
	return std::string(s.str());

}


int DateSequence::getNumber(const std::string& filenane) {
	size_t pos = filenane.find_last_of('.');
	std::string numStr = filenane.substr(pos - 5, 5);
	if (isNumber(numStr) == false) {
		return -1;
	}
	int num = atoi(numStr.c_str());
	return num;
}
const char * DateSequence::toSequence()
{
	uint16_t num =fileCount(m_path.c_str());
	uint16_t numDate = DateSequence::days_from_civil(m_year, m_month, m_day);
	m_int = numDate;
	m_int <<= 16;
	m_int |= num;

	m_str.clear();
	switch (m_renameBase) {
	case RenameBase::Base10:
		m_str = toBase10();
		break;
	case RenameBase::Base16:
		m_str = toBase16();
		break;
	case RenameBase::Base32:
		m_str = toBase32();
		break;
	case RenameBase::Base64:
		m_str = toBase64();
		break;
	}
	
	return m_str.c_str();
}

int DateSequence::toDecimal()
{
	return 0;
}

int DateSequence::toDate()
{
	return 0;
}

std::string ReadabledateSequence::toBase10()
{
	Base10 base;
	m_str = base.toBase10(m_int, 3);
	return m_str;
};

std::string ReadabledateSequence::toBase16()
{
	Base16 base;
	m_str = base.toBase16(m_int, 3);
	return m_str;
};

std::string ReadabledateSequence::toBase32()
{
	Base32 base;
	m_str = base.toBase32(m_int, 2);
	return m_str;
};

std::string ReadabledateSequence::toBase64()
{
	Base64 base;
	m_str = base.tobase64(m_int, 2);
	return m_str;
};



ReadabledateSequence::ReadabledateSequence(int y, int m, int d, const char* path)
{
	m_year = y;
	m_month = m;
	m_day = d;
	m_path = path;
}

ReadabledateSequence::ReadabledateSequence(std::string& encoded)
{

}

const char* ReadabledateSequence::toSequence()
{
	if (m_year > 2999 || m_year < 1900) {
		throw  std::exception();
	}
	if (m_year >= 2000) {
		m_year = m_year - 2000;
	}
	else {
		m_year = m_year - 1900;
	}
	
	std::stringstream s;
	s << std::setw(2) << std::setfill('0') << m_year;
	s << std::setw(2) << std::setfill('0') << m_month;
	s << std::setw(2) << std::setfill('0') << m_day;
	m_int = fileCount(m_path.c_str());

	std::string numStr;
	switch (m_renameBase) {
	case RenameBase::Base10:
		numStr = toBase10();
		break;
	case RenameBase::Base16:
		numStr = toBase16();
		break;
	case RenameBase::Base32:
		numStr = toBase32();
		break;
	case RenameBase::Base64:
		numStr = toBase64();
		break;
	}
	
	m_str = s.str();
	m_str += "-";
	m_str += numStr;

	return m_str.c_str();
}
	
int ReadabledateSequence::getNumber(const std::string& filenane)
{
	return 0;
}

std::string ReadabledateSequence::toString()
{
	return 0;
}

int ReadabledateSequence::toDecimal()
{
	return 0;
}

int ReadabledateSequence::toDate()
{
	return 0;
}


std::string NoRenameSequence::toBase10()
{
	Base10 base;
	m_str = base.toBase10(m_int, 3);
	return m_str;
};

std::string NoRenameSequence::toBase16()
{
	Base16 base;
	m_str = base.toBase16(m_int, 3);
	return m_str;
};

std::string NoRenameSequence::toBase32()
{
	Base32 base;
	m_str = base.toBase32(m_int, 2);
	return m_str;
};

std::string NoRenameSequence::toBase64()
{
	Base64 base;
	m_str = base.tobase64(m_int, 2);
	return m_str;
};

// No rename only changes the original file name if there is a file name clash.

/*
const char *NoRenameSequence::toSequence() {

	std::string lower_string;
	bool found = false;
	bool complete = false;
	int seqTotal = 1;

	std::string targetFilename = getFilenameNoExt(m_name.c_str());;
	std::filesystem::path p = m_path;
	uint32_t count = 0;
	if (p.empty() == true) {
		return nullptr;
	}
	try {
		std::error_code ec;
		while (!complete) {
			for (auto& entry : std::filesystem::directory_iterator(p, ec)) {
				//std::cout << entry;
				//printf("%s, %d", ec.message().c_str(), ec.value());
				if (ec.value() != 0) {
					continue;
				}
				if (std::filesystem::is_directory(entry, ec)) {
					continue;
				}
				if (std::filesystem::is_regular_file(entry, ec)) {
					std::string file = entry.path().stem().string();
					std::string name = getFilenameNoExt(file.c_str());
					if (name.compare(targetFilename) == 0) {
						found = true;
						break;
					}
				}
			}
			if (found) {
				found = false;
				targetFilename = m_name;
				std::string seqext = "_";
				seqext += std::to_string(seqTotal++);
				targetFilename += seqext;
			}
			else {
				complete = true;
			}
		}
	}
	catch (std::exception e)
	{
		printf("%s", e.what());
	}
	m_name = targetFilename;
	return m_name.c_str();
	
	switch (m_renameBase) {
	case RenameBase::Base10:
		m_str = toBase10();
		break;
	case RenameBase::Base16:
		m_str = toBase16();
		break;
	case RenameBase::Base32:
		m_str = toBase32();
		break;
	case RenameBase::Base64:
		m_str = toBase64();
		break;
	}
	return m_str.c_str();
}
*/

uint16_t DateSequence::days_from_civil(int y, unsigned m, unsigned d)
{

	y -= m <= 2;
	const int era = (y >= 0 ? y : y - 399) / 400;
	const unsigned yoe = y - era * 400;      // [0, 399]
	const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;  // [0, 365]
	const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;         // [0, 146096]
	return era * 146097 + doe - 719468;
}

uint16_t ImageSequence::fileCount(const char* path) {
	std::filesystem::path p = path;
	uint32_t count = 0;
	if (p.empty() == true) {
		return true;
	}
	try {
		std::error_code ec;
		for (auto& entry : std::filesystem::directory_iterator(p, ec)) {
			//std::cout << entry;
			//printf("%s, %d", ec.message().c_str(), ec.value());
			if (ec.value() != 0) {
				continue;
			}
			if (std::filesystem::is_directory(entry, ec)) {
				continue;
			}
			if (std::filesystem::is_regular_file(entry, ec)) {
				count++;
			}
			
		}
	}
	catch (std::exception e)
	{
		printf("%s", e.what());
	}
	return count;
}

#include <iostream>
#include <filesystem>


NoRenameSequence::NoRenameSequence(const char* p)
{
	std::filesystem::path path = p;
	m_file = path.filename().string();
	m_path = path.parent_path().string();
}


const char* NoRenameSequence::toSequence()
{
	std::string filename = fileNoExtention(m_file.c_str());
	bool found = false;
	std::filesystem::directory_iterator end_itr; // default construction yields past-the-end
	for (auto const& dir_entry : std::filesystem::directory_iterator{ m_path })
	{
		if (dir_entry.is_regular_file() == true) {
			std::filesystem::path p = dir_entry.path();
			std::filesystem::path f = p.filename();
			std::string fileItem = f.string();
			fileItem = fileNoExtention(fileItem.c_str());
			fileItem = fileNoRenameSeq(fileItem.c_str());
			if (filename == fileItem) {
				found = true;
			}

		}
	}
	if (found == true) {
		filename += '-';
		
		std::string versionStr = std::to_string(++m_max);
		if (versionStr.length() <= 1) {
			versionStr = '0' + versionStr;
		}
		filename += versionStr;
		//filename += '.';
		//filename += fileExtention(m_file.c_str());
		m_result = filename;
	}
	else
	{
		m_result = filename;
	}
	return m_result.c_str();
}


std::string NoRenameSequence::fileExtention(const char* f)
{
	std::string fullname = f;
	size_t lastindex = fullname.find_last_of(".");
	return fullname.substr(lastindex + 1, fullname.length() - (lastindex + 1));
}

std::string NoRenameSequence::fileNoExtention(const char* f)
{
	std::string fullname = f;
	size_t lastindex = fullname.find_last_of(".");
	return fullname.substr(0, lastindex);
}

std::string NoRenameSequence::fileNoRenameSeq(const char* f)
{
	std::string fullname = f;
	size_t lastindex = fullname.find_last_of("-");
	if (lastindex != std::string::npos) {
		std::string renameSeq = fullname.substr(lastindex + 1, fullname.length() - 1);
		int max = std::stoi(renameSeq);
		if (max > m_max) m_max = max;
	}
	return fullname.substr(0, lastindex);
}








/*
ImageSequencefactory& ImageSequencefactory::Get()
{
	static ImageSequencefactory imageSequencefactory;
	return imageSequencefactory;
}

std::unique_ptr<ImageSequence> make(RenameType type, RenameBase base, int y, int m, int d, uint32_t seq, const char* path)
{
	
	switch (type) {
	case RenameType::Sequence:
	{
		std::unique_ptr<NumberSequence> sequence = std::make_unique<NumberSequence>(seq);
		sequence->setBase(base);
		return sequence;
	}
	case RenameType::Date:
		return std::make_unique<DateSequence>(y, m, d, seq, path);
		break;
	case RenameType::Readabledate:
		//sequence = std::make_shared<ReadabledateSequence>(y, m, d, imagePath.c_str());
		break;
	case RenameType::NoRename:
		return std::make_unique<NoRenameSequence>(path);
		break;
	case RenameType::Unknown:
		return nullptr;
	}
	return nullptr;
}
*/
