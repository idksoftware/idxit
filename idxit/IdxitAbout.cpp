#include "IdxitAbout.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IdxitAppOptions.h"
#include "IdxitParseOptions.h"

using namespace CommandLineProcessing;


	bool IdxitAbout::doCommand()
	{
		IdxitAppOptions& appOptions = IdxitAppOptions::get();
		if (getParser().foundOption("format-type") == true)
		{
			OutputType outputType;
			std::string outType = getParser().optionValue("format-type");
			if (outputType.parse(getParser().optionValue("format-type").c_str()) == false)
			{
				ReturnCodeObject::setReturnString("Option for argument \"format-type\" for sub-command: %s is invalid: %s\n\n",
				                                  getParser().getCurrentCommand().c_str(),
				                                  getParser().optionValue("format-type").c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			else
			{
				appOptions.m_textOutputType = getParser().optionValue("out");
			}
		}
		if (getParser().foundOption("file") == true)
		{
			appOptions.m_outputFile = getParser().optionValue("file");
		}
		appOptions.setCommandMode(IdxitAppOptions::CommandMode::CM_About);
		return true;
	}

