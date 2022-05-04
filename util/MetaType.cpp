//@@>>MetaType.cpp
// =========== MetaType.cpp ===========
//

/*
 * MetaType.cpp
 *
 *  Created on: May 5, 2015
 *      Author: wzw7yn
 */
#include <fstream>
#include <sstream>
#include "MetaType.h"

//#include "siaglobal.h"
#ifdef _WIN32
#ifdef _DEBUG
 //#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

namespace simplearchive {

	const char* MTColumn::nullStr = "";


	class MetaTypeObject {
		
		MTType m_type;
		bool m_isSet;
		MetaTypeObject() {
			m_type = MTType::Unknown;

		};
		typedef union {
			double m_double;
			std::string* m_string;
			uint64_t m_int;
			ExifDateTime* m_date;
		} TypeObject;
		TypeObject m_typeObject;
	public:

		MetaTypeObject(double d) {
			m_typeObject.m_double = d;
			m_type = MTType::Float;
			m_isSet = true;
		}
		MetaTypeObject(const char* s) {
			m_typeObject.m_string = new std::string(s);
			m_type = MTType::Text;
			m_isSet = true;
		}

		MetaTypeObject(int64_t l) {
			m_typeObject.m_int = l;
			m_type = MTType::Integer;
			m_isSet = true;
		}

		MetaTypeObject(uint64_t l) {
			m_typeObject.m_int = l;
			m_type = MTType::UInteger;
			m_isSet = true;
		}

		MetaTypeObject(const ExifDateTime& date) {
			m_typeObject.m_date = new ExifDateTime(date);
			m_type = MTType::Date;
			m_isSet = true;
		}
		MetaTypeObject& operator=(const ExifDateTime& date) {
			delete m_typeObject.m_date;
			m_typeObject.m_date = new ExifDateTime(date);
			m_type = MTType::Date;
			m_isSet = true;
			return *this;
		}
		virtual ~MetaTypeObject() {
			switch (m_type) {
			case MTType::Integer: break;
			case MTType::UInteger: break;
			case MTType::Text:
				delete m_typeObject.m_string;
				break;
			case MTType::Date:
				delete m_typeObject.m_date;
				break;
			case MTType::Float:
				break;
			case MTType::Unknown:
				break;
			}
		}
		void set(int64_t l) {
			if (m_type != MTType::Integer) {
				throw MTTypeException("Not integer type");
			}
			m_typeObject.m_int = l;
		}

		void set(uint64_t l) {
			if (m_type != MTType::UInteger) {
				throw MTTypeException("Not Uinteger type");
			}
			m_typeObject.m_int = l;
		}

		void operator=(uint64_t l) {
			set(l);
		}

		void operator=(int64_t l) {
			set(l);
		}

		void set(const std::string& s) {
			if (m_type != MTType::Text) {
				throw MTTypeException("Not a text type");
			}
			*(m_typeObject.m_string) = s;
		}

		void operator=(std::string& s) {
			set(s);
		}

		void operator=(const char* s) {
			set(s);
		}

		void set(ExifDateTime& d) {
			if (m_type != MTType::Date) {
				throw MTTypeException("Not a date type");
			}
			*(m_typeObject.m_date) = d;
		}

		void operator=(ExifDateTime& d) {
			set(d);
		}
		void set(double d) {
			if (m_type != MTType::Float) {
				throw MTTypeException("Not a float type");
			}
			m_typeObject.m_double = d;
		}

		void operator=(double d) {
			set(d);
		}

		const ExifDateTime& getDate() {
			return *(m_typeObject.m_date);
		}

		double getDouble() const {
			return m_typeObject.m_double;
		}

		int64_t getInt() const {
			return m_typeObject.m_int;
		}

		uint64_t getUInt() const {
			return m_typeObject.m_int;
		}

		const std::string& getString() const {
			return *(m_typeObject.m_string);
		}


	};

