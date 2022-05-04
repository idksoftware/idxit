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

#include <iostream>
#include "ExternalComand.h"
#include "ExternalShell.h"
#include "ConfigReader.h"
#include "SAUtils.h"
#include "CLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

/*! \exif_mainpage Shell and Configuration file components
*
* \section intro_sec Introduction
*
*	Sia provides a simple EXIF reader that can read EXIF data out of jpgs and possibly other image
*	types.  However EXIF data may be present in images that the internal reader may not be able to
*	decode.
*	There are external tools that may able to decode the EXIF information is images such as
*	ExifTool by Phil Harvey and Exiv2 project (www.exiv2.org). Sia enables these tools to be
*	integrated into the work-flow when adding images into the archive.
*	This extracted data is seamlessly inserted into the normal metadata that is archived with the image
*	as if, the internal reader extracted it.
*	Three external tools are supported :
*
*	Exiftool
*	Exiv2
*	Python Image Library.
*
*	The Python Image Library is a python library that can be used to extract EXIF ImgArchive product provides a python
*	sample that intergrates into Sia. This camn be further extended as requited.
*
* \section external_tool_components_sec External Tool Components
*
* These components are used control external tools by a set of configuration files that provides the appropriate
* command line for the tool and key mapping for the output of the subsequently generated metadata. The output from
* an external command can be in two forms:
* 1) An External Output file that is read by Img.
* 2) Reading the output from the external tool from the command line.
*
*
*
*
*
*
*
*/

ExternalComand::ExternalComand(const char *tempPath) {
	m_tempPath = tempPath;
}

ExternalComand::~ExternalComand() {
	// TODO Auto-generated destructor stub
}

bool ExternalComand::init(const char *externalCommandLine, const char *exifMapPath) {
	m_commandLine = externalCommandLine;
	m_exifMapPath = exifMapPath;
	return true;
}

// python exif.py [input] [output]



std::string ExternalComand::process(const char *path, const char* file) {
	CLogger &logger = CLogger::getLogger();
	
	std::string imagefile = path;
	imagefile += '/';
	imagefile += file;

	std::string in = "\""; in += imagefile; in += "\"";

	ExternalShell externalShell;
	std::string returnStr;
	if (findToken(m_commandLine, "exif") == false) {
		std::string cmd = replace(m_commandLine, in);
		logger.log(LOG_OK, CLogger::Level::FINE, "Using Exif command: \"%s\"", cmd.c_str());
		externalShell.exec(cmd.c_str());
		logger.log(LOG_OK, CLogger::Level::FINE, "Reading Exif map file \"%s\"", m_exifMapPath.c_str());
		
		returnStr = externalShell.getOutput();
	}
	else {
		std::string out = m_tempPath;
		out += '/';
		out += SAUtils::getFilenameNoExt(file);
		
		out.append(".exf");
		std::string cmd = replace(m_commandLine, in, out);
		logger.log(LOG_OK, CLogger::Level::FINE, "Using Exif command: \"%s\"", cmd.c_str());
		
		externalShell.exec(cmd.c_str());
		logger.log(LOG_OK, CLogger::Level::FINE, "Exif command output: \"%s\"", externalShell.getOutput().c_str());
		
		
		
		returnStr = out.c_str();
	}
	return returnStr;
}

std::string ExternalComand::replaceToken(std::string &str, const char *toklabel, const char *repstr) {
	size_t s = str.find_first_of("[");
	if (s == -1) {
		return str;
	}
	std::string tok = toklabel;
	std::string tokstr = str.substr(s,tok.length());
	if (tokstr.compare(tok) != 0) {
		return str;
	}
	str.replace(s,tok.length(), repstr);
	return str;
}

std::string ExternalComand::replace(std::string &commandLine, std::string &in, std::string &out) {
	CLogger &logger = CLogger::getLogger();
	std::string command = commandLine;
	command = replaceToken(command, "[source]", in.c_str());
	command = replaceToken(command, "[exif]", out.c_str());
	logger.log(LOG_OK, CLogger::Level::FINE, "Exif command line to be executed \"%s\"", command.c_str());
	return command;
}

std::string ExternalComand::replace(std::string &commandLine, std::string &in) {
	CLogger &logger = CLogger::getLogger();
	std::string command = commandLine;
	command = replaceToken(command, "[source]", in.c_str());
	logger.log(LOG_OK, CLogger::Level::FINE, "Exif command line to be executed \"%s\"", command.c_str());
	return command;
}

bool ExternalComand::findToken(std::string &str, const char *toklabel) {
	std::string tstr = str;
	while (1) {
		size_t pos = tstr.find_first_of("[");
		if (pos == -1) {
			return false;
		}
		std::string tok = toklabel;
		std::string tokstr = tstr.substr(pos+1, tok.length());
		if (tokstr.compare(tok) == 0) {
			return true;
		}
		tstr = tstr.substr(pos + tok.length());
	}
	return true;
}

} /* namespace simplearchive */
