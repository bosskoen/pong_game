#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Surface.h"


namespace Util{
class ResourceManager
{
private: 
	static std::unordered_map<std::string, std::weak_ptr<Tmpl8::Surface>> opened_resources;

	
public:
	static std::shared_ptr<Tmpl8::Surface> LoadImage(const std::string& path);
	static void cleanup();
};

}

