//@@>>SAUtils.cpp
// =========== SAUtils.cpp ===========
//
/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>
#include <crtdbg.h>
#endif

//#include "CIDKFile.h"
#include <algorithm>
//#include "CIDKFileFind.h"
#include "SAUtils.h"

#include <cstdlib>
#include <istream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <cstdio>
#include <vector>
#include <cstring>
#include <sstream>
#include <cstdarg>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include <filesystem>
//#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>

#define getcwd _getcwd // stupid MSFT "deprecation" warning
#else
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cerrno>
//#include "cport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using namespace std;

const char* SIAException::what() throw()
{
	std::stringstream s;
	s << message << " line:" << __LINE__ << " file:" << __FILE__;
	std::string tmp = s.str();
	message = s.str();
	return message.c_str();
}


SAUtils::SAUtils()
{
	// TODO Auto-generated constructor stub
}

SAUtils::~SAUtils()
{
	// TODO Auto-generated destructor stub
}

bool SAUtils::FileExists(const char* p)
{
#ifdef STDCPP14
	struct stat buffer;
	if (stat(p, &buffer) != 0)
	{
		return false;
	}
	return true;
#else
	std::filesystem::path path = p;
	if (std::filesystem::exists(path) == true) {
		return std::filesystem::is_regular_file(path);
	}
	return false;
#endif
}

bool SAUtils::DirExists(const char* p)
{
#ifdef STDCPP14
	struct stat info;
	if (stat(path, &info) != 0) {
		return false;
	}
	if (info.st_mode & S_IFDIR) {
		return true;
	}
	return false;
#else
	std::filesystem::path path = p;
	error_code ec;
	if (std::filesystem::exists(path, ec) == true) {
		return std::filesystem::is_directory(path);
	}
	return false;
#endif
}


/*
FileList_Ptr SAUtils::getFiles_(const char* dirpath)
{
	//FileList_Ptr fileList(new std::vector<std::string>);
	FileList_Ptr fileList = std::make_unique<FileList>();

	//DIR *dir;
	//struct dirent *ent;
	//if ((dir = opendir(dirpath)) == NULL) {
	//return false;
	//}
	//while ((ent = readdir(dir)) != NULL) {
	//printf("%s", ent->d_name);
	//fileList->push_back(new std::string(ent->d_name));
	//}

	std::string dirpathstr(dirpath);
	dirpathstr = dirpathstr + "/*.*";
	CIDKFileFind fileFind(dirpathstr);
	fileFind.Open();
	if (fileFind.GotFile() == false)
	{
		return fileList;
	}
	do
	{
		std::string tmp(fileFind.GetFileName());
		fileList->emplace_back(tmp);
	} 	while (fileFind.GetNext());

	return fileList;
}
*/

FileList_Ptr SAUtils::getFiles_(const char* dirpath)
{
	return SAUtils::getFiles(dirpath);
}

FileList_Ptr SAUtils::getFiles(const char* dirpath)
{
	FileList_Ptr fileList = std::make_unique<FileList>();
	for (auto const& dir_entry : std::filesystem::directory_iterator{ dirpath })
	{
		std::filesystem::path p = dir_entry.path();
		std::filesystem::path f = p.filename();
		std::string fileItem = f.string();
		std::string pathStr = p.string();
		fileList->emplace_back(fileItem);
	}
	return fileList;
}


bool SAUtils::IsFile(const char* path)
{
#ifdef STDCPP14
	struct stat info;
	if (stat(path, &info) != 0)
	{
		return false;
	}
	if (!(info.st_mode & S_IFDIR))
	{
		// S_ISDIR
		return true;
	}
#else
	return std::filesystem::is_regular_file(path);
#endif
}

time_t SAUtils::createTime(const char* path)
{
	struct stat info;

	if (stat(path, &info) != 0)
	{
		return 0;
	}
	return info.st_ctime;
}

time_t SAUtils::modTime(const char* path)
{
	struct stat info;

	if (stat(path, &info) != 0)
	{
		return 0;
	}
	return info.st_mtime;
}

