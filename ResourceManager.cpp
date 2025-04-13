#include "ResourceManager.h"

std::unordered_map<std::string, std::weak_ptr<Tmpl8::Surface>> Util::ResourceManager::opened_resources;

void Util::ResourceManager::cleanup()
{
    for (auto it = opened_resources.begin(); it != opened_resources.end(); ) {
        if (it->second.expired()) {
            it = opened_resources.erase(it); // Erase and get next iterator
        }
        else {
            ++it; // Move to the next element
        }
    }
}

std::shared_ptr<Tmpl8::Surface> Util::ResourceManager::LoadImage(const std::string& path)
{
    cleanup();

    if (std::shared_ptr<Tmpl8::Surface> existing = opened_resources[path].lock()) {
		return existing;
	}

    std::shared_ptr<Tmpl8::Surface> new_surface = std::make_shared<Tmpl8::Surface>(path.c_str());
    opened_resources[path] = new_surface;
    return new_surface;
}
