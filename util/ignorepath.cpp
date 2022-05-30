// ignorepath.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

class IqnorePath {
    std::string m_pattern;
    std::string m_drivePattern;
    std::string m_filePattern;
    bool m_isFile{ false };
    bool m_anyDrive{ false };
    bool m_anyDir{ false };
    bool m_anyFile{ false };
    bool m_invalid{ false };
    std::vector<std::string> m_folderList;
    void init();
    void parseEnv();
    void parseDrive(int cpos, int position);
    void parseFile(int cpos, int position);
    void parseDir();
public:
    IqnorePath(const char *pattern) : m_pattern(pattern)
    {
        init();
    }
    ~IqnorePath() = default;
    bool match(const char* path);

    void print()
    {
        std::cout << "************************" << std::endl;
        std::cout << "Pattern: " << m_pattern << std::endl;
        std::cout << "Any Drive: " << ((m_anyDrive)?"True":"False") << std::endl;
        std::cout << "Drive: " << m_drivePattern << std::endl;
        std::cout << "Any Dir: " << ((m_anyDir) ? "True" : "False") << std::endl;
        std::cout << "Folder list: "<< std::endl;
        int i = 1;
        for (auto it : m_folderList) {
            std::cout << "    " << i++ << ':' << it << std::endl;
        }
        std::cout << "------: " << std::endl;
        std::cout << "Any File: " << ((m_anyFile) ? "True" : "False") << std::endl;
        std::cout << "File: " << m_filePattern << std::endl;
        std::cout << "Invalid: " << ((m_invalid) ? "True" : "False") << std::endl;
        std::cout << "************************" << std::endl;
    }
};

void IqnorePath::init()
{
    parseEnv();
    size_t position = 0, cpos = 0;

    bool firstSep = true;
    bool driveSep = true;
    while (cpos != -1)
    {
        cpos = m_pattern.find_first_of('\\', position);
        if (firstSep) {
            firstSep = false;
            parseDrive(cpos, position);

        }
        if (cpos == -1) {
            parseFile(cpos, position);
        }
        else {
            if (driveSep) {
                driveSep = false;
            }
            else {
                std::string str = m_pattern.substr(position, cpos - position);
                m_folderList.push_back(str);
            }
            position = cpos + 1;
        }
    }
    parseDir();
    
   

}

void IqnorePath::parseEnv()
{
    std::string filePath = m_pattern;
    std::string newPath;
    std::vector<std::string> directories;
    size_t startpos = 0, endpos = 0;
    bool envfound = false;
    std::string startString;
    std::string endString;
    while (startpos != -1)
    {
        startpos = filePath.find_first_of("$(", endpos);
        if (startpos != -1) {
            endpos = filePath.find_first_of(")");
            std::string item = filePath.substr(startpos + 2, endpos - (startpos + 2));
            envfound = true;
            item = std::getenv(item.c_str());
            startString = filePath.substr(0, startpos);
            endString = filePath.substr(endpos + 1, filePath.length() - endpos + 1);
            filePath = startString + item + endString;
            startpos = startString.length() + item.length();
            endpos = startpos;
        }
       
    }
    if (envfound) {
        m_pattern = filePath;
    }
   
}


void IqnorePath::parseDrive(int cpos, int position)
{
    if (cpos == 0) {
        m_anyDrive = true;
    }
    else {
        m_drivePattern = m_pattern.substr(position, cpos - position);
        if (m_drivePattern[0] == '*' && m_drivePattern.length() == 1) {
            m_anyDrive = true;
        }
        else if (m_drivePattern[0] == '*' && m_drivePattern[1] == ':' && m_drivePattern.length() == 2) {
            m_anyDrive = true;
        }
        else if (m_drivePattern[0] == '*' && m_drivePattern[1] != ':' && m_drivePattern.length() == 2) {
            m_invalid = true;
        }
        else if (m_drivePattern[0] != '*' && m_drivePattern[1] == ':' && m_drivePattern.length() == 2) {
            m_anyDrive = false;
        }
        else if (m_drivePattern[0] != '*' && m_drivePattern[1] != ':' && m_drivePattern.length() == 2) {
            m_invalid = true;
        }
        else if (m_drivePattern.length() > 2) {
            m_invalid = true;
        }
        //std::string m_drivePattern = m_pattern.substr(position, cpos - position);
    }
}

void IqnorePath::parseDir()
{

}

void IqnorePath::parseFile(int cpos, int position)
{
    m_filePattern = m_pattern.substr(position, cpos - position);
    if (m_filePattern.length() == 0) {
        m_anyFile = true;
    }
    else if (m_filePattern.length() == 1 && m_drivePattern[0] == '*') {
        m_anyFile = true;
    }
}

bool IqnorePath::match(const char* path)
{
    std::string filePath = path;
    std::vector<std::string> directories;
    size_t position = 0, currentPosition = 0;

    while (currentPosition != -1)
    {
        currentPosition = filePath.find_first_of('\\', position);
        directories.push_back(filePath.substr(position, currentPosition - position));
        position = currentPosition + 1;
    }
    for (auto it : directories)
        std::cout << it << std::endl;
    return true;
}

int main()
{
    IqnorePath iqnorePath1("$(HOMEDRIVE)$(HOMEPATH)\\Pictures\\*");
    iqnorePath1.print();
    std::cout << "C:\\Users\\Iain Ferguson\\ImgArchive\\Pictures\\2017: " <<
                ((iqnorePath1.match("C:\\Users\\Iain Ferguson\\ImgArchive\\Pictures\\2017")) ? "True" : "False") << std::endl;
   

    IqnorePath iqnorePath2("$(ProgramFiles)\\.git\\*");
    IqnorePath iqnorePath3("*:\\Windows\\System32\\*");
    //IqnorePath iqnorePath4("c:\\*\\ttt\\.git\\*\\g");
    //IqnorePath iqnorePath5("c?\\*\\ttt\\.git\\*\\ggg.gg");

    std::string filePath = "C:\\ProgramData\\Users\\CodeUncode\\Documents";
    std::vector<std::string> directories;
    size_t position = 0, currentPosition = 0;

    while (currentPosition != -1)
    {
        currentPosition = filePath.find_first_of('\\', position);
        directories.push_back(filePath.substr(position, currentPosition - position));
        position = currentPosition + 1;
    }
    for (auto it : directories)
        std::cout << it << std::endl;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