uint64_t SAUtils::fileSize(const char* path)
{
	uint64_t size = std::filesystem::file_size(path);
	return size;
}

bool SAUtils::hasExt(const char* file)
{
	std::string filestr = file;
	return hasExt(filestr);
}

bool SAUtils::hasExt(const std::string& file)
{
	unsigned int i = 0;
	if ((i = static_cast<unsigned>(file.find_last_of("."))) == static_cast<unsigned>(-1))
	{
		return false;
	}
	if (i >= (file.length() - 1))
	{
		// find . but no extention i.e "."
		return false;
	}
	return true;
}

std::string SAUtils::getExtention(const std::string& file)
{
	std::string ext = file.substr(file.find_last_of(".") + 1);
	return ext;
}

std::string SAUtils::getExtention(const char* file)
{
	std::string filestr = file;
	return getExtention(filestr);
}

std::string SAUtils::getFilePathNoExt(const char* path)
{

	std::filesystem::path p = path;
	return p.replace_extension().string();
}

std::string SAUtils::getFilePathNoExt(const std::string& path)
{
	std::filesystem::path p = path;
	return p.replace_extension().string();
}

std::string SAUtils::getFilename(const std::string& filepath)
{
#ifdef STDCPP14
	size_t sp = filepath.find_last_of("/");
	if (sp == string::npos)
	{
		sp = filepath.find_last_of("\\");
	}
	if (sp == string::npos)
	{
		return filepath;
	}
	std::string name = filepath.substr(++sp);
	return name;
#else
	std::filesystem::path p = filepath;
	return p.filename().string();
#endif
}

std::string SAUtils::getFolder(const std::string& filepath)
{
#ifdef STDCPP14
	size_t sp = filepath.find_last_of("/");
	if (sp == string::npos)
	{
		sp = filepath.find_last_of("\\");
	}
	if (sp == string::npos)
	{
		return filepath;
	}
	std::string name = filepath.substr(0, filepath.length() - (sp + 1));
	return name;
#else
	std::filesystem::path p = filepath;
	return p.parent_path().string();
#endif
}


std::string SAUtils::getFilenameNoExt(const std::string& file)
{
#ifdef STDCPP14
	size_t sp = file.find_last_of("/\\");

	std::string name;
	if (sp != -1)
	{
		name = file.substr(++sp);
	}
	const size_t ep = file.find_last_of('.');
	name = file.substr(0, ep);
	return name;
#else
	std::filesystem::path p = file;
	std::filesystem::path f = p.filename();
	return f.replace_extension().string();
#endif
}

std::string SAUtils::getFilenameNoExt(const char* file)
{
	std::string filestr = file;
	return getFilenameNoExt(filestr);
}


std::string SAUtils::getCurrentDirectory()
{
#ifdef STDCPP14
	std::string s_cwd(getcwd(NULL, 0));
	return s_cwd;
#else
	return std::filesystem::current_path().string();
#endif
}

bool SAUtils::setCurrentDirectory(const std::string& filepath)
{
	error_code ec;
	std::filesystem::current_path(filepath, ec);
	return (ec.value() != 0);
}

std::string SAUtils::to_string(int i)
{
	std::stringstream tmp;
	tmp << i;
	return std::string(tmp.str());
}

bool SAUtils::isEquals(const std::string& a, const std::string& b)
{
	return std::equal(a.begin(), a.end(),
		b.begin(), b.end(),
		[](char a, char b)
		{
			return tolower(a) == tolower(b);
		});
}

bool SAUtils::boolOptionValue(BoolOption b) {
	if (b == BoolOption::True) return true;
	if (b == BoolOption::False) return false;
	throw std::exception();
	return false;
}

BoolOption SAUtils::isTrueFalse(std::string& s)
{
	if (isEquals("enabled", s) || isEquals("true", s) || isEquals("on", s) || isEquals("yes", s))
	{
		s = "True";
		return BoolOption::True;
	}
	if (isEquals("disabled", s) || isEquals("false", s) || isEquals("off", s) || isEquals("no", s))
	{
		s = "False";
		return BoolOption::False;
	}
	s = "Invalid";
	return BoolOption::Invalid;
}

