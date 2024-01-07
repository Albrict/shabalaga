#include "new_resource_system.hpp"
#include "aseprite.hpp"
#include <unordered_map>

namespace ResourceSystem {
    template<size_t T>
    struct AsepriteResource {
        Aseprite::Aseprite aseprite;
    };

    std::unordered_map<std::string_view, Aseprite::Aseprite> aseprite_storage;
}

void ResourceSystem::loadAseprite(const char *path, const std::string_view &key)
{
    Aseprite::Aseprite aseprite = Aseprite::LoadAseprite(path);
}

Aseprite::Aseprite ResourceSystem::getAseprite(const std::string_view &key)
{
    
}
