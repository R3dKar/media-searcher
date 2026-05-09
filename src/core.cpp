#include "core.hpp"
#include "utility.hpp"
#include <string_view>
#include <unordered_map>

namespace media_searcher {
  nlohmann::json search_media(const std::filesystem::path& search_dir) {
    using namespace nlohmann::json_literals;
    using namespace std::string_view_literals;
    namespace fs = std::filesystem;

    static const std::unordered_map<std::string_view, std::string_view> extension_category_map = {
        {".mp3"sv, "audio"sv},   {".aac"sv, "audio"sv},  {".flac"sv, "audio"sv}, {".m4a"sv, "audio"sv},  {".wav"sv, "audio"sv},
        {".aiff"sv, "audio"sv},  {".ogg"sv, "audio"sv},  {".opus"sv, "audio"sv}, {".mp4"sv, "video"sv},  {".mkv"sv, "video"sv},
        {".avi"sv, "video"sv},   {".mov"sv, "video"sv},  {".webm"sv, "video"sv}, {".flv"sv, "video"sv},  {".jpg"sv, "images"sv},
        {".jpeg"sv, "images"sv}, {".png"sv, "images"sv}, {".gif"sv, "images"sv}, {".bmp"sv, "images"sv}, {".tif"sv, "images"sv},
        {".webp"sv, "images"sv}, {".svg"sv, "images"sv}};

    nlohmann::json data = R"({
      "audio": [],
      "video": [],
      "images": []
    })"_json;

    if (!fs::is_directory(search_dir)) return data;

    // more code to be able to run it on Windows with some WSL files in file tree
    // which crashes the entire thing
    std::error_code os_error;
    auto walker = fs::recursive_directory_iterator(search_dir, fs::directory_options::skip_permission_denied);

    // yeah, increment and operator++ logic are different
    for (auto it = fs::begin(walker); it != fs::end(walker); it = it.increment(os_error)) {
      if (os_error) continue;

      const auto& entry = *it;
      try {
        if (!entry.is_regular_file()) continue;
      } catch (fs::filesystem_error) {
        continue;
      }

      const auto extension = utility::to_lower(utility::path_to_utf8(entry.path().extension()));

      if (!extension_category_map.contains(extension)) continue;

      const auto path = utility::path_to_utf8(entry.path().lexically_relative(search_dir));
      data[extension_category_map.at(extension)].emplace_back(std::move(path));
    }

    return data;
  }
} // namespace media_searcher
