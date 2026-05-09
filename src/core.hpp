#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

namespace media_searcher {
  nlohmann::json search_media(const std::filesystem::path& search_dir);
}
