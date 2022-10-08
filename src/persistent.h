#pragma once

#include <nlohmann/json.hpp>
#include <stack>
#include <list>
#include "object_props.h"

enum class ApplicationSettings
{
	ShowGround = 0,
	BackgroundColor
};


class PersistentStorage
{
	std::list<std::string>	m_lstMRUFiles;
	void						SaveToFile();
	void						LoadFromFile();
	
	std::vector<propsData_t> m_ApplicationProps;
public:
	
	PersistentStorage();
	~PersistentStorage();

	void						PushMRUFile(const char* fileName);
	std::list<std::string>&		GetMRUFiles();
		
	
	propsData_t * GetSetting(ApplicationSettings id);
private:
	nlohmann::json SerializeApplicationProperty(nlohmann::json& j, propsData_t& it);
	void ParseApplicationSettings(nlohmann::json j);
};