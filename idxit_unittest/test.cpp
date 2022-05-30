#include "gtest/gtest.h"

#include "ExtentionsFilterFile.h"
#include "IndexSpecifications.h"
#include "IgnoreList.h"
#include "IgnorePath.h"


TEST(TestCaseName, TestName) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(TestCaseName, TestIgnoreFile) {

	IqnorePath iqnorePath1("$(HOMEDRIVE)$(HOMEPATH)\\Pictures\\junk.txt");
	iqnorePath1.print();
	std::cout << "C:\\Users\\Iain Ferguson\\ImgArchive\\Pictures\\2017: " <<
		((iqnorePath1.match("C:\\Users\\Iain Ferguson\\ImgArchive\\Pictures\\junk.txt")) ? "True" : "False") << std::endl;


	IqnorePath iqnorePath2("$(ProgramFiles)\\.git\\*");
	IqnorePath iqnorePath3("*:\\Windows\\System32\\*");
	//IqnorePath iqnorePath4("c:\\*\\ttt\\.git\\*\\g");
	//IqnorePath iqnorePath5("c?\\*\\ttt\\.git\\*\\ggg.gg");

	
	


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