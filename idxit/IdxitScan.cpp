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
			appOptions.m_sourcePath = opt;
		}
		if (isSourePathSet == false) {
			//config.setSourcePath(SAUtils::getCurrentDirectory().c_str());
		}

		if (getParser().foundOption("index-file") == true) {
			std::string opt = getParser().optionValue("index-file");
			appOptions.m_indexfile = opt;
		}

		if (getParser().foundOption("ign") == true) {
			std::string opt = getParser().optionValue("ign");
			appOptions.m_ignoreFile = opt;
		}

		if (getParser().foundOption("nousys") == true) {
			std::string opt = getParser().optionValue("nousys");
			appOptions.m_nousys = opt;
		}

		if (getParser().foundOption("nouser") == true) {
			std::string opt = getParser().optionValue("nouser");
			appOptions.m_nouser = opt;
		}

		if (getParser().foundOption("nosys") == true) {
			std::string opt = getParser().optionValue("nosys");
			appOptions.m_nosys = opt;
		}

		if (getParser().foundOption("inc-group") == true) {
			std::string opt = getParser().optionValue("inc-group");
			appOptions.m_incGroupFile = opt;
		}

		if (getParser().foundOption("exc-group") == true) {
			std::string opt = getParser().optionValue("exc-group");
			appOptions.m_excGroupFile = opt;
		}

		if (getParser().foundOption("show-def") == true) {
			std::string opt = getParser().optionValue("show-def");
			appOptions.m_outputFile = opt;
		}

		if (getParser().foundOption("show-def-all") == true) {
			std::string opt = getParser().optionValue("show-def-all");
			appOptions.m_outputFile = opt;
		}

		if (getParser().foundOption("def-file") == true) {
			std::string opt = getParser().optionValue("def-file");
			appOptions.m_outputFile = opt;
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

			m_indexfile = opt;
			appOptions.m_ignoreFile = opt;
		
			appOptions.m_nousys = opt;
		
			appOptions.m_nouser = opt;
		
			appOptions.m_nosys = opt;
		
			appOptions.m_incGroupFile = opt;
		
			appOptions.m_excGroupFile = opt;