
#include <vector>
#include "Base16.h"
#include <algorithm>

static char base10[] = "0123456789";

static int LookupDigitsDec[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,           //gap: spc,!"#$%'()*
	-1,                   // +
	-1, -1, -1,             // gap ,-.
	-1,                   // /
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, // 0-9
	-1, -1, -1,             // gap: :;<
	-1,                   //  = (end padding)
	-1, -1, -1,             // gap: >?@
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,-1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, // A-Z
	-1, -1, -1, -1, -1, -1,    // gap: [\]^_`
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1, // a-z    
	-1, -1, -1, -1,          // gap: {|}~ (and the rest...)
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

const char* Base10::toBase10(long dec, int padding)
{
	std::vector<char> list;
	m_str.clear();

	int rem = 0;
	if (dec > 0) {
		while (dec > 0) {
			rem = dec % 10;
			list.push_back(base10[rem]);
			dec = dec / 10;
		}
	}
	else {
		list.push_back(base10[0]);
	}

	std::reverse(list.begin(), list.end());
	for (auto i = list.begin(); i != list.end(); i++) {
		const char c = *i;
		m_str += c;
	}
	if (padding != 0) {
		padToBase10(m_str, padding);
	}
	return m_str.c_str();
}


uint32_t Base10::toDecimal(std::string encoded)
{
	int ret = 0;
	unsigned char c = 0;
	unsigned int num = 0;
	const size_t len = encoded.length();
	for (unsigned int index = 0; index < encoded.length(); index++) {
		c = encoded[index];
		if (c >= sizeof(LookupDigitsDec)) {
			return -1;
		}
		num = LookupDigitsDec[c];
		if (num == -1) {
			return -1;
		}
		ret = (10 * ret) + num;
	}
	return ret;
}



static char base16[] = "0123456789ABCDEF";

static int LookupDigitsHex[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, //gap: ctrl chars
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,           //gap: spc,!"#$%'()*
	-1,                   // +
	-1, -1, -1,             // gap ,-.
	-1,                   // /
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, // 0-9
	-1, -1, -1,             // gap: :;<
	-1,                   //  = (end padding)
	-1, -1, -1,             // gap: >?@
	10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1,-1,-1,-1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, // A-Z
	-1, -1, -1, -1, -1, -1,    // gap: [\]^_`
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1, // a-z    
	-1, -1, -1, -1,          // gap: {|}~ (and the rest...)
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};



const char* Base16::toBase16(long dec, int padding) {

	std::vector<char> list;
	m_str.clear();
	
	int rem = 0;
	if (dec > 0) {
		while (dec > 0) {
			rem = dec % 16;
			list.push_back(base16[rem]);
			dec = dec / 16;
		}
	}
	else {
		list.push_back(base16[0]);
	}
	
	std::reverse(list.begin(), list.end());
	for (auto i = list.begin(); i != list.end(); i++) {
		const char c = *i;
		m_str += c;
	}
	if (padding != 0) {
		padToBase16(m_str, padding);
	}
	return m_str.c_str();
}

int32_t Base16::toDecimal(std::string encoded)
{
	int ret = 0;
	unsigned char c = 0;
	unsigned int num = 0;
	const size_t len = encoded.length();
	for (unsigned int index = 0; index < encoded.length(); index++) {
		c = encoded[index];
		if (c >= sizeof(LookupDigitsHex)) {
			return -1;
		}
		num = LookupDigitsHex[c];
		if (num == -1) {
			return -1;
		}
		ret = (16 * ret) + num;
	}
	return ret;
}
