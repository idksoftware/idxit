#pragma once
#include <string>

//E:\workspace\2017\2017-07-15

class WorkspaceFiles
{
	static std::string m_workspaceRoot;
	std::string m_address;
public:
	WorkspaceFiles(const char* szAddress);
	~WorkspaceFiles() = default;
	bool process();

	static void setWorkspaceRoot(char* path);
};

