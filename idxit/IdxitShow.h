
#pragma once

#include "IdxitArgvParser.h"

using namespace CommandLineProcessing;



	class IdxitShow : public IdxitSubCommand {
	public:
		IdxitShow(IdxitArgvParser& argvParser) : IdxitSubCommand("show", argvParser) {};
	protected:
		bool doCommand() override;
	};
