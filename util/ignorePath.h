#pragma once

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>

class IgnorePath {
    enum class PatternType {
        AbsolutePath,       // "*:/windows/", "/name.file"
        RelativePath,       // "**/lib/name.file", "/lib/**/name", "**/name/"
        AnyNamedFile,       // "name.*"
        AnyNamedFolder,     // "name/"
        AnyMatch,           // "name"
        Unknown
    };

    PatternType m_patternType{ PatternType::Unknown };

    std::string m_pattern;
    std::string m_drivePattern;
    std::string m_filePattern;
    std::string m_folderPattern; // used for single folder
    bool m_root{ false };
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
    void parseFolder(size_t cpos, size_t position);
    void parseDir();
    bool simplePattern();
    const char* patternTypeString();

    bool matchDrive(std::string& str);
    bool matchFolder(std::vector<std::string> matchList);
    bool matchFile(std::string& str);

    int m_line{ 0 };
    std::string m_ignFile;

    bool absolutePath(std::string& filePath);
    bool relativePath(std::string& filePath);
    bool anyNamedFile(std::string& filePath);
    bool anyNamedFolder(std::string& filePath);
    bool anyMatch(std::string& filePath);
    

public:
    IgnorePath(const char* pattern, int line, const char *ignFile)
        : m_pattern(pattern), m_line(line), m_ignFile(ignFile)
    {
        init();
    }
    ~IgnorePath() = default;
    bool match(const char* path);
    const char* getPattern() { return m_pattern.c_str(); }

    void print()
    {
        std::cout << "************************" << std::endl;
        std::cout << "Pattern: " << m_pattern << " Type: " << patternTypeString() << std::endl;
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

    int getLine() { return m_line;  };
    const char* getIgnFile() { return m_ignFile.c_str(); };
};
