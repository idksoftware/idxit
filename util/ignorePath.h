#pragma once

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>

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
    void parseDrive(size_t cpos, size_t position);
    void parseFile(size_t cpos, size_t position);
    void parseDir();

    bool matchDrive(std::string& str);
    bool matchFolder(std::vector<std::string> matchList);
    bool matchFile(std::string& str);
    
public:
    IqnorePath(const char* pattern) : m_pattern(pattern)
    {
        init();
    }
    ~IqnorePath() = default;
    bool match(const char* path);

    void print()
    {
        std::cout << "************************" << std::endl;
        std::cout << "Pattern: " << m_pattern << std::endl;
        std::cout << "Any Drive: " << ((m_anyDrive) ? "True" : "False") << std::endl;
        std::cout << "Drive: " << m_drivePattern << std::endl;
        std::cout << "Any Dir: " << ((m_anyDir) ? "True" : "False") << std::endl;
        std::cout << "Folder list: " << std::endl;
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
