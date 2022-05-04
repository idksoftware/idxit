//@@>>FileInfo.h
// =========== FileInfo.h ===========
//
#pragma once

#include <string>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include "CIDKDate.h"

/*
Media Pro
-<AssetProperties>
<Filename>P1160039.JPG</Filename>
<Filepath>D:\tmp\La Grave\P1160039.JPG</Filepath>
<UniqueID>1</UniqueID>
<Label>0</Label>
<Rating>0</Rating>
<MediaType>JPEG</MediaType>
<FileSize unit="Bytes">3240511</FileSize>
<Created>2017:03:09 17:02:59</Created>
<Modified>2017:01:16 09:18:16</Modified>
<Added>2017:03:19 22:41:36</Added>
<ThumbnailSource>Thumbnails_(Catalog2)\P1160039.jpg</ThumbnailSource>
*/

//namespace simplearchive {

	class FileInfo
	{
		std::filesystem::path m_path;
		uint64_t m_crc;
		std::string m_mediaType;
		std::string m_sha256;
		std::string m_uuid;
		

		void Init(std::string& path);
	public:
		FileInfo(std::string& path);
		~FileInfo();

		uint64_t getCrc() const {
			return m_crc;
		}

		uintmax_t getSize() const {
			return std::filesystem::file_size(m_path);
		}

		const std::string& getSha256() const {
			return m_sha256;
		}

		const std::string& getMediaType() const {
			return m_mediaType;
		}

		void setMediaType(const std::string& mediaType) {
			this->m_mediaType = mediaType;
		}

		const std::string& getUuid() const {
			return m_uuid;
		}

		void setUuid(const std::string& uuid) {
			this->m_uuid = uuid;
		}

		const std::string getName() const {
			return m_path.filename().string();
		}

		const std::filesystem::path& getPath() const {
			return m_path;
		}

		void setPath(std::string& path) {
			Init(path);
		}

		const std::string getExt() const {
			return m_path.extension().string();
		}

		const CIDKDate getCreateTime() const;
		const CIDKDate getModTime() const;

	};


//};