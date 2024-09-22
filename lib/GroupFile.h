#pragma once

#include <string>
#include <vector>
#include <memory>



	class GroupItem {
	public:
		GroupItem() = default;
		~GroupItem() = default;
		std::string m_ext;
		std::string m_mediaType;
		std::string m_discription;
	};

	class GroupFile {
		//bool compare(std::string c1, std::string c2);
		//void sorted();
		//bool insert(uint64_t idx, const char* imagePath, const char* name, uint64_t size,
		//	uint64_t crc, const char* sha, const char* uuid, int version, ExifDate& date, uint64_t dbIdx);

		std::vector<std::shared_ptr<GroupItem>> m_list;
	public:
		GroupFile() = default;


		virtual ~GroupFile() = default;

		bool read(const char* datafile);
		bool write(const char* datafile);

		std::shared_ptr<GroupItem> find(const char* ext) const;
		std::vector<std::shared_ptr<GroupItem>>& getList() { return m_list; };
	};
