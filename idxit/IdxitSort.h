
#pragma once

#include "IdxitArgvParser.h"

using namespace CommandLineProcessing;



	class IdxitSort : public IdxitSubCommand {
	public:
		IdxitSort(IdxitArgvParser& argvParser) : IdxitSubCommand("sort", argvParser) {};
	protected:
		bool doCommand() override;
	};
