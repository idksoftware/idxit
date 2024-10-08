#include <sstream>
#include <iostream>
#include "Quiet.h"
//#include <filesystem>
#include "IdxitArgvParser.h"
#include "ConfigReader.h"
#include "IdxitAppOptions.h"
#include "AppConfig.h"
#include "pathcontroller.h"
#include "SAUtils.h"
#include "stdio.h"
#include "argvparser.h"
#include "Environment.h"
#include "EnvFunc.h"
#include "IdxitProperties.h"
//#include "ParseProperties.h"
#include "IdxitAbout.h"
#include "IdxitScan.h"
#include "IdxitSort.h"

// beyond compare
using namespace CommandLineProcessing;


void  IdxitArgvParser::defineOptions() {

	addErrorCode(0, "Success");
	addErrorCode(1, "Warnings");
	addErrorCode(2, "Errors");
	addErrorCode(3, "Fatal");

	setIntroductoryDescription("idxit - Is a command line tool for indexing storage devices");

	setHelpOption();

	// Subcommands
	defineOption("prop", "Show properties", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("prop", "idxit prop [--s]\n\t[--logging-level=<level>]"
		"[--scope=<scope-address]\n\t[--set=<property:value>]");


	defineOption("scan", "scan for new images in the file sytem. Output to a jounal file.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("scan", "idxit scan [--source-path=<path>]\n\t"
		"[--format-type=<output format>]\n\t[--index-file=<index file path>");

	defineOption("define", "An index definition.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("define", "idxit define [--source-path=<path>]\n\t"
		"[--comment=<comment text>]\n\t[--lightroom=<On|Off>]");

	
	defineOption("sort", "Sort an Index by file properties.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("sort", "idxit sort [--index-file=<path>]\n\t"
		"[--name]\n\t[--=<scope-address]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	
	defineOption("about", "prints this version information", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("about", "about [--out]\n\t[--file]\n");

	/*
	defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("b", "backup");
	*/
	//defineOption("set", "Sets a metadata property:value", ArgvParser::OptionAttributes::OptionRequiresValue);
	//defineOptionAlternative("set", "set-prop");
	//defineCommandSyntax("set", "iaarc workspace [--sync]\n\t[--logging-level=<level>]");
	//	"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]");

	defineOption("index-file", "Path to index file.", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineCommandSyntax("index-file", "index-file=<path/filename>\n");



	defineOption("n", "name of the view.", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("n", "name");

	defineOption("q", "quiet output is sent to the terminal.", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("q", "quiet");

	defineOption("s", "no output is sent to the terminal.", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("s", "silent");

	defineOption("e", "Generate progress events.", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("e", "events");

	defineOption("p", "source of the images", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("p", "source-path");

	defineOption("I", "group of media file types to include", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("I", "inc-group");

	defineOption("H", "Scan hidden files and folders", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("H", "exc-group");

	defineOption("E", "group of media file types to exclude", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("E", "scan-hidden");
	
	defineOption("Q", "quick scan", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("Q", "quick");

	//defineOption("R", "Remode server mode", ArgvParser::OptionAttributes::NoOptionAttribute);
	//defineOptionAlternative("R", "remote-server");

	//defineOption("d", "destination of the images", ArgvParser::OptionAttributes::OptionRequiresValue);
	//defineOptionAlternative("d", "dist-path");

	//defineOption("a", "archive", ArgvParser::OptionAttributes::NoOptionAttribute);
	//defineOptionAlternative("a", "archive");


	//defineOption("image", "Specifies a image address in the form \"<date>/<image name>", ArgvParser::OptionAttributes::OptionRequiresValue);
	//defineOptionAlternative("i", "image");

	defineOption("F", "no output is sent to the terminal.", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("F", "force");

	defineOption("C", "current template (master or derivative)", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("C", "current");



	defineOption("v", "specifies a version.", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("v", "version");


	defineOption("td", "to date", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("td", "to-date");

	defineOption("fd", "From date", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("fd", "from-date");



	defineOption("r", "location of the archive root folder.", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("r", "root");

	

	defineOption("l", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("l", "logging-level");

	defineOption("c", "Comment to be included in command", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("c", "comment");

	defineOption("U", "Show settup", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("U", "settup");

	defineOption("format-type", "text output format type. Can be \"Humam\", \"XML\" \"Json\" or \"cvs\" i.e format-type=XML.", ArgvParser::OptionAttributes::OptionRequiresValue);
	//defineOptionAlternative("ft", "format-type");
	defineCommandSyntax("format-type", "format-type=[Human] | [xml] | [json] | [html] | [csv]\n");


	defineOption("o", "property option.", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("o", "option");
	defineCommandSyntax("option", "option <argument=value>\n");

	defineOption("file", "output file name.", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineCommandSyntax("file", "file=<filename>\n");

	

	defineOption("force-date", "Overrides all dates found associated with the images in the selection", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOption("default-date", "Uses this date if none found associated with an image", ArgvParser::OptionAttributes::OptionRequiresValue);

	defineOption("list", "list checked out/in and delete images", ArgvParser::OptionAttributes::OptionRequiresValue); // =all =year{2015}
	defineCommandSyntax("list", "list=[checked-out] | [checked-in] | [deleted]\n");

	defineCommandOption("about", "format-type");
	defineCommandOption("about", "file");

	defineCommandOption("prop", "source-path");
	defineCommandOption("prop", "format-type");

	
	defineCommandOption("scan", "source-path");
	defineCommandOption("scan", "format-type");
	defineCommandOption("scan", "index-file");
	defineCommandOption("scan", "inc-group");
	defineCommandOption("scan", "exc-group");
	defineCommandOption("scan", "scan-hidden");
	defineCommandOption("scan", "quick");
	
	defineCommandOption("define", "comment");
	defineCommandOption("define", "source-path");
	
	defineCommandOption("sort", "index-file");
	
}



bool IdxitArgvParser::doInitalise(int argc, char **argv) {

	IdxitAppOptions& appOptions = IdxitAppOptions::get();

	
	IDXITAppConfig config;
	
	
//	defineCommandOption("mode", "remote-server");

	defineOptions();

	if (handleParseResult(argc, argv) == false) {
		return false;
	}
	
	std::shared_ptr <IdxitAbout>		aboutCmd		= std::make_shared<IdxitAbout>(*this);
	std::shared_ptr <IdxitProperties>	propCmd			= std::make_shared<IdxitProperties>(*this);
	std::shared_ptr <IdxitScan>			scanCmd			= std::make_shared<IdxitScan>(*this);
	std::shared_ptr <IdxitSort>			sortCmd			= std::make_shared<IdxitSort>(*this);


	addSubCommand(aboutCmd);
	addSubCommand(propCmd);
	addSubCommand(scanCmd);
	addSubCommand(sortCmd);
	
	return doCommand();
}



std::string IdxitArgvParser::usageDescriptionHeader(unsigned int _width) const
{
	std::string usage;
	
	//usage += "usage: imgarc subcommand [options] [args]\n\n";
	//usage += "IdxIt command line client, version 1.0.0.1\n";
	//usage += "Type 'iaarc help <subcommand>' for help on a specific subcommand.\n\n";
	
	// setIntroductoryDescription("iaarc - Image archive client provides the means to operate IdxIt from the command line");
	std::string tmp = "iaarc - Image archive client provides the means to operate IdxIt from the command line";
	usage += '\n';
	usage += formatString(tmp, _width);
	usage += '\n';
	usage += '\n';
	usage += "Note:\n";
	usage += formatString("The administration of the archive is carried out by the \"iaadmin\" command-line interface.", _width) + "\n";

	return usage;
}

std::string IdxitArgvParser::commandUsage(unsigned int width) const
{
	std::string usage; // the usage description text
	usage = formatString("usage: iaarc[--version][--help] <command>[<args>]\n", width);
	usage += '\n';

	return usage;
}



std::string IdxitArgvParser::generalHelp(unsigned int _width) const
{
	std::string usage; // the usage description text
	usage = commandUsage(_width);
	/*
	if (intro_description.length())
		usage += formatString(intro_description, _width) + "\n";

	if (max_key <= 1) {// if we have some options

		usage += formatString("No options available\n", _width) + "\n\n";
		return(usage);
	}

	*/
	usage += '\n';
	/*
	usage += "usage: sia subcommand [options] [args]\n\n";
	usage += "Image archive command line client, version 1.0.0.1\n";
	usage += "Type 'sia help <subcommand>' for help on a specific subcommand.\n\n";
	*/
	std::string tmp = "The command imgarc is the primary command-line interface to IdxIt. This interface is used to manage the control of images going in and out of the archive software. ";
	tmp += "It has a rich set of subcommands that \"add/import\" images to the archive and \"export\" images out of the archive, In addition manages the controlled modification of images";
	tmp += " using the \"check-in/check-out\" command set";
	usage += '\n';
	usage += formatString(tmp, _width);
	usage += '\n';

	usage += "Note:\n";
	usage += formatString("The administration of the archive is carried out by the imgadmin command-line interface.", _width) + "\n";

	usage += formatString(command_header, _width) + "\n";
	usage += '\n';
	usage += AVAILABLE_COMMANDS;
	usage += "-\n";
	usage += "\n";
	for (auto it = option2attribute.begin(); it != option2attribute.end(); ++it)
	{
		std::string _os; // temp string for the option
		if (option2attribute.find(it->first)->second != OptionAttributes::MasterOption) {
			continue;
		}
		std::string _longOpt;
		std::string _shortOpt;
		std::list<std::string> alternatives = getAllOptionAlternatives(it->first);
		for (auto alt = alternatives.begin();
			alt != alternatives.end();
			++alt)
		{
			if (option2attribute.find(it->first)->second == OptionAttributes::MasterOption) {
				OptionAttributes option = option2attribute.find(it->first)->second;
				_os.clear();
				if (alt->length() > 1) {
					_longOpt += *alt;
				}
				else {
					_shortOpt += *alt;
				}


			}
		}

		if (!_longOpt.empty()) {
			_os += ' ';
			_os += _longOpt;
		}
		if (!_shortOpt.empty()) {
			_os += " (";
			_os += _shortOpt;
			_os += ')';
		}
		//_os += " : ";
		usage += formatLine(_os, _width, 0, 20);
		_os.clear();
		_longOpt.clear();
		_shortOpt.clear();
		if (option2descr.find(it->first) != option2descr.end())
			usage += formatString(option2descr.find(it->first)->second, _width, 4) + "\n";
		else
			usage += formatString("(no description)", _width, 4) + "\n";

	}
	usage += "\n";
	/*
	//printf("%s\n", usage.c_str());
	// loop over all option attribute entries (which equals looping over all
	// different options (not option names)
	for (Key2AttributeMap::const_iterator it = option2attribute.begin();
	it != option2attribute.end();
	++it)
	{
	string os; // temp string for the option

	// get the list of alternative names for this option
	list<string> alternatives = getAllOptionAlternatives(it->first);

	unsigned int count = 0;
	for( list<string>::const_iterator alt = alternatives.begin();
	alt != alternatives.end();
	++alt )
	{
	++count;
	if (option2attribute.find(it->first)->second == MasterOption) {
	continue;
	}
	// additional '-' for long options
	if (alt->length() > 1)
	os += "-";

	os += "-" + *alt;

	// note if the option requires a value
	if (option2attribute.find(it->first)->second & OptionRequiresValue)
	os += " <value>";

	// alternatives to come?
	if (count < alternatives.size())
	os += ", "; // add separator
	}

	// note if the option is required
	if (option2attribute.find(it->first)->second & OptionRequired)
	os += " [required]";

	usage += formatString(os, _width) + "\n";

	if (option2descr.find(it->first) != option2descr.end())
	usage += formatString(option2descr.find(it->first)->second, _width, 4);
	else
	usage += formatString("(no description)", _width, 4);

	// finally a little gap
	usage += "\n\n";
	}
	*/
	if (!errorcode2descr.size()) // if have no errorcodes
		return(usage);

	usage += formatString("\n\nReturn codes:\n", _width) + "\n";

	//   map<int, string>::const_iterator eit;
	for (auto alt = errorcode2descr.begin();
		alt != errorcode2descr.end();
		++alt)
	{
		std::ostringstream codeStr;
		codeStr << alt->first;
		std::string label = formatString(codeStr.str(), _width, 4);
		std::string descr = formatString(alt->second, _width, 10);
		usage += label + descr.substr(label.length()) + "\n";
	}
	usage += '\n';
	usage += "Image Archive is a tool for image archiving and version control system.\n";
	usage += "For additional information, see \"http:/www.idk-software.com/\"";
	usage += '\n';
	return(usage);
}


