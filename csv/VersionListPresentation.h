#pragma once
#include "ResultsPresentation.h"

namespace simplearchive
{
	class VersionListResultsPresentation : public ResultsPresentation
	{
	public:
		VersionListResultsPresentation(ResultsList& resultsList) : ResultsPresentation(resultsList)
		{};

		~VersionListResultsPresentation() override = default;

		bool writeHuman() override;
		bool writeXML() override;
		bool writeCSV() override;
		bool writeJson() override;
		bool writeHtml() override;
	};
}
