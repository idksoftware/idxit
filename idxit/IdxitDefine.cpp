#include "IdxitDefine.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IdxitAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IdxitDefinition::doCommand() {
		IdxitAppOptions& appOptions = IdxitAppOptions::get();
		appOptions.setCommandMode(IdxitAppOptions::CommandMode::CM_Show);

		return true;
	}
}