#pragma once

#include <string>
#include <list>
#include <memory>
#include <sstream>

#include "HomePaths.h"
#include "MetaType.h"
#include "ResultsList.h"
#include <sstream>
#include <iomanip>

namespace simplearchive {

	class ColumnJustification {
		size_t* m_list;
		size_t m_rowSize;
	public:
		ColumnJustification(size_t rowSize)
			: m_rowSize{ rowSize }

		{
			m_list = new size_t[rowSize];
		}
		~ColumnJustification()
		{
			delete[] m_list;
		}

		void header(int idx, const char* name) {
			std::string s = name;
			header(idx, s);
		}

		void header(int idx, std::string& name) {
			size_t len = name.length();
			size_t* c = (m_list + idx);
			if (*c < len) *c = len;
		}

		void readRow(SharedMTRow row) {
			for (size_t i = 0; i < m_rowSize; i++) {
				*(m_list + i) = 0;
			}
			int i = 0;
			for (auto irow = row->begin(); irow != row->end(); irow++) {
				SharedMTColumn column = *irow;
				size_t len = column->toString().length();
				size_t* c = (m_list + i);
				if (*c < len) *c = len;
				i++;
			}
		}

		size_t getSize(int i) {
			size_t* c = (m_list + i);
			return *c;
		}
	};

	class BlockFormat {
		ResultsList& m_resultsList;
		int m_headerIdx{ 0 };
		std::stringstream m_output;
		std::vector<int> m_filterList;
	public:
		BlockFormat(ResultsList& resultsList) : m_resultsList(resultsList) {}
		BlockFormat(int headerIdx, ResultsList& resultsList) : m_resultsList(resultsList), m_headerIdx(headerIdx) {}
		void addFilterIdx(int i) { m_filterList.push_back(i); }
		bool process()
		{
			for (SharedMTRow& row : m_resultsList) {
				readRow(row);
			}
			return true;
		}
		void readRow(SharedMTRow row) {
			MTTableSchema& tableSchem = m_resultsList.getTableSchema();
			//m_output << std::setw(15);
			int i = 0;
			for (auto irow = row->begin(); irow != row->end(); irow++) {
				if (std::find(m_filterList.begin(), m_filterList.end(), i) != m_filterList.end()) {
					i++;
					continue;
				}
				SharedMTColumn column = *irow;
				std::string name = tableSchem[i].getName();
				name += ": ";
				if (m_headerIdx == i) {
					m_output << name << std::setw(15) << column->toString() << std::endl;
				}
				else {
					m_output << "    " << std::left << std::setw(15) << name << column->toString() << std::endl;
				}
				i++;
			}
			m_output << std::endl;
		}

		std::string getOutput() { return m_output.str();  }
	};


	class ResultsList;

	class WriteBase {
	protected:
		ResultsList& m_resultsList;
		std::ostringstream m_output;
	public:
		WriteBase(ResultsList& resultsList) : m_resultsList{ resultsList } {}
		virtual ~WriteBase() = default;

		virtual bool write() = 0;
		std::string getOutput();
	};

	//
	// WriteHuman
	//
	

	class WriteHuman : public WriteBase {
	
	public:
		WriteHuman(ResultsList& resultsList) : WriteBase{ resultsList } {}
		~WriteHuman() = default;
	};

	

	class WriteCSV : public WriteBase {
	
	public:
		WriteCSV(ResultsList& resultsList) : WriteBase{ resultsList } {}
		~WriteCSV() = default;

	};

	//
	// WriteXML
	//
	class WriteXML : public WriteBase {
	protected:
		std::string writeTag(const char* tag, const std::string& value, int tab);
		std::string writeTag(const char* tag, const int value, int tab);
	public:
		WriteXML(ResultsList& resultsList) : WriteBase{ resultsList } {}
		~WriteXML() = default;
	};

	

	//
	// WriteJSON
	//
	class WriteJSON : public WriteBase {
	protected:
		std::string writeTag(const char* tag, const std::string& value, bool end);
		std::string writeArrayOpen(const char* tag);
		std::string writeArrayClose(bool end);
	public:
		WriteJSON(ResultsList& resultsList) : WriteBase{ resultsList } {}
		~WriteJSON() = default;
	};

	
	// WriteHtml
	//
	class WriteHtml : public WriteBase {
	public:
		WriteHtml(ResultsList& resultsList) : WriteBase{ resultsList } {}
		~WriteHtml() = default;		
	};



	//
	// ResultsPresentation
	//
	class ResultsPresentation
	{
	public:
		enum class FormatType {
			XML,
			Json,
			Human,
			csv,
			Html,
			unknown
		};
	protected:
		
		
		ResultsList& m_resultsList;
		std::string m_title;
		std::string m_description;
		std::string m_image;

		std::string m_filename;
		bool m_useFile = { false };

		static bool iequals(const std::string& a, const std::string& b);

	public:
		ResultsPresentation(ResultsList& resultsList) :
			m_resultsList{ resultsList }
		{}
			
		virtual ~ResultsPresentation() = default;

		bool write(FormatType formatType);

		virtual bool writeHuman() { return true; };
		virtual bool writeXML() { return true; };
		virtual bool writeCSV() { return true; };
		virtual bool writeJson() { return true; };
		virtual bool writeHtml() { return true; };

		bool write(std::string& output);

		void setFilename(const char* f) {
			m_useFile = true;
			m_filename = f;
		}

		void setTitle(const char* t) {
			m_title = t;
		}

		void setDescription(const char* d) {
			m_description = d;
		}

		void setImage(const char* i) {
			m_image = i;
		}

		static FormatType parse(const char* str);
	};

};