	const char* MTSchema::getTypeString() {
		switch (m_type) {
		case MTType::Integer: return "integer";
		case MTType::Text: return "text";
		case MTType::Date: return "date";
		case MTType::Float: return "float";
		}
		return "unknown";
	}

	size_t MTSchema::getSize()
	{
		return m_size;
	}

	const char* MTTypeException::what() const throw() {
		//std::stringstream s;
		//s << message << " line:" << __LINE__ << " file:" << __FILE__;
		//std::string tmp = s.str();
		//output = tmp.c_str();
		return message.c_str();
	}


	MTColumn::MTColumn(const MTSchema& info) : m_info(std::make_shared<MTSchema>(info)) {
	}

	MTColumn::MTColumn(const MTColumn& r) {
		m_object = nullptr;
		m_boundValue = nullptr;
		MTType type = m_info->getType();
		if (r.m_object == nullptr) {
			m_object = nullptr;
		}
		switch (type) {
		case MTType::Integer:
			set(r.m_object->getInt());
			break;
		case MTType::UInteger:
			set(r.m_object->getUInt());
			break;
		case MTType::Text:
			set(r.m_object->getString());
			break;
		case MTType::Date:
			set(r.m_object->getDate());
			break;
		case  MTType::Float:
			set(r.m_object->getDouble());
			break;
		}
		return;
	}

	MTColumn& MTColumn::operator=(const MTColumn& r) {
		MTType type = m_info->getType();
		if (r.m_object == nullptr) {
			m_object = nullptr;
			return *this;
		}
		switch (type) {
		case MTType::Integer:
			set(r.m_object->getInt());
			break;
		case MTType::UInteger:
			set(r.m_object->getUInt());
			break;
		case MTType::Text:
			set(r.m_object->getString());
			break;
		case MTType::Date:
			set(r.m_object->getDate());
			break;
		case  MTType::Float:
			set(r.m_object->getDouble());
			break;
		}
		return *this;
	}



	MTColumn::~MTColumn() {
		if (m_object != nullptr) {
			//delete m_object;
		}
	}
	std::string& MTColumn::toString() {

		std::string* returned = nullptr;
		if (isBound()) {
			returned = m_boundValue.get();
		}
		else {
			m_unboundValue.reset(new std::string);
			returned = m_unboundValue.get();
		}
		MTType type = m_info->getType();
		if (m_object == nullptr) {
			*returned = nullStr;
			return *returned;
		}
		switch (type) {
		case MTType::Integer:
		{
			std::stringstream tmp;
			uint64_t integerVal = m_object->getInt();
			tmp << integerVal;
			*returned = tmp.str();
			break;
		}
		case MTType::UInteger:
		{
			std::stringstream tmp;
			uint64_t integerVal = m_object->getUInt();
			tmp << integerVal;
			*returned = tmp.str();
			break;
		}
		case MTType::Text:
			*returned = m_object->getString();
			break;
		case MTType::Date:
		{
			//const CDate& getDate()
			const ExifDateTime& date = m_object->getDate();
			*returned = ((ExifDateTime&)date).current();
			break;
		}
		case  MTType::Float:
		{
			std::stringstream tmp;
			double doubleVal = m_object->getDouble();
			tmp << doubleVal;
			*returned = tmp.str();
			break;
		}
		default:
			throw MTTypeException("Invalid type");
		}

		return *returned;
	}

	bool MTColumn::fromString(const char* str) {
		const std::string tmp(str);
		return fromString(tmp);
	}

	bool MTColumn::fromString(const std::string& str) {
		MTType type = m_info->getType();
		std::stringstream tmp(str);
		switch (type) {
		case MTType::Integer:
		{
			int64_t i = 0;
			tmp >> i;
			set(i);
			return true;
		}
		case MTType::UInteger:
		{
			uint64_t i = 0;
			tmp >> i;
			set(i);
			return true;
		}
		case MTType::Text:
			set(str);
			//= (const std::string)str;
			return true;
		case MTType::Date:
		{
			ExifDateTime date(str.c_str());
			set(date);
			return true;
		}
		case  MTType::Float:
		{
			double doubleVal = 0.0;
			tmp >> doubleVal;
			set(doubleVal);
			return true;
		}
		}
		return false;
	}

