#pragma once
#include "../include/entt.hpp"

namespace Hierarchy {
    struct ChildsComponent {
        std::vector<entt::entity> childs {};
    };
    
    struct ParentComponent {
        entt::entity parent = entt::null;
    };
    
    inline void addChildToHierarchy(entt::registry &object_registry, const entt::entity child, const entt::entity parent) 
    {
        auto &hierarchy = object_registry.get<ChildsComponent>(parent);
        object_registry.emplace<ParentComponent>(child, parent);
        hierarchy.childs.push_back(child);
    }
}
