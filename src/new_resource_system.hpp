#pragma once
#include <string_view>
#include "aseprite.hpp"

namespace ResourceSystem {
    template<size_t tag_count, size_t slice_cout>
    void loadAseprite(const char *path, const std::string_view &key);

    Aseprite::Aseprite getAseprite(const std::string_view &key);
}