	std::string MTColumn::token(size_t* pos, std::string& str) {
		size_t delim = str.find_first_of(DELIM, *pos);
		std::string tmp = str.substr(*pos, (delim - *pos));
		//printf("%s\n",tmp.c_str());
		*pos = (size_t)delim;
		return tmp;
	}


	void MTColumn::set(int i) {
		set((int64_t)i);
	}

	void MTColumn::set(int64_t i) {
		MTType type = m_info->getType();
		if (type == MTType::Integer) {
			if (m_object == nullptr) {
				m_object = std::make_shared<MetaTypeObject>(i);
			}
			else {
				*m_object = i;
			}
			//		boundUpdate();
		}
		else {
			throw MTTypeException("Invalid type Integer");
		}
	}

	void MTColumn::set(uint64_t i) {
		MTType type = m_info->getType();
		if (type == MTType::UInteger) {
			if (m_object == nullptr) {
				m_object = std::make_shared<MetaTypeObject>(i);
			}
			else {
				*m_object = i;
			}
			//		boundUpdate();
		}
		else {
			throw MTTypeException("Invalid type Unsigned Integer");
		}
	}

	void MTColumn::set(unsigned int i) {
		set((uint64_t)i);
	}
	
	void MTColumn::set(const char* str) {
		std::string tmp(str);
		MTType type = m_info->getType();
		if (type == MTType::Text) {
			if (m_object == nullptr) {
				m_object = std::make_shared<MetaTypeObject>(tmp.c_str());
			}
			else {
				*m_object = str;
			}
			//		boundUpdate();
		}
		else if (type == MTType::Integer) {
			if (m_object == nullptr) {
				uint64_t tmpLong = std::stoll(str);
				m_object = std::make_shared<MetaTypeObject>(tmpLong);
			}
			else
			{
				uint64_t tmpLong = std::stoll(str);
				*m_object = tmpLong;
			}
		}
		else if (type == MTType::Integer) {
			if (m_object == nullptr) {
				uint64_t tmpLong = std::stoll(str);
				m_object = std::make_shared<MetaTypeObject>(tmpLong);
			}
			else
			{
				uint64_t tmpLong = std::stoll(str);
				*m_object = tmpLong;
			}
		}
		else if (type == MTType::Date) {
			if (m_object == nullptr) {
				ExifDateTime date(str);
				m_object = std::make_shared<MetaTypeObject>(date);
			}
			else
			{
				ExifDateTime date(str);
				*m_object = date;
			}
		}
		else if (type == MTType::Float) {
			if (m_object == nullptr) {
				double tmp = atof(str);
				m_object = std::make_shared<MetaTypeObject>(tmp);
			}
			else
			{
				double tmp = atof(str);
				*m_object = tmp;
			}
		}
		else {
			throw MTTypeException("Invalid type date");
		}
	}
	void MTColumn::set(const std::string& str) {
		MTType type = m_info->getType();
		if (type == MTType::Text) {
			if (m_object == nullptr) {
				m_object = std::make_shared<MetaTypeObject>(str.c_str());
			}
			else {
				*m_object = str.c_str();
			}
			//		boundUpdate();
		}
		else {
			throw MTTypeException("Invalid type string");
		}
	}
	void MTColumn::set(const ExifDateTime& date) {
		MTType type = m_info->getType();
		if (type == MTType::Date) {
			if (m_object == nullptr) {
				m_object = std::make_shared<MetaTypeObject>(date);
			}
			else {
				*m_object = date;
			}
			//		boundUpdate();
		}
		else {
			throw MTTypeException("Invalid type date");
		}
	}
	void MTColumn::set(double d) {
		MTType type = m_info->getType();
		if (type == MTType::Float) {
			if (m_object == nullptr) {
				m_object = std::make_shared<MetaTypeObject>(d);
			}
			else {
				*m_object = d;
			}
			//		boundUpdate();
		}
		else {
			throw MTTypeException("Invalid type double");
		}
	}
	void MTColumn::set(float f) {
		set((double)f);
	}

