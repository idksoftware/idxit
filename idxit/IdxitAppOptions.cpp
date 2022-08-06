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
**	Filename	: IdxitAppOptions.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include "ConfigReader.h"
#include "IdxitAppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "IdxitArgvParser.h"
#include "Environment.h"
#include "EnvFunc.h"
#include "ResultsPresentation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using namespace CommandLineProcessing;
namespace simplearchive {

bool IdxitAppOptions::m_list = false;

bool IdxitAppOptions::m_peekOnly = false;
bool IdxitAppOptions::m_eventsOn = false; // UDP events
bool IdxitAppOptions::m_serverOn = true; // false;
bool IdxitAppOptions::m_forceDate = false; // false;
bool IdxitAppOptions::m_force = false; // false;

bool IdxitAppOptions::m_sync = false;
bool IdxitAppOptions::m_current = false;
bool IdxitAppOptions::m_master = false;



std::string IdxitAppOptions::m_name;
IdxitAppOptions::CommandMode IdxitAppOptions::m_commandMode = IdxitAppOptions::CommandMode::CM_Unknown;
std::string IdxitAppOptions::m_comment;

std::string IdxitAppOptions::m_sourcePath;
std::string IdxitAppOptions::m_distinationPath;
std::string IdxitAppOptions::m_filePath;
std::string IdxitAppOptions::m_indexfile;
std::string IdxitAppOptions::m_ignoreFile;
bool IdxitAppOptions::m_nousys = false;
bool IdxitAppOptions::m_nouser = false;
bool IdxitAppOptions::m_nosys = false;
bool IdxitAppOptions::m_scanHidden = false;
std::string IdxitAppOptions::m_incGroupFile;
std::string IdxitAppOptions::m_excGroupFile;

std::string IdxitAppOptions::m_version = "@";
std::string IdxitAppOptions::m_FromDate;
std::string IdxitAppOptions::m_ToDate;
std::string IdxitAppOptions::m_option;
std::string IdxitAppOptions::m_value;
std::string IdxitAppOptions::m_outputFile;
std::string IdxitAppOptions::m_textOutputType;
ResultsPresentation::FormatType IdxitAppOptions::m_formatType = ResultsPresentation::FormatType::csv;
IdxitAppOptions::ShowCommandOption IdxitAppOptions::m_showCommandOption;

int IdxitAppOptions::m_udpPortNum = 64321;
std::string IdxitAppOptions::m_udpAddress = "127.0.0.1";
int IdxitAppOptions::m_tcpPortNum = 64322;
//ResultsPresentation::FormatType IdxitAppOptions::m_formatType = ResultsPresentation::FormatType::Human;


IdxitAppOptions::IdxitAppOptions() : m_argvParser(std::make_unique<IdxitArgvParser>()) {
	
};


DefaultArgumentsContainer IdxitAppOptions::defaultArgumentsContainer;


IdxitAppOptions::~IdxitAppOptions() {}



IdxitAppOptions::CommandMode IdxitAppOptions::getCommandMode() {
	return m_commandMode;
}


const char *IdxitAppOptions::getName() {
	return m_name.c_str();
}

void IdxitAppOptions::setCommandMode(const IdxitAppOptions::CommandMode mode)
{
	m_commandMode = mode;
}

void IdxitAppOptions::setName(const char *name) {
	m_name = name;
}

const char *IdxitAppOptions::getComment() {
	return m_comment.c_str();
}

const char* IdxitAppOptions::getVersions() {
	return m_version.c_str();
}

const char* IdxitAppOptions::getSourcePath() {
	return m_sourcePath.c_str();
}

const char *IdxitAppOptions::getDistinationPath() {
	return m_distinationPath.c_str();
}
const char *IdxitAppOptions::getFilePath() {
	return m_filePath.c_str();
}

const char* IdxitAppOptions::getFromDate()
{
	return m_FromDate.c_str();
}

const char* IdxitAppOptions::getToDate()
{
	return m_ToDate.c_str();
}

const char* IdxitAppOptions::getOption()
{
	return m_option.c_str();
}

const char* IdxitAppOptions::getValue()
{
	return m_value.c_str();
}

bool IdxitAppOptions::isEnventsOn() {
	return m_eventsOn;
}

int IdxitAppOptions::eventPort() {
	return m_udpPortNum;
	
}
const char *IdxitAppOptions::eventAddress() {
	return m_udpAddress.c_str();
}

bool IdxitAppOptions::isServerOn() {
	return m_serverOn;
}

int IdxitAppOptions::serverPort() {
	return m_tcpPortNum;

}


bool IdxitAppOptions::isDataForced() {
	return m_forceDate;
}

bool IdxitAppOptions::isForced() {
	return m_force;
}

bool IdxitAppOptions::isSync()
{
	return m_sync;
}

bool IdxitAppOptions::isCurrent()
{
	return m_current;
}

bool IdxitAppOptions::isMaster()
{
	return m_master;
}

const char* IdxitAppOptions::getOutputFile()
{
	return m_outputFile.c_str();
}
const char* IdxitAppOptions::getTextOutputType()
{
	return m_textOutputType.c_str();
}


void IdxitAppOptions::setDefaultArguments(std::string s) {
	defaultArgumentsContainer.push_back(s);
}

DefaultArgumentsContainer& IdxitAppOptions::getDefaultArguments() {
	return defaultArgumentsContainer;
}

//ResultsPresentation::FormatType& IdxitAppOptions::getFormatType() {
//	return m_formatType;
//}


bool StatusOptions::parse(const char* optionString)
{
	if (iequals(optionString, STATUS_CHECKED_OUT)) {
		m_option = STATUS_CHECKED_OUT;
		IdxitAppOptions::m_showCommandOption = IdxitAppOptions::ShowCommandOption::SC_ShowCheckedOut;
		return true;
	}
	else if (iequals(optionString, STATUS_CHECKED_IN)) {
		m_option = STATUS_CHECKED_IN;
		IdxitAppOptions::m_showCommandOption = IdxitAppOptions::ShowCommandOption::SC_ShowCheckedIn;
		return true;
	}
	else if (iequals(optionString, STATUS_DELETED)) {
		m_option = STATUS_DELETED;
		return true;
	}
	return false;
}


} /* namespace simplearchive */
