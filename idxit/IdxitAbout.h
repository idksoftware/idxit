#pragma once

#include "IdxitArgvParser.h"

using namespace CommandLineProcessing;



	class IdxitAbout : public IdxitSubCommand {
	public:
		IdxitAbout(IdxitArgvParser& argvParser) : IdxitSubCommand("about", argvParser) {};
	protected:
		bool doCommand() override;
	};
