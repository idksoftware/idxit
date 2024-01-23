
#pragma once

#include "IdxitArgvParser.h"

using namespace CommandLineProcessing;



	class IdxitProperties : public IdxitSubCommand {
	public:
		IdxitProperties(IdxitArgvParser& argvParser) : IdxitSubCommand("prop", argvParser) {};
	protected:
		bool doCommand() override;
	};
