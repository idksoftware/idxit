
#include "VersionListPresentation.h"

#include <iostream>
#include "CSVArgs.h"
#include <sstream>
#include "MetaType.h"
#include <iomanip>
#include "DBDefines.h"
#include "HistoryEvent.h"

#include "ResultsList.h"
#include "ResultsPresentation.h"


#include "ResultsPresentation.h"



	//
		// WriteHuman
		//
	class  VersionListWriteHuman : public WriteHuman {

	public:
		VersionListWriteHuman(ResultsList& resultsList);
		~VersionListWriteHuman() override = default;

		bool write() override;
	};

	VersionListWriteHuman::VersionListWriteHuman(ResultsList& resultsList) : WriteHuman(resultsList) {}

	bool VersionListWriteHuman::write()
	{
		BlockFormat blockFormat(1,m_resultsList);
		blockFormat.addFilterIdx(0);
		blockFormat.addFilterIdx(2);
		blockFormat.addFilterIdx(5);
		blockFormat.process();
		m_output << blockFormat.getOutput();
		/*
		ColumnJustification columnJustification(m_resultsList.getTableSchema().size());

		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			columnJustification.readRow(row);
		}
		int eventIdx = -1;
		int idx = 0;
		for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {

			MTSchema& columnInfo = *i;
			std::string s = columnInfo.getName();
			columnJustification.header(idx, s);
			if (idx != 0 && idx != 2) {
				m_output << std::setw(columnJustification.getSize(idx) + 1) << columnInfo.getName();
			}
			idx++;
		}
		m_output << "\n";
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			idx = 0;
			for (auto i = row->begin(); i != row->end(); i++) {
				SharedMTColumn column = *i;
				if (idx != 0 && idx != 2) {
					m_output << std::setw(columnJustification.getSize(idx) + 1) << column->toString();
				}
				idx++;
			}
			m_output << '\n';
		}
		*/
		return true;
	}

	bool VersionListResultsPresentation::writeHuman()
	{
		VersionListWriteHuman writeHuman(m_resultsList);
		if (!writeHuman.write()) {
			return false;
		};
		std::string temp = writeHuman.getOutput();
		if (write(temp) == false) {
			return false;
		}
		return true;
		
	}

	bool VersionListResultsPresentation::writeXML()
	{
		return false;
	}

	bool VersionListResultsPresentation::writeCSV()
	{
		return false;
	}

	bool VersionListResultsPresentation::writeJson()
	{
		return false;
	}

	bool VersionListResultsPresentation::writeHtml()
	{
		return false;
	}
