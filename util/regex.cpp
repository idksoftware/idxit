// regex.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
size_t position = 0, currentPosition = 0;

    while (currentPosition != -1)
    {
        currentPosition = filePath.find_first_of('\\', position);
        directories.push_back(filePath.substr(position, currentPosition - position));
        position = currentPosition + 1;
    }
*/

#include <iostream>
#include <string>
#include <regex>

class MatchString {
    std::string m_pattern;
public:
    MatchString(const char* p)
    {
        std::string pattern = p;
        size_t pos = 0, cpos = 0;
        
        
        while (cpos != -1)
        {
            cpos = pattern.find_first_of("!*[?", pos);
            if (cpos == -1) {
                std::string tmp = pattern.substr(pos, pattern.length() - pos);
                if (tmp.length() > 0) {
                    m_pattern += '(';
                    m_pattern += tmp;
                    m_pattern += ')';
                }
                break;
            }
            switch (pattern[cpos]) {
            case '!':
                break;
            case '*':
                m_pattern += "(.*)";
                break;
            case '[':
                {
                std::string tmp = pattern.substr(pos, cpos - pos);
                m_pattern += '(';
                m_pattern += tmp;
                m_pattern += ')';
                m_pattern += "([";
                pos = cpos + 1;
                if (pattern[pos] == '!') {
                    m_pattern += "^";
                    pos++;
                }
                cpos = pattern.find_first_of(']', pos);
                tmp = pattern.substr(pos, cpos - pos);
                m_pattern += tmp;
                m_pattern += "])";
                }
                break;
            case '?':
                break;
            }
            pos = cpos + 1;
        }
        
    }

    bool match(std::string str)
    {
        std::regex regexStr(m_pattern);
        return std::regex_match(str, regexStr);
    }
};

/*
int main()
{
    MatchString matchString("*debug[0-9]");
    std::cout << ((matchString.match("debug99")) ? "True" : "False");


    
    return 0;
}
*/

/*
All name files, name folders, and files and folders in any name folder
*/
void test1()
{
    std::string pattern = "name";

    std::string match1 = "/name.log";
    std::string match2 = "/name/file.txt";
    std::string match3 = "/lib/name.log";
}

/*
Ending with / specifies the pattern is for a folder. Matches all files and folders in any name folder
*/
void test2()
{
    std::string pattern = "/name";

    std::string match1 = "/name/file.txt";
    std::string match2 = "/name/log/name.log";
    std::string no_match2 = "/name.log";    
}

/*
All files with the name.file
*/
void test3()
{
    std::string pattern = "name.file";

    std::string match1 = "/name.file";
    std::string match2 = "/lib/name.file";    
}
