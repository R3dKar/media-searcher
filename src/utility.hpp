#pragma once

#include <filesystem>
#include <string>

namespace media_searcher::utility {
  std::string path_to_utf8(const std::filesystem::path& path);
  std::string to_lower(const std::string& str);
  std::filesystem::path get_home_dir();
  std::filesystem::path expand_home(const std::filesystem::path& path);
} // namespace media_searcher::utility
