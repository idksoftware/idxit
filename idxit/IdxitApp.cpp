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
**	Filename	: App.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include <csignal>
#include <cstdlib>
#include <cstdio>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <stdio.h>
#include <iostream>

#include "IdxitApp.h"
#include "SAUtils.h"
#include "CLogger.h"
#include "AppConfig.h"
//#include "TargetsList.h"
#include "CIDKDate.h"
#include "AboutCommand.h"
#include "SummaryFile.h"
#include "Lib.h"
#include "UDPOut.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#include <iostream>
using namespace std;

#include <string>
#include <vector>
#include "AppOptions.h"
#include "IdxitAppOptions.h"
#include "IdxitArgvParser.h"
//#include "Threads.h"
//#include "HookCmd.h"
//#include "ImagePath.h"
#include "HomePaths.h"
//#include "UpdateTemplateManager.h"
//#include "DefaultEnvironment.h"






/*
java version "1.7.0_51"
Java(TM) SE Runtime Environment (build 1.7.0_51-b13)
Java HotSpot(TM) 64-Bit Server VM (build 24.51-b03, mixed mode)
*/


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif




using namespace CommandLineProcessing;

IdxitApp::IdxitApp(const char* appName) : AppBase(appName, std::make_shared<IdxitArgvParser>()) {};

class CheckoutSummary : public SummaryReporter {
protected:
	virtual bool doProcess() {
		std::stringstream str;
		//int warnings = 0;
		//int errors = 0;
		//int competed = 0;
		std::shared_ptr<StatusList> list = getList();
		for (auto i = list->begin(); i != list->end(); i++) {
			//ReporterEvent& item = *i;
			
		}
		setSummary(str.str().c_str());

		return false;
	}
};

bool IdxitApp::about(const char* outputType, const char* filename) {
	AboutCommand aboutCommand("client", "Idxit", VERSION, BUILD);
	aboutCommand.setOutputFile(filename);
	aboutCommand.setTextOutputType(outputType);

	if (aboutCommand.process() == false) {
		return false;
	}

	return true;
}

bool IdxitApp::initaliseArgs(int argc, char **argv) {
	
	if (m_argvParser->doInitalise(argc, argv) == false) {
		return false;
	}

	return true;
}

bool IdxitApp::initaliseConfig() {


	IDXITAppConfig config;

	IdxItHome& idxItHome = IdxItHome::getObject();
	idxItHome.init();
	if (idxItHome.isValid() == false) {
		setError(13, "Error found setting Index-it home folder: %s.\n", idxItHome.errorStr().c_str());
		return false;
	}
	// Initalise without the config file i.e. set defaults.
	
	// try to set a systems temp folder 
	std::string tempPath = SAUtils::GetPOSIXEnv("TEMP");
	if (tempPath.empty() == true || tempPath.length() == 0) {
		tempPath = SAUtils::GetPOSIXEnv("TMP");
	}
	std::string homePath = IdxItHome::getIdxItHome();
	AppConfig::setDefaultLocations();
	std::string configfile = homePath + "/config/" + "config.dat";
	std::string configPath = homePath + "/config";
	// Now set the file based configuration with the possablity of overrighting defaults set prevously. 
	if (SAUtils::FileExists(configfile.c_str()) == false) {
		setError(13, "Error configuration file not found at location %s\n", configPath.c_str());
		return false;
	}
	setConfigPath(configPath.c_str());
	AppConfigReader configReader;
	configReader.setNoLogging();
	if (configReader.read(configfile.c_str(), config) == false) {
		setError(13, "Error found at line %d in the configuration file.\n", configReader.getCurrentLineNumber());
		return false;
	}
	config.fileBasedValues(homePath.c_str(), tempPath.c_str());
	setConfigured();
	idxItHome.checkAndMakePaths();
	
	return true;
}


