#include "utility.hpp"
#include <cstdlib>
#include <ranges>

#if !defined(_WIN32)
#include <pwd.h>
#include <unistd.h>
#endif

namespace media_searcher::utility {
  std::string path_to_utf8(const std::filesystem::path& path) {
    const auto path_u8 = path.u8string();
    return {reinterpret_cast<const char*>(path_u8.data()), path_u8.size()};
  }

  std::string to_lower(const std::string& str) {
    const auto view = str | std::views::transform([](uint8_t c) { return std::tolower(c); });
    return {view.begin(), view.end()};
  }

  std::filesystem::path get_home_dir() {
    namespace fs = std::filesystem;

#if defined(_WIN32)
    const char* userprofile = std::getenv("USERPROFILE");
    if (userprofile && fs::is_directory(userprofile)) {
      return userprofile;
    } else {
      const auto homedrive = fs::path(std::getenv("HOMEDRIVE"));
      const auto homepath = fs::path(std::getenv("HOMEPATH"));
      return homedrive / homepath;
    }
#else
    const char* home = std::getenv("HOME");
    if (home && fs::is_directory(home)) {
      return home;
    } else {
      return getpwuid(getuid())->pw_dir;
    }
#endif
  }

  std::filesystem::path expand_home(const std::filesystem::path& path) {
    namespace fs = std::filesystem;

    if (path.empty()) return path;
    if (*path.begin() != "~") return path;

    fs::path expanded = get_home_dir();

    bool skipped_home = false;
    for (const auto& item : path) {
      if (!skipped_home) {
        skipped_home = true;
        continue;
      }

      expanded /= item;
    }

    return expanded;
  }
} // namespace media_searcher::utility
