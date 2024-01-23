#include "IdxitProperties.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IdxitAppOptions.h"
#include "SAUtils.h"


using namespace CommandLineProcessing;



	bool IdxitProperties::doCommand() {
		IdxitAppOptions& appOptions = IdxitAppOptions::get();
		appOptions.setCommandMode(IdxitAppOptions::CommandMode::CM_Prop);

		bool isSourePathSet = false;
		if (getParser().foundOption("source-path") == true) {
			std::string opt = getParser().optionValue("source-path");
			appOptions.m_sourcePath = opt;
		}
		else {
			//config.setSourcePath(SAUtils::getCurrentDirectory().c_str());
			appOptions.m_sourcePath = SAUtils::getCurrentDirectory();
		}
		return true;
	}