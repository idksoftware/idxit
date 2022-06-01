// ignorepath.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "ignorePath.h"


#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

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


void IqnorePath::parseDrive(size_t cpos, size_t position)
{
    if (cpos == 0) {
        m_anyDrive = true;
        m_drivePattern = "*:";
    }
    else {
        m_drivePattern = m_pattern.substr(position, cpos - position);
        if (m_drivePattern[0] == '*' && m_drivePattern.length() == 1) {
            m_drivePattern += ':';
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

void IqnorePath::parseFile(size_t cpos, size_t position)
{
    m_filePattern = m_pattern.substr(position, cpos - position);
    if (m_filePattern.length() == 0) {
        m_anyFile = true;
    }
    else if (m_filePattern.length() == 1 && m_filePattern[0] == '*') {
        m_anyFile = true;
    }
}

bool IqnorePath::match(const char* path)
{
    std::string filePath = path;
    std::vector<std::string> matchList;
    size_t position = 0, currentPosition = 0;
    bool firstSep = true;
    while (currentPosition != -1)
    {
        
        currentPosition = filePath.find_first_of('\\', position);
        if (firstSep) {
            firstSep = false;
            if (!m_anyDrive) {
               std::string str = filePath.substr(position, currentPosition - position);
               if (matchDrive(str) == false) {
                   return false;
               }
            }
        } else if (currentPosition == -1) {
            if (!m_anyDrive) {
                std::string str = filePath.substr(position, currentPosition - position);
                if (matchFile(str) == false) {
                    return false;
                }
            }
        }
        else {
            std::string str = filePath.substr(position, currentPosition - position);
            matchList.push_back(str);
            
        }
        //directories.push_back(filePath.substr(position, currentPosition - position));
        position = currentPosition + 1;
    }
    if (matchFolder(matchList) == false) {
        return false;
    }
    
    return true;
}

bool IqnorePath::matchDrive(std::string &str)
{
    if (m_drivePattern[0] == str[0] && str[1] == ':') {
        return true;
    }
    return false;
}

bool IqnorePath::matchFolder(std::vector<std::string> matchList)
{
    std::string item1 = m_folderList[0];
    if (item1[0] == '*') {
        // rel path
    }
    else {
        if (matchList.size() != m_folderList.size()) {
            return false;
        }
        for (int i = 0; i < matchList.size(); i++) {
            if (matchList[i] != m_folderList[i]) {
                return false;
            }
        }
      
    }
    return true;
}

bool IqnorePath::matchFile(std::string& str)
{
    if (m_filePattern == str) {
        return true;
    }
    return false;
}
/*
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
*/

