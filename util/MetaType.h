
//@@>>MetaType.h
// =========== MetaType.h ===========
//
/*
 * MetaType.h
 *
 *  Created on: May 5, 2015
 *      Author: wzw7yn
 */

#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <stdexcept>
#include <map>
#include "ExifDateTime.h"
#include "CSVArgs.h"
#include "MetaType.h"
//#include "ErrorCode.h"

#define DELIM ','



	class MTTypeException : public std::exception {
		std::string message;

	public:
		explicit MTTypeException(const char* msg) : message(msg) {

		}
		explicit MTTypeException(std::string msg) : message(msg) {}
		virtual ~MTTypeException() throw() {}
		virtual const char* what() const throw();

	};

	enum class MTType {
		Text,
		Date,
		Float,
		Integer,
		UInteger,
		Unknown
	};

	class MTSchema {
		MTType m_type;
		std::string m_name;
		size_t m_size;
		bool m_primaryKey;
	public:

		MTSchema(MTType type, const char* name, size_t size) {
			m_type = type;
			m_name = name;
			m_size = size; // used when converted to a SQL database
			m_primaryKey = false;
		}

		MTSchema(MTType type, const char* name) {
			m_type = type;
			m_name = name;
			m_size = 0; // used when converted to a SQL database
			m_primaryKey = false;
		}

		MTSchema(MTType type, const char* name, bool primaryKey) {
			m_type = type;
			m_name = name;
			m_size = 0; // used when converted to a SQL database
			m_primaryKey = primaryKey;
		}

		MTSchema(MTType type, std::string& name, size_t size = 0) {
			m_type = type;
			m_name = name;
			m_primaryKey = false;
		}

		MTSchema(const MTSchema& c) {
			m_type = c.m_type;
			m_name = c.m_name;
			m_size = c.m_size;
			m_primaryKey = c.m_primaryKey;
		}

		void setPrimaryKey() {
			m_primaryKey = true;
		}

		const std::string& getName() const {
			return m_name;
		}

		MTType getType() const {
			return m_type;
		}

		const char* getTypeString();

		size_t getSize();
	};

	class MTTableSchema : public std::vector<MTSchema> {
		std::map<std::string, int> m_index;
		int m_count;
	protected:
		std::string m_tablename;
	public:
		MTTableSchema(const char* name) {
			m_tablename = name;
			m_count = 0;
		}
		MTTableSchema(MTTableSchema& ts) {
			m_tablename = ts.m_tablename;
			m_count = ts.m_count;
			int count = 0;
			for (auto i = ts.begin(); i != ts.end(); i++) {
				MTSchema& item = *i;
				push_back(item);
				m_index.insert(std::make_pair(item.getName(), count++));
			}

		}
		virtual ~MTTableSchema() {
			m_index.clear();
			clear();
		}

		void add(MTSchema item) {
			push_back(MTSchema(item));
			m_index.insert(std::make_pair(item.getName(), m_count));
			m_count++;
		}
		size_t getIndex(const char* name) const {

			if (m_index.find(name) == m_index.end()) {
				return std::string::npos;
			}
			else {
				return m_index.at(name);
			}

		}
		void print() {
			//m_name = name;
			printf("%s\n", getName().c_str());
			for (std::vector<MTSchema>::iterator i = begin(); i != end(); i++) {
				MTSchema& columnInfo = *i;
				printf("%s      %s\n", columnInfo.getName().c_str(), columnInfo.getTypeString());
			}
		}

		const std::string& getName() const {
			return m_tablename;
		}

		const std::string& getColumnName(size_t i) const {
			return at(i).getName();
		}

		const MTSchema& getColumnSchema(size_t i) const {
			return at(i);
		}

		const size_t size() const {
			return std::vector<MTSchema>::size();
		}

		std::vector<MTSchema>::iterator begin() {
			return std::vector<MTSchema>::begin();
		}

		std::vector<MTSchema>::iterator end() {
			return std::vector<MTSchema>::end();
		}
	};



	class MetaTypeObject;
	class MTColumn {
		static const char* nullStr;
		std::shared_ptr<MTSchema> m_info;
		std::shared_ptr<MetaTypeObject> m_object;
		std::shared_ptr<std::string> m_boundValue;
		std::unique_ptr<std::string> m_unboundValue;
		MTColumn() : m_info(nullptr) {
			//m_info = nullptr;
			m_object = nullptr;
			m_boundValue = nullptr;
		};
		//	void boundUpdate();
		bool isBound() {
			return (m_boundValue != nullptr);
		}
	public:

		MTColumn(const MTSchema& info);
		virtual ~MTColumn();
		MTColumn(const MTColumn& r);
		MTColumn& operator=(const MTColumn& r);

		bool isNull() {
			if (m_object != nullptr) {
				if (m_info->getType() == MTType::Text) {

				}
				return false;
			}

			return true;
		}
		std::string& toString();
		bool fromString(const char* str);
		bool fromString(const std::string& str);
		std::string token(size_t* pos, std::string& str);
		void set(int i);
		void set(int64_t i);
		void set(unsigned int i);
		void set(uint64_t i);
		void set(const char* str);
		void set(const std::string& str);
		void set(const ExifDateTime& date);
		void set(double d);
		void set(float f);
		void set(MTColumn& c);

		MTSchema& getInfo() {
			return *m_info;
		}

		void operator=(const bool i) {
			set((int64_t)i);
		}

		void operator=(const char i) {
			set((int64_t)i);
		}

		void operator=(const unsigned char i) {
			set((uint64_t)i);
		}

		void operator=(const short i) {
			set((int64_t)i);
		}

		void operator=(const unsigned short i) {
			set((uint64_t)i);
		}

		void operator=(const int i) {
			set((int64_t)i);
		}
		
		void operator=(const unsigned int i) {
			set((uint64_t)i);
		}

		void operator=(const long i) {
			set((int64_t)i);
		}

		void operator=(const unsigned long i) {
			set((uint64_t)i);
		}

		void operator=(const int64_t i) {
			set((int64_t)i);
		}

		void operator=(const uint64_t i) {
			set((uint64_t)i);
		}

		void operator=(const char* str) {
			set(str);
		}
		void operator=(std::string& str) {
			if (m_info->getType() == MTType::Date) {
				set(str);
			}
			set(str);
		}
		void operator=(const ExifDateTime& date) {
			set(date);
		}
		void operator=(const double d) {
			set(d);
		}
		void operator=(const float f) {
			set(f);
		}

		std::shared_ptr<MTSchema> getMTSchemaItem() {
			return m_info;
		}

		
		const int64_t getInt();
		const uint64_t getUInt();
		const std::string& getString();
		const ExifDateTime& getDate();
		const double getDouble();
		const float getFloat();

		inline MTColumn& operator<<(unsigned char& val);
		inline MTColumn& operator>>(unsigned char& val);
		inline friend void operator>>(unsigned char& val, MTColumn& col);
		inline friend void operator<<(unsigned char& val, MTColumn& col);

		inline MTColumn& operator<<(const unsigned char& val);
		inline friend void operator>>(const unsigned char& val, MTColumn& col);

		inline MTColumn& operator<<(unsigned short& val);
		inline MTColumn& operator>>(unsigned short& val);
		inline friend void operator>>(unsigned short& val, MTColumn& col);
		inline friend void operator<<(unsigned short& val, MTColumn& col);

		inline MTColumn& operator<<(const unsigned short& val);
		inline friend void operator>>(const unsigned short& val, MTColumn& col);

		inline MTColumn& operator<<(unsigned int& val);
		inline MTColumn& operator>>(unsigned int& val);
		inline friend void operator>>(unsigned int& val, MTColumn& col);
		inline friend void operator<<(unsigned int& val, MTColumn& col);

		inline MTColumn& operator<<(const unsigned int& val);
		inline friend void operator>>(const unsigned int& val, MTColumn& col);
		
		inline MTColumn& operator<<(unsigned long& val);
		inline MTColumn& operator>>(unsigned long& val);
		inline friend void operator>>(unsigned long& val, MTColumn& col);
		inline friend void operator<<(unsigned long& val, MTColumn& col);

		inline MTColumn& operator<<(const unsigned long& val);
		inline friend void operator>>(const unsigned long& val, MTColumn& col);

		inline MTColumn& operator<<(uint64_t& val);
		inline MTColumn& operator>>(uint64_t& val);
		inline friend void operator>>(uint64_t& val, MTColumn& col);
		inline friend void operator<<(uint64_t& val, MTColumn& col);

		inline MTColumn& operator<<(char& val);
		inline MTColumn& operator>>(char& val);
		inline friend void operator>>(char& val, MTColumn& col);
		inline friend void operator<<(char& val, MTColumn& col);

		inline MTColumn& operator<<(const char& val);
		inline friend void operator>>(const char& val, MTColumn& col);

		inline MTColumn& operator<<(short& val);
		inline MTColumn& operator>>(short& val);
		inline friend void operator>>(short& val, MTColumn& col);
		inline friend void operator<<(short& val, MTColumn& col);

		inline MTColumn& operator<<(const short& val);
		inline friend void operator>>(const short& val, MTColumn& col);

		inline MTColumn& operator<<(int& val);
		inline MTColumn& operator>>(int& val);
		inline friend void operator>>(int& val, MTColumn& col);
		inline friend void operator<<(int& val, MTColumn& col);

		inline MTColumn& operator<<(const int& val);
		inline friend void operator>>(const int& val, MTColumn& col);

		inline MTColumn& operator<<(long& val);
		inline MTColumn& operator>>(long& val);
		inline friend void operator>>(long& val, MTColumn& col);
		inline friend void operator<<(long& val, MTColumn& col);

		inline MTColumn& operator<<(const long& val);
		inline friend void operator>>(const long& val, MTColumn& col);



		inline MTColumn& operator<<(int64_t& val);
		inline MTColumn& operator>>(int64_t& val);
		inline friend void operator>>(int64_t& val, MTColumn& col);
		inline friend void operator<<(int64_t& val, MTColumn& col);

		inline MTColumn& operator<<(std::string& val);
		inline MTColumn& operator>>(std::string& val);
		inline friend void operator>>(std::string& val, MTColumn& col);
		inline friend void operator<<(std::string& val, MTColumn& col);

		inline MTColumn& operator<<(ExifDateTime& val);
		inline MTColumn& operator>>(ExifDateTime& val);
		inline friend void operator>>(ExifDateTime& val, MTColumn& col);
		inline friend void operator<<(ExifDateTime& val, MTColumn& col);

		inline MTColumn& operator<<(double& val);
		inline MTColumn& operator>>(double& val);
		inline friend void operator>>(double& val, MTColumn& col);
		inline friend void operator<<(double& val, MTColumn& col);

		inline MTColumn& operator<<(float& val);
		inline MTColumn& operator>>(float& val);
		inline friend void operator>>(float& val, MTColumn& col);
		inline friend void operator<<(float& val, MTColumn& col);


		void bind(std::string& str) {
			m_boundValue = std::make_shared<std::string>(str);
		}
	};


	using SharedMTColumn = std::shared_ptr<MTColumn>;

	class MTRow : public std::vector<SharedMTColumn> {
		MTTableSchema& m_schema;
		//std::string m_text;
		char m_delim;
	protected:
		//MTTableSchema& getSchema() {
		//	return *m_schema;
		//}
		friend class MTTable;

	public:

		MTRow(MTTableSchema& ts);
		MTRow(MTTableSchema& ts, char delim);
		MTRow(const MTRow& row);
		MTRow& operator=(const MTRow& row);
		virtual ~MTRow();

		const std::string& getName() const {
			return m_schema.getName();
		}

		MTColumn& columnAt(size_t i) const {
			return *(at(i));
		}

		MTColumn& columnAt(const char* name) const {

			try {
				size_t idx = m_schema.getIndex(name);
				if (idx == std::string::npos) {
					throw new std::exception;
				}
				return *(at(idx));
			}
			catch (std::exception /*&e */) {
				std::string err = "Column name invalid: ";
				err += name;
				throw std::invalid_argument(err.c_str());
			}
		}
		void print() {
			std::string text = toString();
			std::cout << text << '\n';
		}

		/*
		for (std::vector<MTSchema>::iterator i = begin(); i != end(); i++) {
		MTSchema& columnInfo = *i;
		printf("%s      %s\n", columnInfo.getName().c_str(), columnInfo.getTypeString());
		}
		*/
		void debugPrint() {
			for (auto i = this->begin(); i != this->end(); i++) {
				SharedMTColumn column = *i;
				std::cout << column->toString() << "\n";
			}
		}

		std::string toDebugString() {
			std::string text;

			for (auto i = this->begin(); i != this->end(); i++) {
				SharedMTColumn column = *i;
				std::shared_ptr<MTSchema> mtSchema = column->getMTSchemaItem();
				text += mtSchema->getName();
				if (mtSchema == nullptr) {
					continue;
				}
				text += ',';
				text += column->toString();
				text += '\n';
			}
			return text;
		}

		std::string escapeString(std::string& s) {
			if (s.find_first_of(m_delim) == std::string::npos) {
				return s;
			}
			return ('\"' + s + '\"');
		}

		std::string toString() {
			std::string text;
			bool first = true;
			for (auto i = this->begin(); i != this->end(); i++) {
				SharedMTColumn column = *i;
				std::shared_ptr<MTSchema> mtSchema = column->getMTSchemaItem();
				std::string tmp;
				if (mtSchema == nullptr) {
					continue;
				}

				if (first) {
					first = false;

					if (mtSchema->getType() == MTType::Date) {
						if (column->isNull()) {
							//text = "0.0.0.0.0.0";
							//printf("%s\n", text.c_str());
						}
					}
					else {
						//text = escapeString(column->toString());
					}
					tmp = escapeString(column->toString());
					text = tmp;
				}
				else {
					text += m_delim;

					if (mtSchema->getType() == MTType::Date) {
						if (column->isNull()) {
							//text += "0.0.0.0.0.0";
							//printf("%s\n", text.c_str());
						}
					}
					else {
						//text += escapeString(column->toString());
					}
					tmp = escapeString(column->toString());
					text += tmp;
					//text += escapeString(column->toString());
				}
			}
			return text;
		}

		bool fromString(const char* s) {
			std::string str = s;
			return fromString(str);
		}

		bool fromString(const std::string& str) {
			CSVArgs csvArgs(m_delim);
			csvArgs.process(str);

			if (m_schema.size() != csvArgs.size() && m_schema.size() < (csvArgs.size() - 1)) {
				// Note if the last item is null then csvArgs will be one less then m_schema size
				//ErrorCode::setErrorCode(IMGA_ERROR::ROW_SCHEMA_MISMATCH);
				return false;
			}
			std::vector<std::string>::iterator arg = csvArgs.begin();
			for (auto i = this->begin(); i != this->end(); i++, arg++) {
				if (arg == csvArgs.end()) {
					break;
				}
				std::string argStr = *arg;
				SharedMTColumn column = *i;
				column->fromString(argStr.c_str());
			}
			return true;
		}

		//bool join(MTRow& row);

		bool join(const MTRow& row);

		const MTTableSchema& getSchema() const {
			return m_schema;
		}
	};

	using SharedMTRow = std::shared_ptr<MTRow>;
	//using ResultsList = std::vector<SharedMTRow>;

	class MTTable : public std::vector<SharedMTRow> {

		std::shared_ptr<MTTableSchema> m_TableSchema;
		int m_rowCursor{ NOT_FOUND };
	public:
		const int NOT_FOUND = -1;
		MTTable(MTTableSchema* pSchemaTable)
			: m_TableSchema(std::make_shared<MTTableSchema>(*pSchemaTable)),
			m_rowCursor(NOT_FOUND) {};

		MTTable(std::shared_ptr<MTTableSchema> pSchemaTable)
			: m_TableSchema(pSchemaTable),
			m_rowCursor(NOT_FOUND) {};

		virtual ~MTTable() {};
		MTTableSchema& getSchema() const {
			return *m_TableSchema;
		}

		std::shared_ptr<MTTableSchema> getSchemaPtr() const {
			return m_TableSchema;
		}

		SharedMTRow makeRow();
		bool addRow(const MTRow& r);
		bool fromString(const std::string& r);
		/// reads a csv GPSProperties file
		bool read(const char* path, const char* filename);
		bool read(const char* path);


		/// writes a csv GPSProperties file
		bool write(const char* path, const char* filename);
		bool write(const char* path);
		bool truncate() {
			clear();
			return true;
		}

		SharedMTRow SetRowsetCursorPosition(int pos) {
			m_rowCursor = pos;
			return at(m_rowCursor);
		}

		const MTSchema& getSchema(int pos) {
			const MTSchema& s = m_TableSchema->getColumnSchema(pos);
			return s;
		}
		/// create a column that has the column name set and the value to be matched.
		/// If true the m_rowCursor will be at the matched row.
		bool find(MTColumn& column);

		SharedMTRow getCurrentRow() {
			if (m_rowCursor == NOT_FOUND) {
				return nullptr;
			}
			std::shared_ptr<MTRow> srow = at(m_rowCursor);
			return srow;
		}

		void print();
	};


	class MTDatabase : public std::map<std::string, std::unique_ptr<MTTable>> {

	public:
		MTDatabase() {
		};
		virtual ~MTDatabase() {};
		bool addTable(MTTableSchema* pSchemaTable);

		MTTable& getTable(std::string name);
	};

	// int
	inline MTColumn& MTColumn::operator<<(int64_t& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(int64_t& val) {
		val = getInt();
		return *this;
	}

	inline void operator>>(int64_t& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(int64_t& val, MTColumn& col) {
		val = col.getInt();
	}

	// -- uint --
	// unsigned char
	inline MTColumn& MTColumn::operator<<(unsigned char& val) {
		set((uint64_t)val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(unsigned char& val) {
		val = (char)getUInt();
		return *this;
	}
	inline void operator>>(unsigned char& val, MTColumn& col) {
		col.set((unsigned char)val);
	}

	inline void operator<<(unsigned char& val, MTColumn& col) {
		val = (char)col.getUInt();
	}

	// const unsigned char
	inline MTColumn& MTColumn::operator<<(const unsigned char& val) {
		set((uint64_t)val);
		return *this;
	}

	inline void operator>>(const unsigned char& val, MTColumn& col) {
		col.set((uint64_t)val);
	}
	// unsigned short
	inline MTColumn& MTColumn::operator<<(unsigned short& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(unsigned short& val) {
		val = (unsigned short)getUInt();
		return *this;
	}
	inline void operator>>(unsigned short& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(unsigned short& val, MTColumn& col) {
		val = (unsigned short)col.getUInt();
	}

	// const unsigned short
	inline MTColumn& MTColumn::operator<<(const unsigned short& val) {
		set(val);
		return *this;
	}

	inline void operator>>(const unsigned short& val, MTColumn& col) {
		col.set(val);
	}
	// unsigned int
	inline MTColumn& MTColumn::operator<<(unsigned int& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(unsigned int& val) {
		val = (unsigned int)getUInt();
		return *this;
	}
	inline void operator>>(unsigned int& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(unsigned int& val, MTColumn& col) {
		val = (unsigned int)col.getUInt();
	}

	// const unsigned int
	inline MTColumn& MTColumn::operator<<(const unsigned int& val) {
		set(val);
		return *this;
	}

	inline void operator>>(const unsigned int& val, MTColumn& col) {
		col.set(val);
	}
	// unsigned long
	inline MTColumn& MTColumn::operator<<(unsigned long& val) {
		set((uint64_t)val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(unsigned long& val) {
		val = (unsigned long)getUInt();
		return *this;
	}
	inline void operator>>(unsigned long& val, MTColumn& col) {
		col.set((uint64_t)val);
	}

	inline void operator<<(unsigned long& val, MTColumn& col) {
		val = (unsigned long)col.getUInt();
	}

	// const unsigned long
	inline MTColumn& MTColumn::operator<<(const unsigned long& val) {
		set((uint64_t)val);
		return *this;
	}

	inline void operator>>(const unsigned long& val, MTColumn& col) {
		col.set((uint64_t)val);
	}

	inline MTColumn& MTColumn::operator<<(uint64_t& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(uint64_t& val) {
		val = getUInt();
		return *this;
	}

	inline void operator>>(uint64_t& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(uint64_t& val, MTColumn& col) {
		val = col.getUInt();
	}

	// -- int --
	// char
	inline MTColumn& MTColumn::operator<<(char& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(char& val) {
		val = (char)getInt();
		return *this;
	}
	inline void operator>>(char& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(char& val, MTColumn& col) {
		val = (char)col.getInt();
	}

	// const char
	inline MTColumn& MTColumn::operator<<(const char& val) {
		set(val);
		return *this;
	}

	inline void operator>>(const char& val, MTColumn& col) {
		col.set(val);
	}
	// short
	inline MTColumn& MTColumn::operator<<(short& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(short& val) {
		val = (short)getInt();
		return *this;
	}
	inline void operator>>(short& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(short& val, MTColumn& col) {
		val = (short)col.getInt();
	}

	// const short
	inline MTColumn& MTColumn::operator<<(const short& val) {
		set(val);
		return *this;
	}

	inline void operator>>(const short& val, MTColumn& col) {
		col.set(val);
	}
	// int
	inline MTColumn& MTColumn::operator<<(int& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(int& val) {
		val = (int)getInt();
		return *this;
	}
	inline void operator>>(int& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(int& val, MTColumn& col) {
		val = (int)col.getInt();
	}

	// const int
	inline MTColumn& MTColumn::operator<<(const int& val) {
		set(val);
		return *this;
	}

	inline void operator>>(const int& val, MTColumn& col) {
		col.set(val);
	}
	// long
	inline MTColumn& MTColumn::operator<<(long& val) {
		set((int64_t)val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(long& val) {
		val = (long)getInt();
		return *this;
	}
	inline void operator>>(long& val, MTColumn& col) {
		col.set((int64_t)val);
	}

	inline void operator<<(long& val, MTColumn& col) {
		val = (long)col.getInt();
	}

	// const long
	inline MTColumn& MTColumn::operator<<(const long& val) {
		set((int64_t)val);
		return *this;
	}

	inline void operator>>(const long& val, MTColumn& col) {
		col.set((int64_t)val);
	}


	// std::string
	inline MTColumn& MTColumn::operator<<(std::string& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(std::string& val) {
		val = getString();
		return *this;
	}

	inline void operator>>(std::string& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(std::string& val, MTColumn& col) {
		val = col.getString();
	}

	// CDate
	inline MTColumn& MTColumn::operator<<(ExifDateTime& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(ExifDateTime& val) {
		val = getDate();
		return *this;
	}

	inline void operator>>(ExifDateTime& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(ExifDateTime& val, MTColumn& col) {
		val = col.getDate();
	}

	// int
	

	

	

	

	// double
	inline MTColumn& MTColumn::operator<<(double& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(double& val) {
		val = getDouble();
		return *this;
	}

	inline void operator>>(double& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(double& val, MTColumn& col) {
		val = col.getDouble();
	}

	// float
	inline MTColumn& MTColumn::operator<<(float& val) {
		set(val);
		return *this;
	}

	inline MTColumn& MTColumn::operator>>(float& val) {
		val = getFloat();
		return *this;
	}

	inline void operator>>(float& val, MTColumn& col) {
		col.set(val);
	}

	inline void operator<<(float& val, MTColumn& col) {
		val = col.getFloat();
	}

