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

#pragma once

#include <string>
#include <memory>
#include <vector>
#include "ExifDate.h"
#include "DBDefines.h"
#include "MetaType.h"

namespace simplearchive {

/*
 * @brief This class manages the CSV database files.
 *
 * The CSV Database if file based. as the database expands the records in
 * the files will also expand. At some point the file containing the records
 * will become un-manageably large.
 * This class splits the files in to smaller parts
 */

/*
	class TableIndexSchema : public MTTableSchema {
	public:
		TableIndexSchema(const char *tablename) : MTTableSchema(tablename) {
			add(MTSchema(MTType::Integer, DB_SEQUENCEID));// template
			add(MTSchema(MTType::Text, DB_SHORTIMAGEPATH));
			add(MTSchema(MTType::Text, DB_FILENAME));
			add(MTSchema(MTType::Integer, DB_FILESIZE));
			add(MTSchema(MTType::UInteger, DB_CRC));
			add(MTSchema(MTType::Text, DB_MD5));
			add(MTSchema(MTType::Text, DB_UUID));
			add(MTSchema(MTType::Integer, DB_VERSION));// template
			add(MTSchema(MTType::Date, DB_DATEADDED));			
			add(MTSchema(MTType::Integer, DB_DATABASEID));
			
		}
	};

	class PrimaryIndexSchema : public TableIndexSchema {
	public:
		PrimaryIndexSchema() : TableIndexSchema(TABLE_PRIMARY_INDEX) {};
	};
	
	class DerivativeIndexSchema : public TableIndexSchema {
	public:
		DerivativeIndexSchema() : TableIndexSchema(TABLE_DERIVATIVE_INDEX) {};
	};

	class MasterIndexSchema : public TableIndexSchema {
	public:
		MasterIndexSchema() : TableIndexSchema(TABLE_MASTER_INDEX) {};
	};

	class IndexRow : public MTRow {
		
	public:
		IndexRow(MTTableSchema &tableSchema);
		IndexRow(int idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
			const char *md5, const char *uuid, int version, ExifDate &date, int m_dbidx, MTTableSchema &tableSchema);
		IndexRow(const MTRow &row, MTTableSchema &tableSchema) : MTRow(tableSchema) {

			for (unsigned int i = 0; i < row.size(); i++) {
				MTColumn& c1 = *at(i);
				MTColumn& c2 = *row.at(i);
				c1 = c2;
			}
		}
	};

	
	class PrimaryIndexRow : public IndexRow {
		static PrimaryIndexSchema m_tableSchema;
	public:
		PrimaryIndexRow();
		PrimaryIndexRow(int idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
			const char *md5, const char *uuid, int version, ExifDate &date, int m_dbidx);
		PrimaryIndexRow(const MTRow &row);

	};

	
	class DerivativeIndexRow : public IndexRow {
		static DerivativeIndexSchema m_tableSchema;
	public:
		DerivativeIndexRow();
		DerivativeIndexRow(int idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
			const char *md5, const char *uuid, int version, ExifDate &date, int m_dbidx);
		DerivativeIndexRow(const MTRow &row);
	};
	
	class MasterIndexRow : public IndexRow {
		static MasterIndexSchema m_tableSchema;
	public:
		MasterIndexRow();
		MasterIndexRow(int idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
			const char *md5, const char *uuid, int version, ExifDate &date, int m_dbidx);
		MasterIndexRow(const MTRow &row);

			
	};
*/	

class ImageInfo {
	uint64_t m_idx;
	std::string m_imagePath;
	std::string m_shortImagePath;
	std::string m_name;
	uint64_t m_crc;
	std::string m_sha;
	std::string m_uuid;
	uint64_t m_size;
	int m_version;
	ExifDate m_dateArchived;
	uint64_t m_dbidx;
public:



	ImageInfo(uint64_t idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
		const char *sha, const char *uuid, int version, ExifDate &date, uint64_t dbidx);
	~ImageInfo();
	uint64_t getCrc() const;
	const ExifDate& getDate() const;
	uint64_t getIdx() const;
	const std::string& getImagePath() const;
	const std::string& getShortImagePath() const;
	const std::string& getName() const;
	const std::string& getSha() const;
	uint64_t getSize() const;
	const std::string& getUuid() const;
	int getVersion() const;
	uint64_t getDBIdx() const;
};


//class MirrorIdxDB;
class CSVDBFile {
	std::string m_dbpath;
//	std::unique_ptr<MirrorIdxDB> m_mirrorIdxDB;
	uint16_t m_data[4];
	uint64_t getMaxDirIndex(std::string &path);
	bool insert(uint64_t idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
		const char *sha, const char *uuid, int version, ExifDate &date, const char*rootPath, uint64_t dbidx);
	CSVDBFile(const CSVDBFile &) = delete;
	
public:
	CSVDBFile();

	virtual ~CSVDBFile();

	bool insert(uint64_t idx, const char *imagePath, const char *name, uint64_t size, uint64_t crc,
						const char *sha, const char *uuid, int version, ExifDate &date, uint64_t dbidx = -1);

	void setPath(const char *s);
	bool backupPaths(const char *path);
	const char* findPath(uint64_t idx);
	uint64_t findSize(uint64_t idx);
	uint64_t getNextIndex();
	uint64_t getNextIndex(uint64_t current);
	uint64_t getMaxIndex();
	std::unique_ptr<ImageInfo> getItemAt(uint64_t idx);
	static std::string getYear(const char *path);
};

/*
class PrimaryIndexTable : public CSVDBFile {
	static PrimaryIndexRow m_indexRow;
public:
	PrimaryIndexTable() : CSVDBFile(m_indexRow) {};
};

class MasterIndexTable : public CSVDBFile {
	static MasterIndexRow m_indexRow;
public:
	MasterIndexTable() : CSVDBFile(m_indexRow) {};
};

class DerivativeIndexTable : public CSVDBFile {
	static DerivativeIndexRow m_indexRow;
public:
	DerivativeIndexTable() : CSVDBFile(m_indexRow) {};
};
*/

} /* namespace simplearchive */

