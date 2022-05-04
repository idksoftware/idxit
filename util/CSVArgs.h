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
#include <vector>

#include "siaglobal.h"

namespace simplearchive {

	class CSVArgs : public std::vector<std::string> {
		char m_delim;
	public:
		CSVArgs(char delim = '.');
		virtual ~CSVArgs();
		bool process(const char *dataString);
		bool process(const std::string &data);
		void print();
		/// @brief This will get an optional argument empty if no argument
		/// found.
		std::string getOptional(int pos) {
			if ((size_t)pos >= size()) {
				return "";
			}
			else {
				return at(pos);
			}
		}

		std::istream& nextField(std::istringstream& s, std::string& field);
	};

} /* namespace simplearchive */

