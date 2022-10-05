#pragma once

#include <nlohmann/json.hpp>
#include <stack>
#include <list>

class PersistentStorage
{
	std::list<std::string>	m_lstMRUFiles;
	void						SaveToFile();
	void						LoadFromFile();
public:
	PersistentStorage();
	~PersistentStorage();

	void						PushMRUFile(const char* fileName);
	std::list<std::string>&		GetMRUFiles();
	
};