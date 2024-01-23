#include "IdxitSort.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IdxitAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;



	bool IdxitSort::doCommand() {
		IdxitAppOptions& appOptions = IdxitAppOptions::get();
		appOptions.setCommandMode(IdxitAppOptions::CommandMode::CM_Sort);

		return true;
	}