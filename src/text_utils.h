/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#pragma once

#pragma once

#include <string>
#include <list>
#include <functional>
#include <vector>

namespace TextUtils
{

typedef struct textSplitter_s
{
	const char* value;
	size_t	length;
}textSplitter_t;

typedef std::list<textSplitter_t> AnsiTextSplitterList;

typedef std::list<const char*> AnsiConstCharist;
typedef std::list<std::string> AnsiStringList;

typedef std::vector<std::string> AnsiStringArray;

typedef std::function<void(TextUtils::AnsiStringArray&)> pfnCSVLoaderRowCallback;

class CSVSettings
{
	AnsiTextSplitterList m_lstRowSplitters;
	AnsiTextSplitterList m_lstColumnSplitters;

	char m_cEscapeChar;
public:
	CSVSettings();
	~CSVSettings();

	void InitDefault();

	void AddRowSplitter(const char* splitter);
	void AddColumnSplitter(const char* splitter);

	void SetStringEscaping(char escapeChar);

	AnsiTextSplitterList& GetRowSplitters();
	AnsiTextSplitterList& GetColumnSplitters();
	char StringEscapeChar();
};

AnsiStringList SplitTextWhitespaces(const char* data, const size_t length);
AnsiStringList SplitTextSimple(const char* data, const size_t length, char splitter);
AnsiStringArray SplitTextEx(const char* data, size_t dataLength, AnsiTextSplitterList& splitters, char cEscapeChar = 0);


void ParseCSVFile(const char* data, size_t dataSize, CSVSettings* settings, pfnCSVLoaderRowCallback rowCallback);
}

