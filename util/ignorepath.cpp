// ignorepath.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "ignorePath.h"
#include <algorithm>

#include <functional>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#ifdef XXXXXXX
AbsolutePath,       // file or folder depending on ending "*:/windows/", "/lib/**/name", "/name.file"
RelativePath,       // file or folder depending on ending "**/lib/name.file", "**/name/"
AllNamedFile,       // "name"
AnyNamedFolder,     // "name/"
Unknown
#endif // XXXXXXX

void IqnorePath::init()
{
    parseEnv();
    size_t position = 0, cpos = 0;

    std::replace(m_pattern.begin(), m_pattern.end(), '\\', '/');

    bool firstSep = true;
    bool driveSep = true;
    cpos = m_pattern.find_first_of(':', position);
    // find drive
    if (cpos == 1) {// drive ie c:
        parseDrive(cpos + 1, position);
        cpos++;
        m_patternType = PatternType::AbsolutePath;
    }
    else {
        m_anyDrive = true;
        cpos = 0; // no drive so reset to folder/files char
    }
    
    if (m_pattern[cpos] == '/') {
        m_patternType = PatternType::AbsolutePath;
        m_folderList.push_back("/");
        m_root = true;
        cpos++;
    } else if (m_pattern[cpos] == '*' && m_pattern[cpos+1] == '*') { // ** found so relativePath
        m_patternType = PatternType::RelativePath;
        m_root = false;
        cpos++;
    }
    else {
        simplePattern();
        return;
    }
    cpos = m_pattern.find_first_of('/', 0);
    position = cpos;
    position++;
    while (cpos != -1)
    {
        cpos = m_pattern.find_first_of('/', position);
        if (cpos == -1) {
            parseFile(cpos, position);
        }
        else {
            std::string str = m_pattern.substr(position, cpos - position);
            m_folderList.push_back(str);
        }
        position = cpos + 1;
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

    m_drivePattern = m_pattern.substr(position, cpos - position);

    if (m_drivePattern[0] == '*') {
        m_anyDrive = true;
    }
    //std::string m_drivePattern = m_pattern.substr(position, cpos - position);

}

void IqnorePath::parseDir()
{
    for (auto it : m_folderList)
        std::cout << it << std::endl;

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


#ifdef XXXXXXX

RelativeFilePath,   // "**/lib/name.file"
RelativeFolderPath, // "**/name/"
AllNamedFile,       // "name"
AnyNamedFolder,     // "name/"
Realative2Root,     // "/name.file", "lib/name.file", , "/lib/**/name"
Unknown
#endif // XXXXXXX
bool IqnorePath::simplePattern()
{
    int position = 0;
    int cpos = 0;
    m_anyDir = true;
    m_anyFile = true;
    while (cpos != -1)
    {
        cpos = m_pattern.find_first_of('/', position);
        if (cpos == -1) {
            m_patternType = PatternType::AllNamedFile;
            parseFile(cpos, position);
            m_anyFile = false;
        }
        else {
            std::string str = m_pattern.substr(position, cpos - position);
            m_patternType = PatternType::AnyNamedFolder;
            m_folderList.push_back(str);
            m_anyDir = false;
        }
        position = cpos + 1;
    }
    //int cpos = m_pattern.find_first_of('/', 0);
    

    // name
    // name.file
    // *.file
    return true;
}
/*
The path to be matched will be in the form:
*:\folder\**\*
* i.e. C:\Windows\System32\readme.txt
*/
bool IqnorePath::match(const char* path)
{
    std::string filePath = path;
    std::vector<std::string> matchList;

    std::replace(filePath.begin(), filePath.end(), '\\', '/');
    std::string str = filePath.substr(0, 2);
    if (matchDrive(str) == false) {
        return false;
    }
    size_t position = 3, currentPosition = 3;
    while (currentPosition != -1)
    {

        currentPosition = filePath.find_first_of('/', position);

        if (currentPosition == -1) {
            if (!m_anyFile) {
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

bool IqnorePath::matchDrive(std::string& str)
{
    if (m_anyDrive) {
        return true;
    }
    if (m_drivePattern[0] == str[0] && str[1] == ':') {
        return true;
    }
    return false;
}

bool IqnorePath::matchFolder(std::vector<std::string> matchList)
{
    if (m_anyDir) {
        return true;
    }
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
    if (m_anyFile) {
        return true;
    }
    if (m_filePattern == str) {
        return true;
    }
    return false;
}

const char* IqnorePath::patternTypeString()
{
    switch (m_patternType) {
    case PatternType::AbsolutePath: return "Absolute Path";
    case PatternType::RelativePath:return "Relative Path";
    case PatternType::AllNamedFile:return "Any Named File";
    case PatternType::AnyNamedFolder:return "Any Named Folder";
    case PatternType::Unknown:
    default:
        break;
    }
    return "Unknown";
}