bool SAUtils::mkDir(const char* path)
{
#ifdef STDCPP14
#ifdef _WIN32
	if (_mkdir(path) != 0)
	{
#else
	mode_t mode = 0777;
	if (mkdir(path, mode) != 0) {
#endif
		return false;
	}
#else
	return std::filesystem::create_directory(path);
#endif

	}

bool SAUtils::isNumber(const std::string & s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

bool SAUtils::setHidden(const char* path)
{
#ifdef _WIN32
	DWORD dwAttrs;

	dwAttrs = GetFileAttributes(path);
	if (dwAttrs == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}
	if (!(dwAttrs & FILE_ATTRIBUTE_HIDDEN))
	{
		SetFileAttributes(path, dwAttrs | FILE_ATTRIBUTE_HIDDEN);
	}
#endif
	return true;
}

bool SAUtils::isHidden(const char* path)
{
#ifdef _WIN32
	DWORD dwAttrs;

	dwAttrs = GetFileAttributes(path);
	if (dwAttrs == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}
	if ((dwAttrs & FILE_ATTRIBUTE_HIDDEN))
	{
		return true;
	}
#endif
	return false;
}

#define BUFFER_SIZE 0x1000000

bool SAUtils::fileCompare(const char* filePath1, const char* filePath2)
{
	std::ifstream file1(filePath1, std::ifstream::in | std::ifstream::binary);
	std::ifstream file2(filePath1, std::ifstream::in | std::ifstream::binary);

	if (!file1.is_open() || !file2.is_open())
	{
		return false;
	}

	unsigned char* buffer1 = new unsigned char[BUFFER_SIZE]();
	unsigned char* buffer2 = new unsigned char[BUFFER_SIZE]();
	bool flag = true;
	do
	{
		file1.read((char*)buffer1, BUFFER_SIZE);
		file2.read((char*)buffer2, BUFFER_SIZE);
		if (file1.gcount() != file2.gcount())
		{
			flag = false;
			break;
		}
		if (memcmp(buffer1, buffer2, static_cast<size_t>(file1.gcount())) != 0)
		{
			flag = false;
			break;
		}
	} while (file1.good() || file2.good());

	delete[]buffer1;
	delete[]buffer2;

	return true;
}

bool SAUtils::fileSize(const char* filePath, uintmax_t & size)
{
#ifdef STDCPP14
	struct stat info;
	if (stat(filePath, &info) != 0)
	{
		return false;
	}
	*size = info.st_size;
	return true;
#else
	std::error_code ec;
	size = std::filesystem::file_size(filePath, ec);
	return (!ec);
#endif
}

bool SAUtils::rename(const char* from, const char* to)
{
#ifdef STDCPP14
	int result = ::rename(from, to);
	perror("file error");
	return (result >= 0);
#else
	const std::filesystem::path old_p = from;
	const std::filesystem::path new_p = to;
	std::error_code ec;
	std::filesystem::rename(old_p, new_p, ec);
	return (!ec);
#endif
}

bool SAUtils::copy(const char* from, const char* to)
{
#ifdef STDCPP14
	char buf[BUFSIZ];
	size_t size;

	FILE* source = nullptr;
	fopen_p(source, from, "rb");
	if (source == nullptr)
	{
		return false;
	}
	FILE* dest = nullptr;
	fopen_p(dest, to, "wb");
	if (dest == nullptr)
	{
		printf("%s", strerror(errno));
		return false;
	}
	while ((size = fread(buf, 1, BUFSIZ, source)) > 0)
	{
		fwrite(buf, 1, size, dest);
	}
	fclose(source);
	fclose(dest);

	return true;
#else
	const std::filesystem::path src = from;
	const std::filesystem::path dst = to;
	std::error_code ec;

	std::filesystem::copy_file(src, dst, std::filesystem::copy_options::overwrite_existing, ec);
	return (!ec);;
#endif

}


bool SAUtils::verify(const char* from, const char* to)
{
	return fileCompare(from, to);
}



#define TO_HEX(i) (i <= 9 ? '0' + i : 'a' - 10 + i)

void SAUtils::chartohex3(char* buffer, uint16_t x)
{
	buffer[0] = TO_HEX(((x & 0x0F00) >> 8));
	buffer[1] = TO_HEX(((x & 0x00F0) >> 4));
	buffer[2] = TO_HEX((x & 0x000F));
	buffer[3] = '\0';
}

void SAUtils::chartohex2(char* buffer, uint8_t x)
{
	buffer[0] = TO_HEX(((x & 0x00F0) >> 4));
	buffer[1] = TO_HEX((x & 0x000F));
	buffer[2] = '\0';
}


// Note this MUST be a const char *format not std::string
std::string SAUtils::sprintf(const char* fmt, ...)
{
	size_t final_n, n = (strlen(fmt) * 2); // Reserve two times as much as the length of the fmt_str //
	std::string str;

	va_list ap;
	while (true)
	{
		//formatted.reset(new char[n]); // Wrap the plain char array into the unique_ptr
		auto formatted = std::make_unique<char[]>(n);

		va_start(ap, fmt);
		final_n = vsnprintf(formatted.get(), n, fmt, ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
		{
			n += abs(static_cast<int>(final_n - n + 1));
		}
		else
		{
			str = formatted.get();
			break;
		}
	}
	return str;
}


bool SAUtils::delDir(const char* path)
{
#ifdef STDCPP14
#ifdef _WIN32
	if (_rmdir(path) != 0)
	{
#else
	mode_t mode = 0777;
	if (mkdir(path, mode) != 0) {
#endif
		return false;
	}
	return true;
#else
	const std::filesystem::path p = path;
	if (std::filesystem::is_directory(p) == true) {
		return std::filesystem::remove(p);
	}
	return false;
#endif
	}

bool SAUtils::delFile(const char* file)
{
#ifdef STDCPP14
	if (::unlink(file) < 0)
	{
		return false;
	}
	return true;
#else
	const std::filesystem::path p = file;
	if (std::filesystem::is_regular_file(p) == true) {
		return std::filesystem::remove(p);
	}
	return false;
#endif
}

bool SAUtils::makePath(const char* from, const char* to)
{
#ifdef STDCPP14
	if (DirExists(from) == false)
	{
		if (mkDir(from) == false)
		{
			return false;
		}
	}
	std::string fullPath = from;
	fullPath += '/';
	fullPath += to;

	std::string curPath = from;
	bool last = false;
	std::string node;
	while (last != true)
	{
		size_t start = curPath.length();
		size_t end = 0;
#ifdef _WIN32
		if ((end = fullPath.find_first_of("\\", start + 2)) == std::string::npos)
		{
#else
		if ((end = fullPath.find_first_of("/", start + 2)) == std::string::npos) {
#endif
			//		if ((end = fullPath.find_first_of("/", start+2)) == std::string::npos) {
			node = fullPath.substr(start + 1, (fullPath.length() - start) - 1);
			last = true;
		}
		else
		{
			node = fullPath.substr(start + 1, (end - 1) - (start));
		}

#ifdef _WIN32
		curPath += '\\';
#else
		curPath += '/';
#endif
		//		curPath += '/';
		curPath += node;
		node.clear();
		if (DirExists(curPath.c_str()) == false)
		{
			if (mkDir(curPath.c_str()) == false)
			{
				return false;
			}
		}
		}
	return true;
#else
	const std::filesystem::path& f = from;
	const std::filesystem::path& t = to;
	const std::filesystem::path& p = f / t;
	return std::filesystem::create_directories(p);
#endif
	}

bool SAUtils::makePath(const char* to)
{
#ifdef STDCPP14
	std::string fullPath = to;
	if (fullPath.empty())
	{
		return false;
	}
	size_t idx = fullPath.find_first_of(':');
	std::string drive = fullPath.substr(0, idx + 1);
	bool last = false;
	size_t start = idx + 1;
	size_t end = 0;
	std::string curPath = drive;
	std::string node;
	while (last != true)
	{
#ifdef _WIN32
		if ((end = fullPath.find_first_of("\\/", start + 2)) == std::string::npos)
		{
#else
		if ((end = fullPath.find_first_of("/", start + 2)) == std::string::npos) {
#endif
			node = fullPath.substr(start + 1, (fullPath.length() - start + 1));
			last = true;
		}
		else
		{
			node = fullPath.substr(start + 1, (end)-(start + 1));
		}
#ifdef _WIN32
		curPath += "\\";
#else
		curPath += '/';
#endif
		curPath += node;
		node.clear();
		if (DirExists(curPath.c_str()) == false)
		{
			if (mkDir(curPath.c_str()) == false)
			{
				return false;
			}
		}
		start = curPath.length();
		}
	return true;
#else
	const std::filesystem::path& t = to;
	return std::filesystem::create_directories(to);
#endif
	}


bool SAUtils::makeLink(const char* file, const char* lnk)
{
#ifdef STDCPP14
#ifdef _WIN32
	if (CreateSymbolicLink(link, file, 0x0) == false)
	{
		unsigned int err = GetLastError();
		printf("Error %d", err);
	}
#else
	if (symlink(file, link) < 0) {
		perror("symlink");
		return false;
	}
#endif
	return true;
#else
	const std::filesystem::path& target = file;
	const std::filesystem::path& link = lnk;
	std::error_code ec;
	std::filesystem::create_symlink(target, link, ec);
	return (!ec);
#endif
}

void SAUtils::splitpath(const char* path, char* drive, char* dir, char* fname, char* ext)
{
#ifdef _WIN32
	_splitpath_s(path, drive, 10, dir, 256, fname, 64, ext, 20);
#endif
}

/*
size_t driveNumberOfElements,
char * dir,
size_t dirNumberOfElements,
char * fname,
size_t nameNumberOfElements,
char * ext,
size_t extNumberOfElements
*/

int SAUtils::getFileContents(const char* filename, std::string & contents)
{
	int count = 0;
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize(static_cast<const unsigned>(in.tellg()));
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return 0;
	}
	return errno;
}

std::string SAUtils::getYear(const char* path)
{
	std::string fpath = path;
	return fpath.substr(0, 4);
}

std::string SAUtils::getFullRelativePath(const char* path)
{
	std::string spath = getYear(path);
	spath += '/';
	spath += path;
	return spath;
}

bool SAUtils::mksymlink(const char* sourcePath, const char* destPath)
{
	return true;
}

int SAUtils::stricmp(const char* a, const char* b)
{
	int ca, cb;
	do
	{
		ca = static_cast<unsigned char>(*a++);
		cb = static_cast<unsigned char>(*b++);
		ca = tolower(toupper(ca));
		cb = tolower(toupper(cb));
	} while (ca == cb && ca != '\0');
	return ca - cb;
}

bool SAUtils::IsAdminMode()
{
	bool fRet = false;
#ifdef WIN32
	HANDLE hToken = nullptr;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
		{
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken)
	{
		CloseHandle(hToken);
	}
#else
	if (geteuid()) { // The EFFECTIVE UID
		// You are not root! or sudo
		fRet = false;
	}
	else {
		//OK, you are root.
		fRet = true;
	}
#endif
	return fRet;
}

bool SAUtils::SetEnv(const std::string & key, const std::string & value, bool all)
{
#ifdef WIN32
	HKEY hkey;
	DWORD dwDisposition;
	DWORD dwType, dwSize;
	LONG result;
	if (all)
	{
		const char* regPath = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
		result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, regPath,
			0, nullptr, 0, KEY_WRITE, nullptr, &hkey, &dwDisposition);
	}
	else
	{
		const char* regPath = "Environment";
		result = RegCreateKeyEx(HKEY_CURRENT_USER, regPath,
			0, nullptr, 0, KEY_WRITE, nullptr, &hkey, &dwDisposition);
	}

	if (result == ERROR_SUCCESS)
	{
		dwType = REG_SZ;
		dwSize = (DWORD)(value.length() + 1);
		const LONG setResult = RegSetValueEx(hkey, TEXT(key.c_str()), 0, dwType,
			(PBYTE)value.c_str(), dwSize);
		RegCloseKey(hkey);
		return setResult == ERROR_SUCCESS;
	}
	return false;
#else
	return false;
#endif
}


#ifdef XXXXXX
bool GetEnv(const char* szSIAHome, std::string & resultStr, bool all)
{
	HKEY   hkey;
	DWORD  dwDisposition;
	DWORD dwType, dwSize;
	LONG result;
	/*
	if (all) {
		char *regPath = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
		result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, regPath,
			0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition);
	}
	else {
		char *regPath = "Environment";
		result = RegCreateKeyEx(HKEY_CURRENT_USER, regPath,
			0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition);
	}
	*/
	char* regPath = "Environment";
	char path[MAX_PATH + 1];
	DWORD size = MAX_PATH;
	result = RegQueryValueEx(HKEY_CURRENT_USER, regPath, nullptr, nullptr, (LPBYTE)path, &size);
	resultStr = path;
	return true;
}
#endif

std::string SAUtils::GetPOSIXEnv(const std::string & key)
{
	char* var = nullptr;
#ifdef _WIN32
	size_t pReturnValue = 0;
	char buffer[2 * 1024];
	size_t numberOfElements = 2 * 1024;
	errno_t res = getenv_s(&pReturnValue, buffer, numberOfElements, key.c_str());
	if (res == 0)
	{
		var = buffer;
	}

#else
	var = getenv(key.c_str());
#endif
	std::string retval;
	if (var != nullptr)
	{
		retval = var;
	}
	return retval;
}


std::string SAUtils::GetEnv(const std::string& value, bool all)
{
#ifdef WIN32
	HKEY hKey = nullptr;
	char buf[MAX_PATH];
	DWORD dwType = 0;
	DWORD dwBufSize = MAX_PATH;
	std::string res;
	bool error = false;
	if (all)
	{
		const char* subkey = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		{
			error = true;
		}
	}
	else
	{
		const char* subkey = "Environment";
		if (RegOpenKeyEx(HKEY_CURRENT_USER, subkey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		{
			error = true;
		}
	}
	if (!error) {
		if (RegQueryValueEx(hKey, value.c_str(), nullptr, nullptr, (BYTE*)buf, &dwBufSize) != ERROR_SUCCESS)
			//if (RegQueryValueEx(hKey, "IDXIT_HOME", NULL, NULL, (BYTE*)buf, &dwBufSize) != ERROR_SUCCESS)
		{
			error = true;
		}
	}
	if (!error) {
		res = buf;
	}
	RegCloseKey(hKey);
	return res;
#else
	char* var = getenv(value.c_str());
	std::string retval;
	if (var != NULL) {
		retval = var;
		return retval;
	}
	std::string res;
	return res;
#endif
}

#ifdef XXXXX
std::string SAUtils::GetEnv(const std::string & key, bool all)
{
	char* var = getenv(key.c_str());
	std::string retval;
	if (var != NULL) {
		retval = var;
		return retval;
	}
	return retval;
}

bool SAUtils::SetEnv(const std::string & key, const std::string & value, bool all)
{
	if (setenv(key.c_str(), value.c_str(), 1) != 0) {
		return false;
	}
	return true;
}

std::string SAUtils::GetPOSIXEnv(const std::string & key)
{
	return GetEnv(key, true);
}
#endif


std::string SAUtils::humanSize(uint64_t bytes)
{
	char* suffix[] = { "B", "KB", "MB", "GB", "TB" };
	char length = sizeof(suffix) / sizeof(suffix[0]);

	int i = 0;
	double dblBytes = bytes;

	if (bytes > 1024) {
		for (i = 0; (bytes / 1024) > 0 && i < length - 1; i++, bytes /= 1024)
			dblBytes = bytes / 1024.0;
	}

	std::stringstream str;
	str << dblBytes << suffix[i];
	return str.str();
}