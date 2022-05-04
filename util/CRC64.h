#pragma once
#include <inttypes.h>
#include <stdio.h>

class CRC64
{
public:
	CRC64() = default;
	~CRC64() = default;
	uint64_t crc64(FILE* fp);
	uint64_t crc64(const char* szPath, bool *status);
	uint64_t crc64(uint64_t crc, unsigned char* buf, size_t len);
};

