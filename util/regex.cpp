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
    std::string pattern = "name/";

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

/*
Starting with / specifies the pattern matches only files in the root folder
*/
void test4()
{
    std::string pattern = "/name.file";

    std::string match1 = "/name.file";
    std::string no_match2 = "/lib/name.file";

}

/*
Patterns specifiing files in specific folders are always realative to root (even if you do not start with / )
*/
void test5()
{
    std::string pattern = "lib/name.file";

    std::string match1 = "/name.file";
    std::string no_match1 = "name.file";
    std::string no_match2 = "/test/lib/name.file";

}

/*
Starting with ** before / specifies that it matches any folder in the repository. Not just on root.
*/
void test6()
{
    std::string pattern = "**/lib/name.file";

    std::string match1 = "/lib/name.file";
    std::string match2 = "/test/lib/name.file";
   
}

/*
All name folders, and files and folders in any name folder
*/
void test7()
{
    std::string pattern = "**/name";

    std::string match1 = "/name/log.file";
    std::string match2 = "/lib/name/log.file";
    std::string match3 = "/name/lib/log.file";
}

/*
All name folders, and files and folders in any name folder within the lib folder.
*/
void test8()
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
void test9()
{
    std::string pattern = "*.file";

    std::string match1 = "/name.file";
    std::string match2 = "/lib/name.file";
}

/*
All folders ending with name
*/
void test10()
{
    std::string pattern = "*name/";

    std::string match1 = "/lastname/log.file";
    std::string match2 = "/firstname/log.file";
}

/*
? matches a single non-specific character
*/
void test11()
{
    std::string pattern = "name?.file";

    std::string match1 = "/names.file";
    std::string match2 = "/name1.file";

    std::string no_match1 = "/names1.file";

}

/*
[range] matches a single character in the specified range (in this case a character in the range of a-z, and also be numberic.)
*/
void test12()
{
    std::string pattern = "name[a-z].file";

    std::string match1 = "/names.file";
    std::string match2 = "/nameb.file";

    std::string no_match1 = "/name1.file";
    
}

/*
[set] matches a single character in the specified set of characters (in this case either a, b, or c)
*/
void test13()
{
    std::string pattern = "name[abc].file";

    std::string match1 = "/namea.file";
    std::string match2 = "/nameb.file";

    std::string no_match1 = "/names.file"; 
}

/*
[!set] matches a single character, except the ones spesified in the set of characters (in this case a, b, or c)
*/
void test14()
{
    std::string pattern = "name[!abc].file";

    std::string match1 = "/names.file";
    std::string match2 = "/namex.file";

    std::string no_match1 = "/namesb.file";
    
}

/*
!specifies a negation or exception. Matches all files and folders in any name folder, except name/secret.log
*/
void test15()
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
void test16()
{
    std::string pattern = "name/";
    std::string pattern1 = "!name/secret.log";

    std::string match1 = "/name/file.txt";
    std::string match2 = "/name/log/name.log";

    std::string no_match1 = "/name/secret.log";

}

