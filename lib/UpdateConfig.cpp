
#include <string>
#include <iostream>
#include <fstream>
#include "SAUtils.h"
#include "AppConfig.h"
#include "ConfigReader.h"
//#include "EnvFunc.h"
//#include "AppBase.h"
#include "HomePaths.h"
#include "UpdateConfig.h"



	bool UpdateConfig::read(AppConfigBase&imgaConfig)
	{
		
		/*
		const std::string key = "IMGARCHIVE_HOME";
		std::string temp = SAUtils::GetPOSIXEnv(key);
		std::string homePath = temp
		*/
		
		IdxItHome& idxItHome = IdxItHome::getObject();
		
		HomePathType homePathType = idxItHome.type();
		if (idxItHome.isValid() == false) {
			printf("Archive not found at default loacation");
			return false;
		}

		switch (homePathType) {
		case HomePathType::LocalEnv:	// Local Environment set
			
			//printf("Found IMGARCHIVE_HOME as system profile: %s. Archive found at that loacation", HomePath::get().c_str());
			break;
		case HomePathType::SystemEnv:	// System Environment set
			
			break;
		case HomePathType::UserOnly:	// user only archive
			
			//printf("Archive found at default user loacation: %s.", HomePath::get().c_str());
			break;
		case HomePathType::AllUsers:	// all users archive
			
			//printf("Archive found at default system loacation: %s.", HomePath::get().c_str());
			break;
		case HomePathType::Unknown:
		default:
			//printf("Unknown error");
			return false;
		}
		const std::string &s_homePath = IdxItHome::getIdxItHome();
		
		std::string homePath = s_homePath;
		m_configfile = homePath + "/config/" + "config.dat";
		if (SAUtils::FileExists(m_configfile.c_str()) == false) {

			//printf("IdxIt Unable to start? No config.dat file found in the default location or"
			//	" the environment variable IDXIT_HOME not set.\nUse siaadmin to initalise an archive.\n");
			return false;
		}
		AppConfigReader configReader;
		configReader.setNoLogging();
		configReader.read(m_configfile.c_str(), imgaConfig);
		
		
		return true;
	}
	bool UpdateConfig::update(const char* configOptionBlock, const char* configOption, const char* configValue)
	{
		IDXITAppConfig imgaConfig;
		if (read(imgaConfig) == false) {
			return false;
		}
		AppConfigBaseWriter configWriter(imgaConfig);
		configWriter.update(configOptionBlock, configOption, configValue);
		//imgaConfig.printAll();
		std::ofstream configFile;
		configFile.open(m_configfile.c_str());
		//configFile.open("C:\\temp\\t.dat");
		if (configFile.bad() || configFile.fail()) {
			return false;
		}
		configFile << imgaConfig;
		//std::cout << imgaConfig;
		configFile.close();
		
		return true;
	}

