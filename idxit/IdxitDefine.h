
#pragma once

#include "IdxitArgvParser.h"

using namespace CommandLineProcessing;



	class IdxitDefinition : public IdxitSubCommand {
	public:
		IdxitDefinition(IdxitArgvParser& argvParser) : IdxitSubCommand("define", argvParser) {};
	protected:
		bool doCommand() override;
	};

