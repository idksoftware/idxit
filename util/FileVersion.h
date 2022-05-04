#pragma once
#include <string>

class FileVersion {
    std::string m_file;
    std::string m_result;
    int m_version;


    std::string fileExtention(const char* f)
    {
        std::string fullname = f;
        size_t lastindex = fullname.find_last_of(".");
        return fullname.substr(lastindex + 1, fullname.length() - (lastindex + 1));
    }

    std::string fileNoExtention(const char* f)
    {
        std::string fullname = f;
        size_t lastindex = fullname.find_last_of(".");
        return fullname.substr(0, lastindex);
    }

public:
    FileVersion(const char* f) {
        m_file = f;
        m_result = m_file;
    }
    bool increment();
    std::string getFilename() {
        return m_result;
    }
};