	void MTColumn::set(MTColumn& c) {
		MTType type = m_info->getType();
		if (c.isNull()) {
			return;
		}
		switch (type) {
		case MTType::Integer:
			set(c.getInt());
			break;
		case MTType::UInteger:
			set(c.getUInt());
			break;
		case MTType::Text:
			set(c.getString());
			break;
		case MTType::Date:
			set((ExifDateTime&)c.getDate());
			break;
		case  MTType::Float:
			set(c.getDouble());
			break;
		default:
			throw MTTypeException("Non-standared invalid type.");
		}

	}

	const int64_t MTColumn::getInt() {
		MTType type = m_info->getType();
		if (type == MTType::Integer) {
			return m_object->getInt();
		}
		else {
			throw MTTypeException("Invalid type");
		}
	}

	const uint64_t MTColumn::getUInt() {
		MTType type = m_info->getType();
		if (type == MTType::UInteger) {
			return m_object->getUInt();
		}
		else {
			throw MTTypeException("Invalid type");
		}
	}

	const std::string& MTColumn::getString() {
		MTType type = m_info->getType();
		if (type == MTType::Text) {
			return m_object->getString();
		}
		else {
			throw MTTypeException("Invalid type");
		}
	}

	const ExifDateTime& MTColumn::getDate() {
		MTType type = m_info->getType();
		if (type == MTType::Date) {
			if (m_object == nullptr) {
				throw MTTypeException("null Date type");
			}
			return m_object->getDate();
		}
		else {
			throw MTTypeException("Date Invalid type");
		}
	}

	const double MTColumn::getDouble() {
		MTType type = m_info->getType();
		if (type == MTType::Float) {
			return m_object->getDouble();
		}
		else {
			throw MTTypeException("Invalid type");
		}
	}

	const float MTColumn::getFloat() {
		MTType type = m_info->getType();
		if (type == MTType::Float) {
			return (const float)m_object->getDouble();
		}
		else {
			throw MTTypeException("Invalid type");
		}
	}

	/*
	void MTColumn::boundUpdate() {
		if (m_object != 0 && m_boundString != 0) {
			*m_boundString = toString();
		}
	}
	*/

	MTRow::MTRow(MTTableSchema& schemaTable) : m_schema(schemaTable), m_delim(':')
	{
		for (auto i = m_schema.begin(); i != m_schema.end(); i++) {
			std::shared_ptr<MTColumn> col = std::make_shared<MTColumn>(*i);
			this->emplace_back(col);
		}
	}

	MTRow::MTRow(MTTableSchema& schemaTable, char delim) : m_schema(schemaTable), m_delim(delim)
	{
		for (auto i = m_schema.begin(); i != m_schema.end(); i++) {
			std::shared_ptr<MTColumn> col = std::make_shared<MTColumn>(*i);
			this->emplace_back(col);
		}
	}

	MTRow::MTRow(const MTRow& row) : m_schema(row.m_schema), m_delim(row.m_delim) {

		for (auto i = m_schema.begin(); i != m_schema.end(); i++) {
			std::shared_ptr<MTColumn> col = std::make_shared<MTColumn>(*i);
			this->emplace_back(col);
		}
		join((MTRow&)row);
	}

	MTRow& MTRow::operator=(const MTRow& row) {
		m_schema = row.m_schema;
		m_delim = row.m_delim;


		//for (auto i = this->begin(); i != this->end(); i++) {
		//	MTColumn *column = *i;
		//	delete column;
		//}
		clear();
		for (auto i = m_schema.begin(); i != m_schema.end(); i++) {
			std::shared_ptr<MTColumn> col = std::make_shared<MTColumn>(*i);
			this->emplace_back(col);
		}
		join((const MTRow&)row);
		return *this;
	}

