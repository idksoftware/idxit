#include "IdxitScan.h"

#include "AppConfig.h"
#include "SAUtils.h"
#include "Quiet.h"
#include "ConfigReader.h"
#include "IdxitAppOptions.h"
#include "ParseOptions.h"
#include "ConfigReader.h"
#include "ResultsPresentation.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IdxitScan::doCommand() {

		IDXITAppConfig config;
		IdxitAppOptions& appOptions = IdxitAppOptions::get();

		bool isSourePathSet = false;
		if (getParser().foundOption("source-path") == true) {
			std::string opt = getParser().optionValue("source-path");
			
		}
		if (isSourePathSet == false) {
			//config.setSourcePath(SAUtils::getCurrentDirectory().c_str());
		}

		if (getParser().foundOption("file") == true) {
			std::string opt = getParser().optionValue("file");
			appOptions.m_filePath = opt;
		}

		if (getParser().foundOption("format-type") == true) {
			std::string opt = getParser().optionValue("format-type");
			if ((appOptions.m_formatType = ResultsPresentation::parse(opt.c_str())) == ResultsPresentation::FormatType::unknown) {
				printf("Invalid argument for \"FormatType\" \"%s\"\n\n", opt.c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
		}

		appOptions.setCommandMode(IdxitAppOptions::CommandMode::CM_Scan);
		return true;
	}
}