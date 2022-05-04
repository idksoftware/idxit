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
#include "IdxitParseOptions.h"
//#include "ParseProperties.h"
#include "IdxitAbout.h"

// beyond compare
using namespace CommandLineProcessing;
namespace simplearchive {

void  IdxitArgvParser::defineOptions() {

	addErrorCode(0, "Success");
	addErrorCode(1, "Warnings");
	addErrorCode(2, "Errors");
	addErrorCode(3, "Fatal");

	setIntroductoryDescription("iaarc - Image archive client provides the means to operate IdxIt from the command line");

	setHelpOption();

	// Subcommands
	defineOption("scan", "scan for new images in the file sytem. Output to a jounal file.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("scan", "iaarc scan [--source-path=<path>]\n\t"
		"[--format-type=<output format>]\n\t[--file=<jounal file path>");

	defineOption("import", "import new images to the archive.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("import", "iaarc import [--source-path=<path>]\n\t"
		"[--comment=<comment text>]\n\t[--lightroom=<On|Off>]");

	defineOption("checkout", "Checkout images from archive to workspace.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("checkout", "iaarc checkout [--target-path=<path>]\n\t"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	defineOption("checkin", "Checkin images from workspace into the archive.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("checkin", "iaarc checkin [--target-path=<path>]\n\t[--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	defineOption("uncheckout", "Un-checkout images from workspace into the archive.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("uncheckout", "iaarc uncheckout [--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address>]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	defineOption("get", "Get images from archive to workspace.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("get", "iaarc get [--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address>]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	defineOption("delete", "Delete images in the archive.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("delete", "iaarc delete [--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address>]\n\t[--version=<vesion-num>");

	defineOption("undelete", "Undelete images in the archive.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("undelete", "iaarc undelete [--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address>]\n\t[--version=<vesion-num>");

	defineOption("export", "Export images from archive into a destination folder.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("export", "iaarc export [--target-path=<path>]\n\t[--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	defineOption("prop", "Manage image properties", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("prop", "iaarc prop [--s]\n\t[--logging-level=<level>]"
		"[--scope=<scope-address]\n\t[--set=<property:value>]");

	defineOption("metadata", "Show Metadata properties", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("metadata", "iaarc metadata [--s]\n\t[--format-type=<type>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--master=<yes|No>]");

	defineOption("template", "Manage metadata template", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("template", "iaarc template [--current=<yes|no>]\n\t[--logging-level=<level>]"
		"[--current=<yes|no>]\n\t[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]");

	defineOption("status", "show check in/out status", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("status", "iaarc status [--list>]\n\t[--logging-level=<level>]"
		"[--current=<yes|no>]\n\t[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]");

	defineOption("history", "shows the check in/out status history for the archive.", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("history", "iaarc status [--list>]\n\t[--logging-level=<level>]"
		"[--current=<yes|no>]\n\t[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]");

	defineOption("show", "Show details", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("show", "iaarc show [--history=<image-address>]\n\t");

	defineOption("log", "Show logs", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("log", "iaarc log [--image=<image-address]");

	defineOption("journal", "Show journals", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("journal", "iaarc journal [--image=<image-address]");

	defineOption("mode", "Sets the mode in which imgarchive will be operating", ArgvParser::OptionAttributes::MasterOption);

	defineOption("about", "prints this version information", ArgvParser::OptionAttributes::MasterOption);
	defineCommandSyntax("about", "about [--out]\n\t[--file]\n");

	/*
	defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("b", "backup");
	*/
	defineOption("set", "Sets a metadata property:value", ArgvParser::OptionAttributes::OptionRequiresValue);
	//defineOptionAlternative("set", "set-prop");
	defineCommandSyntax("set", "iaarc workspace [--sync]\n\t[--logging-level=<level>]");
	//	"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]");


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

	defineOption("L", "import from lightroom", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("L", "lightroom");

	defineOption("A", "address scope", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("A", "scope");

	defineOption("R", "Remode server mode", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("R", "remote-server");

	defineOption("d", "destination of the images", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("d", "dist-path");

	defineOption("a", "archive", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("a", "archive");


	defineOption("image", "Specifies a image address in the form \"<date>/<image name>", ArgvParser::OptionAttributes::OptionRequiresValue);
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

	defineOption("format-type", "text output format type. Can be \"Humam\", \"XML\" \"Json\" or \"cvs\" i.e format-type=XML.", ArgvParser::OptionAttributes::OptionRequiresValue);
	//defineOptionAlternative("ft", "format-type");
	defineCommandSyntax("format-type", "format-type=[Human] | [xml] | [json] | [html] | [csv]\n");

	defineOption("l", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("l", "logging-level");

	defineOption("c", "Comment to be included in command", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("c", "comment");

	defineOption("U", "Show settup", ArgvParser::OptionAttributes::NoOptionAttribute);
	defineOptionAlternative("U", "settup");

	defineOption("out", "Output type: text, xml, json or html.", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineCommandSyntax("out", "out=[plain] | [xml] | [json] | [html]\n");

	defineOption("o", "property option.", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOptionAlternative("o", "option");
	defineCommandSyntax("option", "option <argument=value>\n");

	defineOption("file", "output file name.", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineCommandSyntax("file", "file=<filename>\n");

	defineOption("force-date", "Overrides all dates found associated with the images in the selection", ArgvParser::OptionAttributes::OptionRequiresValue);
	defineOption("default-date", "Uses this date if none found associated with an image", ArgvParser::OptionAttributes::OptionRequiresValue);

	defineOption("list", "list checked out/in and delete images", ArgvParser::OptionAttributes::OptionRequiresValue); // =all =year{2015}
	defineCommandSyntax("list", "list=[checked-out] | [checked-in] | [deleted]\n");

	defineCommandOption("about", "comment");

	defineCommandOption("about", "source-path");
	defineCommandOption("about", "lightroom");

	
	defineCommandOption("scan", "source-path");
	defineCommandOption("scan", "format-type");
	defineCommandOption("scan", "file");
	
	defineCommandOption("import", "comment");
	defineCommandOption("import", "source-path");
	defineCommandOption("import", "lightroom");

	defineCommandOption("checkin", "comment");
	defineCommandOption("checkin", "scope");
	defineCommandOption("checkin", "force");
	defineCommandOption("checkin", "image");
	defineCommandOption("checkout", "version");

	defineCommandOption("checkout", "comment");
	defineCommandOption("checkout", "scope");
	defineCommandOption("checkout", "force");
	defineCommandOption("checkout", "image");
	defineCommandOption("checkout", "version");

	defineCommandOption("uncheckout", "comment");
	defineCommandOption("uncheckout", "scope");
	defineCommandOption("uncheckout", "force");
	defineCommandOption("uncheckout", "image");
	defineCommandOption("checkout", "version");

	defineCommandOption("get", "comment");
	defineCommandOption("get", "scope");
	defineCommandOption("get", "force");
	defineCommandOption("get", "image");
	defineCommandOption("checkout", "version");

	defineCommandOption("delete", "comment");
	defineCommandOption("delete", "scope");
	defineCommandOption("delete", "force");
	defineCommandOption("delete", "image");
	defineCommandOption("checkout", "version");

	defineCommandOption("undelete", "comment");
	defineCommandOption("undelete", "scope");
	defineCommandOption("undelete", "force");
	defineCommandOption("undelete", "image");
	defineCommandOption("checkout", "version");

	defineCommandOption("export", "logging-level");
	defineCommandOption("export", "dist-path");
	defineCommandOption("export", "comment");
	defineCommandOption("export", "scope");
	defineCommandOption("export", "force");
	defineCommandOption("export", "version");

	defineCommandOption("status", "list");
	defineCommandOption("status", "scope");
	defineCommandOption("status", "format-type");
	defineCommandOption("status", "file");

	defineCommandOption("history", "list");
	defineCommandOption("history", "scope");
	defineCommandOption("history", "format-type");
	defineCommandOption("history", "file");

	
	defineCommandOption("metadata", "scope");
	defineCommandOption("metadata", "format-type");
	defineCommandOption("metadata", "file");

	defineCommandOption("show", "settup");

	defineCommandOption("journal", "image");
	defineCommandOption("journal", "import");
	defineCommandOption("journal", "format-type");
	defineCommandOption("journal", "file");

	defineCommandOption("prop", "scope");
	defineCommandOption("prop", "set");

	defineCommandOption("template", "current");
	defineCommandOption("template", "archive");
	defineCommandOption("template", "option");

	defineCommandOption("log", "image");
	defineCommandOption("log", "scope");
	defineCommandOption("log", "format-type");
	defineCommandOption("log", "file");

	//defineCommandOption("mode", "");

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
	

	addSubCommand(aboutCmd);
	
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





} /* name*/