bool IdxitApp::doRun()
{
	IDXLib idxLib;

	IdxitAppOptions &appOptions = IdxitAppOptions::get();
	AppConfig &config = AppConfig::get();
	
	
	// Set global options
	if (appOptions.isEnventsOn() == true) {

		//siaLib.enableEvents(appOptions.eventAddress(), appOptions.eventPort());
	}

	if (appOptions.isServerOn() == true) {

		//siaLib.enableServer(appOptions.eventPort());
	}
	
	//printf("%s", config.toString().c_str());
	config.settup();
	
	if (isConfiguratedOk() == false) {
		setError(12, "IdxIt Unable to start? Archive not found at the specified location \"%s\".\n"
			"Use iaadmin -i to create an empty archive at the default location (see documentation).\n", config.getHomePath());
		return false;
	}
	
	
	if (idxLib.initalise(APP_NAME) < 0) {
		return false;
	}
	/*
		INITALISING COMLETE ????
	*/
	bool noSummary = false; // Some commands do not require summery as it breaks some output i.e. XML 
	switch (appOptions.getCommandMode()) {
	case IdxitAppOptions::CommandMode::CM_About:
	{
		return about(appOptions.getTextOutputType(), appOptions.getOutputFile());
	}
	case IdxitAppOptions::CommandMode::CM_Scan:
	{
		IDXLib idxLib;
		idxLib.initalise("Idxit");
		return idxLib.scan(appOptions.getSourcePath(), appOptions.getIndexfile(), appOptions.getignoreFile(),
													appOptions.getnousys(), appOptions.getnouser(), appOptions.getnosys(),
													appOptions.getIncGroupFile(), appOptions.getExcGroupFile());
	}

	case IdxitAppOptions::CommandMode::CM_Prop:
	{
		IDXLib idxLib;
		idxLib.initalise("Idxit");
		return idxLib.prop(appOptions.getSourcePath());
	}

	case IdxitAppOptions::CommandMode::CM_Sort:
		//IDXLib idxLib;

		//return idxLib.sort();
		break;
	case IdxitAppOptions::CommandMode::CM_Unknown:
		setError( CLogger::getLastCode(), CLogger::getLastMessage());
		break;
	default:
		setError(15, "Command not implimented at this time");
		return false;
	}
	if (noSummary == false) {
		//siaLib.complete();
	}
	
	return true;
}

bool IdxitApp::setExitStatus(bool commandReturn)
{
	setError(CLogger::getLastCode(), CLogger::getLastMessage());
	CLogger::Level level = CLogger::getHighestLevel();
	ExitCode exitCode = ExitCode::Fatal;
	switch (level)
	{
	case CLogger::Level::TRACE:		// Trace debugging
	case CLogger::Level::FINE:		// Fine debugging
	case CLogger::Level::INFO:		// General information
	case CLogger::Level::STATUS:		// Status information
	case CLogger::Level::SUMMARY:	// Summary information
		exitCode = ExitCode::Success;
		break;
	case CLogger::Level::WARNING:	// Warning of incomplete operations
		exitCode = ExitCode::Warnings;
		break;
	case CLogger::Level::ERR:		// Failed operations
		exitCode = ExitCode::Errors;
		break;
	case CLogger::Level::FATAL:		// Application cannot complete
	case CLogger::Level::UNKNOWN:
		exitCode = ExitCode::Fatal;
		break;
	}
	setExitCode(exitCode);
	return commandReturn;
}

IdxitApp::~IdxitApp() {
	CLogger::Close();
}

bool ok()
{
	cout << "OK" << endl;
	return(true);
}

bool failed()
{
	cout << "Failed" << endl;
	return(false);
}




void ctrlHandler(int s) {
	printf("Caught signal %d\n", s);
	//exit(1);

}

int main(int argc, char** argv)
{

	signal(SIGINT, ctrlHandler);

	bool error = false;
	IdxitApp app("Idx-it");
	if (app.initalise(argc, argv) == false) {

		error = true;
	}
	else {
		if (app.Run() == false) {
			error = true;
		}
	}
	if (error) {
		int code = CommandLineProcessing::AppBase::getError();
		cout << "\n\n" << CommandLineProcessing::AppBase::getFullErrorString() << '\n';
		//Sleep(3000);
		return code;
	}
	//Sleep(3000);
	return 0;

}


