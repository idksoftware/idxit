#pragma once
#include "ResultsPresentation.h"


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

