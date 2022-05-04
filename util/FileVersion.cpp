#include "FileVersion.h"

bool FileVersion::increment()
{
    // dsc9807[89].jpg
    std::string name = fileNoExtention(m_file.c_str());
    std::size_t botDirPos = name.find_last_of("[");
    if (botDirPos == std::string::npos) {
        name += "[01].";
        name += fileExtention(m_file.c_str());
        m_result = name;
    }
    else {
        std::string prename = name.substr(0, botDirPos);
        std::string versionStr = name.substr(botDirPos + 1, name.length() - (botDirPos + 1));
        std::size_t botDirPos = versionStr.find_last_of("]");
        versionStr = versionStr.substr(0, botDirPos);
        m_version = std::stoi(versionStr);
        name = prename;
        name += '[';
        versionStr = std::to_string(++m_version);
        if (versionStr.length() <= 1) {
            versionStr = '0' + versionStr;
        }
        name += versionStr;
        name += "].";
        name += fileExtention(m_file.c_str());
        m_result = name;
    }
    return true;
}