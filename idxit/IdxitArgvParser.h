#pragma once
#include "PrjArgvParser.h"

using namespace CommandLineProcessing;



	class IdxitArgvParser;

	class IdxitSubCommand : public SubCommand {
		IdxitArgvParser& m_argvParser;
	protected:
		const IdxitArgvParser& getParser() { return m_argvParser; };
	public:
		IdxitSubCommand(const char* name, IdxitArgvParser& argvParser)
			: SubCommand(name),
			m_argvParser(argvParser) {};
	};


	class IdxitArgvParser : public PrjArgvParser
	{
		bool m_error;
	public:

		IdxitArgvParser() : m_error(false) {};

		virtual ~IdxitArgvParser()
		{
		}

		bool initalise(int argc, char **argv) {
			defineOptions();
			return doInitalise(argc, argv);
		}

	protected:
		virtual bool doInitalise(int argc, char **argv);
		virtual void defineOptions();
		std::string usageDescriptionHeader(unsigned int _width) const;

		/** Returns a string with the usage descriptions for all options. The
		 * description string is formated to fit into a terminal of width _width.*/
		virtual std::string commandUsage(unsigned int _width = 80) const;

		virtual std::string generalHelp(unsigned int _width) const;

	};