	MTRow::~MTRow() {
		//for (std::vector<MTColumn *>::iterator i = this->begin(); i != this->end(); i++) {
		//	MTColumn *column = *i;
		//	delete column;
		//}
		this->clear();
		//delete m_schema;
	};
	/*
	bool MTRow::join(MTRow &otherRow) {

		== old ==
		MTTableSchema& thisSchema = (MTTableSchema&)getSchema();
		MTTableSchema& otherSchema = (MTTableSchema&)otherRow.getSchema();
		int thisIndex = 0;
		int otherIndex = 0;
		try {
		for (auto i = thisSchema.begin(); i != thisSchema.end(); i++, thisIndex++) {
			MTSchema& columnInfo = *i;
			//printf("%s\n", columnInfo.getName().c_str());
			otherIndex = otherSchema.getIndex(columnInfo.getName().c_str());
			if (otherIndex == -1) {
				continue;
			}
			MTColumn& thisColumn = columnAt(thisIndex);
			MTColumn& otherColumn = otherRow.columnAt(otherIndex);
			thisColumn.set(otherColumn);
		}
		} catch (const std::exception& ex) {
			//printf(" thisIndex:%d otherIndex:%d\n", thisIndex, otherIndex);
			printf("%s\n", ex.what());
		}
		return true;
		== end old ==

		MTTableSchema& thisSchema = (MTTableSchema&)getSchema();
		MTTableSchema& otherSchema = (MTTableSchema&)otherRow.getSchema();
		int thisIndex = 0;
		int otherIndex = 0;
		try {
			for (auto i = thisSchema.begin(); i != thisSchema.end(); i++, thisIndex++) {
				MTSchema& columnInfo = *i;
				//printf("%s\n", columnInfo.getName().c_str());
				otherIndex = otherSchema.getIndex(columnInfo.getName().c_str());
				if (otherIndex == -1) {
					continue;
				}
				MTColumn& thisColumn = columnAt(thisIndex);
				MTColumn& otherColumn = otherRow.columnAt(otherIndex);
				if (thisColumn.getInfo().getType() != otherColumn.getInfo().getType()) {
					printf("Invalid Types");
					ErrorCode::setErrorCode(IMGA_ERROR::TYPE_MISMATCH);
				}
				if (!otherColumn.isNull()) {
					thisColumn.set(otherColumn);
				}
			}
		}
		catch (const std::exception & ex) {
			printf(" thisIndex:%d otherIndex:%d\n", thisIndex, otherIndex);
			printf("%s\n", ex.what());
		}
		return true;
	}
	*/
	bool MTRow::join(const MTRow& otherRow) {

		MTTableSchema& thisSchema = (MTTableSchema&)getSchema();
		MTTableSchema& otherSchema = (MTTableSchema&)otherRow.getSchema();
		size_t thisIndex = 0;
		size_t otherIndex = 0;
		try {
			for (auto i = thisSchema.begin(); i != thisSchema.end(); i++, thisIndex++) {
				MTSchema& columnInfo = *i;
				//printf("%s\n", columnInfo.getName().c_str());
				otherIndex = otherSchema.getIndex(columnInfo.getName().c_str());
				if (otherIndex == -1) {
					continue;
				}
				MTColumn& thisColumn = columnAt(thisIndex);
				MTColumn& otherColumn = otherRow.columnAt(otherIndex);
				if (thisColumn.getInfo().getType() != otherColumn.getInfo().getType()) {
					printf("Invalid Types");
					//ErrorCode::setErrorCode(IMGA_ERROR::TYPE_MISMATCH);
				}
				if (!otherColumn.isNull()) {
					thisColumn.set(otherColumn);
				}

			}
		}
		catch (const std::exception& ex) {
			printf(" this Index:%I64d other Index:%I64d\n", thisIndex, otherIndex);
			printf("%s\n", ex.what());
		}
		return true;

	}


	SharedMTRow MTTable::makeRow() {
		auto row = std::make_shared<MTRow>(*(m_TableSchema.get()));
		return row;
	}

