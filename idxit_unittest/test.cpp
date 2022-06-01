#include "gtest/gtest.h"

#include "ExtentionsFilterFile.h"
#include "IndexSpecifications.h"
#include "IgnoreList.h"
#include "IgnorePath.h"


TEST(TestCaseName, TestName) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

/*
All name files, name folders, and files and folders in any name folder
*/
TEST(TestCaseName, TestIgnorePath18)
{
    std::string pattern = "c:\\Windows\\";

    std::string match1 = "c:\\Windows\\system32";
    std::string no_match2 = "*:\\Windows\\system32";
   

    IqnorePath iqnorePath1(pattern.c_str());
    iqnorePath1.print();
    std::cout << "Pattern: \"" << pattern << "\" Match: \"" << match1 <<
        ((iqnorePath1.match(match1.c_str())) ? "\" True" : "\" False") << std::endl;



}


/*
All name files, name folders, and files and folders in any name folder
*/
TEST(TestCaseName, TestIgnorePath1) 
{
    std::string pattern = "name";

    std::string match1 = "/name.log";
    std::string match2 = "/name/file.txt";
    std::string match3 = "/lib/name.log";

    IqnorePath iqnorePath1(pattern.c_str());
    iqnorePath1.print();
    std::cout << "Pattern: \"" << pattern << "\" Match: \"" << match1 << 
        ((iqnorePath1.match(match1.c_str())) ? "\" True" : "\" False") << std::endl;


    
}

/*
Ending with / specifies the pattern is for a folder. Matches all files and folders in any name folder
*/
TEST(TestCaseName, TestIgnorePath2)
{
    std::string pattern = "name/";

    std::string match1 = "/name/file.txt";
    std::string match2 = "/name/log/name.log";
    std::string no_match2 = "/name.log";
}

/*
All files with the name.file
*/
TEST(TestCaseName, TestIgnorePath3)
{
    std::string pattern = "name.file";

    std::string match1 = "/name.file";
    std::string match2 = "/lib/name.file";
}

/*
Starting with / specifies the pattern matches only files in the root folder
*/
TEST(TestCaseName, TestIgnorePath4)
{
    std::string pattern = "/name.file";

    std::string match1 = "/name.file";
    std::string no_match2 = "/lib/name.file";

}

/*
Patterns specifiing files in specific folders are always realative to root (even if you do not start with / )
*/
TEST(TestCaseName, TestIgnorePath5)
{
    std::string pattern = "lib/name.file";

    std::string match1 = "/name.file";
    std::string no_match1 = "name.file";
    std::string no_match2 = "/test/lib/name.file";

}

/*
Starting with ** before / specifies that it matches any folder in the repository. Not just on root.
*/
TEST(TestCaseName, TestIgnorePath6)
{
    std::string pattern = "**/lib/name.file";

    std::string match1 = "/lib/name.file";
    std::string match2 = "/test/lib/name.file";

}

/*
All name folders, and files and folders in any name folder
*/
TEST(TestCaseName, TestIgnorePath7)
{
    std::string pattern = "**/name";

    std::string match1 = "/name/log.file";
    std::string match2 = "/lib/name/log.file";
    std::string match3 = "/name/lib/log.file";
}

/*
All name folders, and files and folders in any name folder within the lib folder.
*/
TEST(TestCaseName, TestIgnorePath8)
{
    std::string pattern = "/lib/**/name";

    std::string match1 = "/lib/name/log.file";
    std::string match2 = "/lib/test/name/log.file";
    std::string match3 = "/lib/test/ver1/name/log.file";

    std::string no_match1 = "/name/log.file";

}

/*
All files withe .file extention
*/
TEST(TestCaseName, TestIgnorePath9)
{
    std::string pattern = "*.file";

    std::string match1 = "/name.file";
    std::string match2 = "/lib/name.file";
}

/*
All folders ending with name
*/
TEST(TestCaseName, TestIgnorePath10)
{
    std::string pattern = "*name/";

    std::string match1 = "/lastname/log.file";
    std::string match2 = "/firstname/log.file";
}

/*
? matches a single non-specific character
*/
TEST(TestCaseName, TestIgnorePath11)
{
    std::string pattern = "name?.file";

    std::string match1 = "/names.file";
    std::string match2 = "/name1.file";

    std::string no_match1 = "/names1.file";

}

/*
[range] matches a single character in the specified range (in this case a character in the range of a-z, and also be numberic.)
*/
TEST(TestCaseName, TestIgnorePath12)
{
    std::string pattern = "name[a-z].file";

    std::string match1 = "/names.file";
    std::string match2 = "/nameb.file";

    std::string no_match1 = "/name1.file";

}

