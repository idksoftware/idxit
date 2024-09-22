//@@>>FileInfo.cpp
// =========== FileInfo.cpp ===========
//
#include "FileInfo.h"

#include "crc64.h"
#include "sha256.h"
#include "UniqueId.h"
#include <string>
#include <fstream>
#include <iostream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#include <random>
#include <sstream>

//

/*
	namespace uuid {

#ifdef CPPSTD20
		template<class T = std::mt19937, std::size_t N = T::state_size * sizeof(typename T::result_type)>
		T ProperlySeededRandomEngine() {
			std::random_device source;
			auto random_data = std::views::iota(std::size_t(), (N - 1) / sizeof(source()) + 1)
				| std::views::transform([&](auto) { return source(); });
			std::seed_seq seeds(std::begin(random_data), std::end(random_data));
			return T(seeds);
		}
#else
		template<class T = std::mt19937, std::size_t N = T::state_size * sizeof(typename T::result_type)>
		auto ProperlySeededRandomEngine() -> typename std::enable_if<N, T>::type {
			std::random_device source;
			std::random_device::result_type random_data[(N - 1) / sizeof(source()) + 1];
			std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
			std::seed_seq seeds(std::begin(random_data), std::end(random_data));
			return T(seeds);
		}
#endif
		//static std::random_device              rd;
		static std::mt19937                    gen(ProperlySeededRandomEngine());
		static std::uniform_int_distribution<> dis(0, 15);
		static std::uniform_int_distribution<> dis2(8, 11);

		std::string generate_uuid_v4() {
			std::stringstream ss;
			int i;
			ss << std::hex;
			for (i = 0; i < 8; i++) {
				ss << dis(gen);
			}
			ss << "-";
			for (i = 0; i < 4; i++) {
				ss << dis(gen);
			}
			ss << "-";
			for (i = 0; i < 3; i++) {
				ss << dis(gen);
			}
			ss << "-";
			ss << dis2(gen);
			for (i = 0; i < 3; i++) {
				ss << dis(gen);
			}
			ss << "-";
			for (i = 0; i < 12; i++) {
				ss << dis(gen);
			};
			return ss.str();
		}
	}
*/
	

	//#include "AppConfig.h"
	template <typename TP>
	std::time_t to_time_t(TP tp)
	{
		using namespace std::chrono;
		auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
			+ system_clock::now());
		return system_clock::to_time_t(sctp);
	}



	static std::string splitName(std::string pathStr) {
		size_t pos = pathStr.find_last_of("/");
		if (pos == std::string::npos) {
			pos = pathStr.find_last_of("\\");
			if (pos == std::string::npos) {
				return pathStr;
			}
		}
		std::string filename = pathStr.substr(pos + 1, pathStr.length() - (pos + 1));
		return filename;
	}

	static std::string getFileContents(const char* filename)
	{
		try {
			int count = 0;
			std::ifstream in(filename, std::ios::in | std::ios::binary);
			if (in)
			{
				std::string contents;
				in.seekg(0, std::ios::end);
				contents.resize((std::string::size_type)in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&contents[0], contents.size());
				in.close();
				return(contents);
			}
			throw std::exception("Error: Unable to read contents");
		}
		catch (std::exception /*&e*/) {
			throw std::exception();
		}
		std::string error;
		return error;
	}

	bool FileInfo::m_simple = false;

	FileInfo::FileInfo(std::string& path)
	{
		m_path = path;
		if (m_simple == false) {
			Init(path);
		}
	}

	/*
	const CIDKDate FolderInfo::getCreateTime() const {
		std::filesystem::file_time_type time = std::filesystem::last_write_time(m_path);
		time_t timet = to_time_t(time);
		CIDKDate createTime(timet);

		return createTime;
	}
	*/

	const CIDKDate FileInfo::getCreateTime() const {
		std::filesystem::file_time_type time = std::filesystem::last_write_time(m_path);
		time_t timet = to_time_t(time);
		CIDKDate createTime(timet);
		
		return createTime;
	}

	const CIDKDate FileInfo::getModTime() const {
		std::filesystem::file_time_type time = std::filesystem::last_write_time(m_path);
		time_t timet = to_time_t(time);
		CIDKDate modTime(timet);
		return modTime;
	}

	void FileInfo::Init(std::string& path)
	{
		std::string buf;
		try {
			buf = getFileContents(path.c_str());
		}
		catch (std::exception /*&e*/) {
			throw std::exception();
		}
		SHA256 sha256;
		sha256.update(buf);
		uint8_t* digest = sha256.digest();
		m_sha256 = sha256.toString(digest);
		delete[] digest;
		UniqueId uniqueId;
		m_uuid = uniqueId.get();
		CRC64 crc64;
		m_crc = crc64.crc64(0, (unsigned char*)buf.c_str(), buf.size());
		//printf("e9c6d914c4b8d9ca == %016llx\n",
		//    (uint64_t) crc64.crc64(0, (unsigned char*)"1234567890w", 11));
	}



	FileInfo::~FileInfo()
	{
	}



//};