	bool MTTable::addRow(const MTRow& r) {
		auto row = std::make_shared<MTRow>(r);
		emplace_back(row);
		return true;
	}

	bool MTTable::fromString(const std::string& r) {
		MTTableSchema& ts = *m_TableSchema;
		auto rowPtr = std::make_shared<MTRow>(ts);
		//MTRow *row = rowPtr.get();
		if (!(rowPtr->fromString(r))) {
			return false;
		}
		push_back(std::move(rowPtr));
		return true;
	}

	bool MTTable::read(const char* path, const char* filename) {
		std::string fullpath(path);
		fullpath += '/';
		fullpath += filename;
		return read(fullpath.c_str());
	}


	bool MTTable::read(const char* fullpath) {


		clear();

		std::ifstream file(fullpath);
		if (file.is_open() == false) {
			//ErrorCode::setErrorCode(IMGA_ERROR::OPEN_ERROR);
			return false;
		}

		for (std::string line; std::getline(file, line);) {
			if (line.length() > 0) {
				if (fromString(line) == false) {
					//ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
					return false;
				}
			}
			else {
				break;
			}
		}


		file.close();
		m_rowCursor = 0;
		return true;
	}

	bool MTTable::write(const char* path, const char* filename) {
		std::string fullpath(path);
		fullpath += '/';
		fullpath += filename;
		return write(fullpath.c_str());
	}

	bool MTTable::write(const char* fullpath) {


		std::ofstream file(fullpath, std::ofstream::trunc);
		if (file.is_open() == false) {
			//ErrorCode::setErrorCode(IMGA_ERROR::OPEN_ERROR);
			return false;
		}
		std::stringstream s;
		for (auto i = this->begin(); i != this->end(); i++) {
			s << (*i)->toString() << "\n";

		}
		this->clear();
		file << s.str();
		file.close();
		return true;
	}

	bool MTTable::find(MTColumn& column) {

		if (m_rowCursor == NOT_FOUND) {
			m_rowCursor = 0;
		}
		if (empty()) {
			return false;
		}
		int pos = m_rowCursor;
		if (size() <= (size_type)pos) {
			return false;
		}
		MTSchema& thisSchema = column.getInfo();
		std::string name = thisSchema.getName();
		size_t idx = this->m_TableSchema->getIndex(name.c_str());
		bool first = true;
		for (auto i = begin() + pos; i != end(); i++, pos++) {
			auto row = *i;
			MTColumn& other = row->columnAt(idx);
			MTType type = thisSchema.getType();

			switch (type) {
			case MTType::Integer:
				if (other.getInt() == column.getInt()) {
					m_rowCursor = pos;
					return true;
				}
				break;
			case MTType::UInteger:
				if (other.getUInt() == column.getUInt()) {
					m_rowCursor = pos;
					return true;
				}
				break;
			case MTType::Text:
				if (other.getString() == column.getString()) {
					m_rowCursor = pos;
					return true;
				}
				break;
			case MTType::Date:
			{
				ExifDateTime otherDate = other.getDate();
				ExifDateTime columnDate = column.getDate();
				if (otherDate == columnDate) {
					m_rowCursor = pos;
					return true;
				}
				break;
			}
			case  MTType::Float:
				if (other.getDouble() == column.getDouble()) {
					m_rowCursor = pos;
					return true;
				}
				break;
			default:
				throw MTTypeException("Invalid type");
			}

		}
		return false;
	}

	void MTTable::print()
	{
		for (auto i = begin(); i != end(); i++) {
			auto row = *i;
			row->print(); printf("\n");
		}
	}

	bool MTDatabase::addTable(MTTableSchema* pSchemaTable) {
		auto table = std::make_unique<MTTable>(pSchemaTable);
		insert(std::make_pair(pSchemaTable->getName(), std::move(table)));
		return true;
	}

	MTTable& MTDatabase::getTable(std::string name) {
		auto iter = find(name);
		if (iter == end()) {
			throw std::exception();
		}
		return *(iter->second);
	}
};