/*
[set] matches a single character in the specified set of characters (in this case either a, b, or c)
*/
TEST(TestCaseName, TestIgnorePath13)
{
    std::string pattern = "name[abc].file";

    std::string match1 = "/namea.file";
    std::string match2 = "/nameb.file";

    std::string no_match1 = "/names.file";
}

/*
[!set] matches a single character, except the ones spesified in the set of characters (in this case a, b, or c)
*/
TEST(TestCaseName, TestIgnorePath14)
{
    std::string pattern = "name[!abc].file";

    std::string match1 = "/names.file";
    std::string match2 = "/namex.file";

    std::string no_match1 = "/namesb.file";

}

/*
!specifies a negation or exception. Matches all files and folders in any name folder, except name/secret.log
*/
TEST(TestCaseName, TestIgnorePath15)
{
    std::string pattern = "name/";
    std::string pattern1 = "!name/secret.log";

    std::string match1 = "/name/file.txt";
    std::string match2 = "/name/log/name.log";

    std::string no_match1 = "/name/secret.log";

}

/*
!specifies a negation or exception. Matches all files and folders in any name folder, except name/secret.log
*/
TEST(TestCaseName, TestIgnorePath16)
{
    std::string pattern = "name/";
    std::string pattern1 = "!name/secret.log";

    std::string match1 = "/name/file.txt";
    std::string match2 = "/name/log/name.log";

    std::string no_match1 = "/name/secret.log";

}

TEST(TestCaseName, TestIgnorePath17) {

    IqnorePath iqnorePath1("$(HOMEDRIVE)$(HOMEPATH)\\ImgArchive\\Pictures\\junk.txt");
    iqnorePath1.print();
    std::cout << "C:\\Users\\Iain Ferguson\\ImgArchive\\Pictures\\2017: " <<
        ((iqnorePath1.match("C:\\Users\\Iain Ferguson\\ImgArchive\\Pictures\\junk.txt")) ? "True" : "False") << std::endl;


    IqnorePath iqnorePath2("$(ProgramFiles)\\.git\\*");
    IqnorePath iqnorePath3("*:\\Windows\\System32\\*");
    //IqnorePath iqnorePath4("c:\\*\\ttt\\.git\\*\\g");
    //IqnorePath iqnorePath5("c?\\*\\ttt\\.git\\*\\ggg.gg");





}



TEST(TestCaseName, TestIgnoreList) {
	IgnoreList ignoreList;
	ignoreList.read("C:\\ProgramData\\IDK-Software\\idxit\\filters\\sys.ign");
}



TEST(TestCaseName, TestExtentionsFilterObject) {

	ExtentionsFilterObject extentionsFilterObject;

	EXPECT_TRUE(extentionsFilterObject.setExtentionsFilePath("C:\\ProgramData\\IDK-Software\\idxit\\filters\\userincext.fff"));

	EXPECT_TRUE(extentionsFilterObject.init());
	std::vector<std::shared_ptr<ExtentionItem>> list;
	EXPECT_TRUE(extentionsFilterObject.getList(list));
	for (auto ii : list)
	{
		printf("Ext: %s\n", ii->getExt().c_str());
	}

	EXPECT_TRUE(extentionsFilterObject.isAllowed("nef"));
}

TEST(TestCaseName, TestExtentionsFilter) {

	IndexExtentionsFilter indexExtentionsFilter;
	EXPECT_TRUE(indexExtentionsFilter.read());
	/*
	EXPECT_TRUE(extentionsFilterObject.setExtentionsFilePath("C:\\ProgramData\\IDK-Software\\idxit\\config\\userincext.dat"));

	EXPECT_TRUE(extentionsFilterObject.init());
	std::vector<std::shared_ptr<ExtentionItem>> list;
	EXPECT_TRUE(extentionsFilterObject.getList(list));
	for (auto ii : list)
	{
		printf("Ext: %s\n", ii->getExt().c_str());
	}

	EXPECT_TRUE(extentionsFilterObject.isAllowed("nef"));
	*/
}

TEST(TestCaseName, TestFolderFilter) {

	IndexFolderFilter indexFolderFilter;
	EXPECT_TRUE(indexFolderFilter.read());
	/*
	EXPECT_TRUE(extentionsFilterObject.setExtentionsFilePath("C:\\ProgramData\\IDK-Software\\idxit\\config\\userincext.dat"));

	EXPECT_TRUE(extentionsFilterObject.init());
	std::vector<std::shared_ptr<ExtentionItem>> list;
	EXPECT_TRUE(extentionsFilterObject.getList(list));
	for (auto ii : list)
	{
		printf("Ext: %s\n", ii->getExt().c_str());
	}

	EXPECT_TRUE(extentionsFilterObject.isAllowed("nef"));
	*/
}