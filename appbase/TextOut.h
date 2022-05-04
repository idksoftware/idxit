#pragma once
#include <string>

enum class TextOutType {
	plain,
	xml,
	json,
	html,
	unknown
};

#define XML_HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"

class TextOut
{	
protected:
	std::string m_filename;
	std::string writePlainTag(const char* tag, const std::string& value);
	std::string writeXMLTag(const char* tag, const std::string& value);
	std::string writeJsonTag(const char* tag, const std::string& value, bool end = false);
	std::string writeHtmlTag(const char* tag, const std::string& value);

	std::string writeXMLTag(const char* tag, bool b);

	std::string writePlainTag(const char* tag, const char* value);
	std::string writeXMLTag(const char* tag, const char* value);
	std::string writeJsonTag(const char* tag, const char* value, bool end = false);
	std::string writeHtmlTag(const char* tag, const char* value);
	TextOutType m_textOutType;
	std::string toString();
public:
	TextOut() : m_textOutType(TextOutType::plain) {};
	virtual ~TextOut() = default;
	void setFilename(const char *fn) {
		m_filename = fn;
	}
	bool parseTextOutType(const char* s);
	void process();
	virtual std::string writePlain() = 0;
	virtual std::string writeXML() = 0;
	virtual std::string writeJson() = 0;
	virtual std::string writeHtml() = 0;
};

