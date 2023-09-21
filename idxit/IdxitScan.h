
#pragma once

#include "IdxitArgvParser.h"

using namespace CommandLineProcessing;



	class IdxitScan : public IdxitSubCommand {
	public:
		IdxitScan(IdxitArgvParser& argvParser) : IdxitSubCommand("scan", argvParser) {};
	protected:
		bool doCommand() override;
	};

