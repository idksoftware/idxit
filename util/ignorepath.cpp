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



void IgnorePath::init()
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
    while (cpos != std::string::npos)
    {
        cpos = m_pattern.find_first_of('/', position);
        if (cpos == std::string::npos) {
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

void IgnorePath::parseEnv()
{
    std::string filePath = m_pattern;
    std::string newPath;
    std::vector<std::string> directories;
    size_t startpos = 0, endpos = 0;
    bool envfound = false;
    std::string startString;
    std::string endString;
    while (startpos != std::string::npos)
    {
        startpos = filePath.find("$(", endpos);
        if (startpos != std::string::npos) {
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


void IgnorePath::parseDrive(size_t cpos, size_t position)
{

    m_drivePattern = m_pattern.substr(position, cpos - position);

    if (m_drivePattern[0] == '*') {
        m_anyDrive = true;
    }
    //std::string m_drivePattern = m_pattern.substr(position, cpos - position);

}

void IgnorePath::parseDir()
{
    for (auto it : m_folderList)
        std::cout << it << std::endl;

}

void IgnorePath::parseFile(size_t cpos, size_t position)
{
    m_filePattern = m_pattern.substr(position, cpos - position);
    if (m_filePattern.length() == 0) {
        m_anyFile = true;
    }
    else if (m_filePattern.length() == 1 && m_filePattern[0] == '*') {
        m_anyFile = true;
    }
    else {
        if (m_pattern.find_first_of('.', position) == std::string::npos) {
            m_patternType = PatternType::AnyMatch;
            m_anyFile = true;

        }
        else {
            m_patternType = PatternType::AnyNamedFile;
            m_anyFile = false;
        }   
    }
}

void IgnorePath::parseFolder(size_t cpos, size_t position)
{
    m_folderPattern = m_pattern.substr(position, cpos - position);
    if (m_folderPattern.length() == 0) {
        m_anyDir = true;
        }
    else if (m_folderPattern.length() == 1 && m_folderPattern[0] == '*') {
        m_anyDir = true;
    }
    else {
       m_patternType = PatternType::AnyNamedFolder;
       m_anyDir = false;
       
    }
}

bool IgnorePath::simplePattern()
{
    int position = 0;
    int cpos = 0;
    m_anyDir = true;
    m_anyFile = true;
    bool isFolder = (m_pattern[m_pattern.length() - 1] == '/'); // last char is '/';
    bool isMultiFolder = false;
    bool isNameOnly = (m_pattern.find_first_of('/', position) == std::string::npos); // no char '/' found in string
    if (isFolder) {
        std::string folderStr = m_pattern.substr(position, m_pattern.length() - 1);
        isMultiFolder = (folderStr.find_first_of('/', position) != std::string::npos);
    }
    if (isNameOnly) {
            parseFile(m_pattern.length(), 0);
    }
    else if (!isMultiFolder) {
        parseFolder(m_pattern.length(), 0);
    }
    else {
        while (cpos != std::string::npos)
        {
            cpos = m_pattern.find_first_of('/', position);
            if (cpos == std::string::npos) {
                if (isFolder) {
                    break;
                }
                m_patternType = PatternType::AnyNamedFile;
                parseFile(cpos, position);

            }
            else {
                std::string str = m_pattern.substr(position, cpos - position);
                m_patternType = PatternType::AnyNamedFolder;
                m_folderList.push_back(str);
                m_anyDir = false;
            }
            position = cpos + 1;
        }
    }
   
    return true;
}
/*
The path to be matched will be in the form:
*:\folder\**\*
* i.e. C:\Windows\System32\readme.txt
*/
bool IgnorePath::match(const char* path)
{
    std::string filePath = path;
    
    bool rootPathSep = (filePath[2] == '/');
    switch (m_patternType) {
    case PatternType::AbsolutePath:
        return absolutePath(filePath);
    case PatternType::RelativePath:
        return relativePath(filePath);
    case PatternType::AnyNamedFile:
        return anyNamedFile(filePath);
    case PatternType::AnyNamedFolder:
        return anyNamedFolder(filePath);
    case PatternType::AnyMatch:
        return anyMatch(filePath);
    case PatternType::Unknown:
    default:
        break;
    }
    return false;
}

bool IgnorePath::matchDrive(std::string& str)
{
    if (m_anyDrive) {
        return true;
    }
    if (m_drivePattern[0] == str[0] && str[1] == ':') {
        return true;
    }
    return false;
}

bool IgnorePath::matchFolder(std::vector<std::string> matchList)
{
    if (m_anyDir) {
        return true;
    }
    if (matchList.empty()) {
        return false;
    }
    if (m_folderList.empty()) {
        return false;
    }
    std::string item1 = m_folderList[0];
    if (item1[0] == '*') {
        // rel path
    }
    else {
        // abs match
        if (matchList.size() < m_folderList.size()) {
            return false;
        }
        for (int i = 0; i < m_folderList.size(); i++) {
            if (matchList[i] != m_folderList[i]) {
                return false;
            }
        }

    }
    return true;
}

bool IgnorePath::matchFile(std::string& str)
{
    if (m_anyFile) {
        return true;
    }
    if (m_filePattern == str) {
        return true;
    }
    return false;
}

const char* IgnorePath::patternTypeString()
{
    switch (m_patternType) {
    case PatternType::AbsolutePath: return "Absolute Path";
    case PatternType::RelativePath: return "Relative Path";
    case PatternType::AnyNamedFile: return "Any Named File";
    case PatternType::AnyNamedFolder: return "Any Named Folder";
    case PatternType::AnyMatch: return "Any Named Folder";
    case PatternType::Unknown:
    default:
        break;
    }
    return "Unknown";
}


bool IgnorePath::absolutePath(std::string &filePath)
{
    std::vector<std::string> matchList;
    matchList.push_back("/");
    std::replace(filePath.begin(), filePath.end(), '\\', '/');
    std::string str = filePath.substr(0, 2);
    if (matchDrive(str) == false) {
        return false;
    }
    size_t position = 3, currentPosition = 3;
    while (currentPosition != std::string::npos)
    {

        currentPosition = filePath.find_first_of('/', position);

        if (currentPosition == std::string::npos) {
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

bool IgnorePath::relativePath(std::string& filePath)
{
    return true;
}

bool IgnorePath::anyNamedFile(std::string& filePath)
{
    return true;
}

bool IgnorePath::anyNamedFolder(std::string& filePath)
{
    return true;
}

bool IgnorePath::anyMatch(std::string& filePath)
{
    return true;
}

