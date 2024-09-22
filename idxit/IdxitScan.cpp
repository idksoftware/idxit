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



	bool IdxitScan::doCommand() {

		IDXITAppConfig config;
		IdxitAppOptions& appOptions = IdxitAppOptions::get();

		bool isSourePathSet = false;
		if (getParser().foundOption("source-path") == true) {
			std::string opt = getParser().optionValue("source-path");
			appOptions.m_sourcePath = opt;
		} else {
			//config.setSourcePath(SAUtils::getCurrentDirectory().c_str());
			appOptions.m_sourcePath = SAUtils::getCurrentDirectory();
		}

		if (getParser().foundOption("index-file") == true) {
			std::string opt = getParser().optionValue("index-file");
			appOptions.m_indexfile = opt;
		}
		else {
			appOptions.m_indexfile = SAUtils::getCurrentDirectory() + "/default.idx";
		}

		if (getParser().foundOption("ign") == true) {
			std::string opt = getParser().optionValue("ign");
			appOptions.m_ignoreFile = opt;
		}

		appOptions.m_scanHidden = false;
		if (getParser().foundOption("scan-hidden") == true) {
			std::string opt = getParser().optionValue("scan-hidden");
			BoolOption bo = SAUtils::isTrueFalse(opt);
			if (bo == BoolOption::Invalid) {
				printf("Invalid argument for \"scan-hidden\" \"%s\"\n\n", opt.c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.m_scanHidden = SAUtils::boolOptionValue(bo);
		}

		appOptions.m_nousys = false;
		if (getParser().foundOption("nousys") == true) {
			std::string opt = getParser().optionValue("nousys");
			BoolOption bo = SAUtils::isTrueFalse(opt);
			if (bo == BoolOption::Invalid) {
				printf("Invalid argument for \"nousys\" \"%s\"\n\n", opt.c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.m_nousys = SAUtils::boolOptionValue(bo);
		}

		appOptions.m_nouser = false;
		if (getParser().foundOption("nouser") == true) {
			std::string opt = getParser().optionValue("nouser");
			BoolOption bo = SAUtils::isTrueFalse(opt);
			if (bo == BoolOption::Invalid) {
				printf("Invalid argument for \"nousys\" \"%s\"\n\n", opt.c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.m_nouser = SAUtils::boolOptionValue(bo);
		}

		appOptions.m_nosys = false;
		if (getParser().foundOption("nosys") == true) {
			std::string opt = getParser().optionValue("nosys");
			BoolOption bo = SAUtils::isTrueFalse(opt);
			if (bo == BoolOption::Invalid) {
				printf("Invalid argument for \"nousys\" \"%s\"\n\n", opt.c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.m_nosys = SAUtils::boolOptionValue(bo);
		}

		if (getParser().foundOption("quick") == true) {
			appOptions.m_quick = true;
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
