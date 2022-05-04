#include "IdxitAbout.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IdxitAppOptions.h"
#include "IdxitParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive
{
	bool IdxitAbout::doCommand()
	{
		IdxitAppOptions& appOptions = IdxitAppOptions::get();
		if (getParser().foundOption("out") == true)
		{
			OutputType outputType;
			std::string outType = getParser().optionValue("out");
			if (outputType.parse(getParser().optionValue("out").c_str()) == false)
			{
				ReturnCodeObject::setReturnString("Option for argument \"out\" for sub-command: %s is invalid: %s\n\n",
				                                  getParser().getCurrentCommand().c_str(),
				                                  getParser().optionValue("out").c_str());
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
}
