
#pragma once

#include "IdxitArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IdxitScan : public IdxitSubCommand {
	public:
		IdxitScan(IdxitArgvParser& argvParser) : IdxitSubCommand("scan", argvParser) {};
	protected:
		bool doCommand() override;
	};

};
