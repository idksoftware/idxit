/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#pragma once
#include <string>
#include "SAUtils.h"

/// This class defines the Extention type (System, User).
class ExtentionType {
public:
	enum class Type {
		UNKNOWN_EXT = -1,
		SYSTEM_EXT,
		USER_EXT,
	};
private:
	Type m_type;
public:
	ExtentionType() {
		m_type = Type::UNKNOWN_EXT;
	}
	ExtentionType(Type type) {
		m_type = type;
	}
	std::string toString() {
		switch (m_type) {
		case Type::UNKNOWN_EXT:
			return "Unknown";
		case Type::SYSTEM_EXT:
			return "System";
		case Type::USER_EXT:
			return "User";
		}
		return "Unknown";
	}
	bool fromString(const char *str) {
		std::string s = str;
		
		if (SAUtils::isEquals(s, "unknown")) {
			m_type = Type::UNKNOWN_EXT;
			return true;
		} else
		if (SAUtils::isEquals(s, "system")) {
			m_type = Type::SYSTEM_EXT;
			return true;
		} else
		if (SAUtils::isEquals(s, "user")) {
			m_type = Type::USER_EXT;
			return true;
		} else {
			m_type = Type::UNKNOWN_EXT;
		}
		return false;
	}

	Type getType() const {
		return m_type;
	}

	void setType(Type type) {
		m_type = type;
	}

	ExtentionType operator=(const char *str) {
		fromString(str);
		return *this;
	}
};
