#include <windows.h>
#include "JounalVisitor.h"

//std::shared_ptr<XMLFileInfoWriter> ScanVisitor::m_xmlFileInfoWriter = nullptr;
std::string JounalVisitor::m_sourcePath;

uint64_t JounalVisitor::m_files = 0;
uint64_t JounalVisitor::m_folders = 0;
uint64_t JounalVisitor::m_size = 0;
