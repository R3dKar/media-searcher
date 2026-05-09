#include "core.hpp"
#include "utility.hpp"
#include <argparse/argparse.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <httplib.h>

int main(int argc, char** argv) {
  namespace fs = std::filesystem;

  argparse::ArgumentParser program(argv[0]);

  auto& work_mode = program.add_mutually_exclusive_group();
  work_mode.add_argument("-s", "--server").help("starts server").default_value(false).implicit_value(true);
  work_mode.add_argument("-f", "--file").help("fetch data over time and store to file").default_value(true).implicit_value(true);

  fs::path search_dir = "~";
  program.add_argument("-S", "--search-dir").help("directory where media is searched").store_into(search_dir);

  // Server
  uint16_t port = 1234;
  program.add_argument("-p", "--port").help("server port to serve on").store_into(port);

  // File
  size_t interval_seconds = 300;
  program.add_argument("-i", "--interval").help("interval over which data is stored to file").store_into(interval_seconds);

  fs::path output_file_path = "~/.media_files";
  program.add_argument("-o", "--output-file").help("file where data is stored").store_into(output_file_path);

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception& err) {
    std::cerr << err.what() << '\n';
    std::cerr << program;
    return 1;
  }

  // Prepare paths
  search_dir = media_searcher::utility::expand_home(search_dir);
  output_file_path = media_searcher::utility::expand_home(output_file_path);

  // Check options
  // Search directory
  if (!fs::exists(search_dir)) {
    std::cerr << "Search directory does not exist: " << search_dir << '\n';
    return 1;
  } else if (!fs::is_directory(search_dir)) {
    std::cerr << "Search directory is not a directory: " << search_dir << '\n';
    return 1;
  }

  // Output file path
  const fs::path output_parent_path = output_file_path.parent_path();
  if (!output_parent_path.empty() && (!fs::exists(output_parent_path) || !fs::is_directory(output_parent_path))) {
    std::cerr << "Output file directory does not exists: " << output_parent_path << '\n';
    return 1;
  } else if (fs::exists(output_file_path) && !fs::is_regular_file(output_file_path)) {
    std::cerr << "Output file exists and is not a file: " << output_file_path << '\n';
    return 1;
  }

  if (program.is_used("--server")) {
    // Server
    httplib::Server server{};

    server.Get("/media_files", [&](const httplib::Request&, httplib::Response& res) {
      const auto data = media_searcher::search_media(search_dir);
      res.set_content(data.dump(), "application/json");
    });

    server.listen("0.0.0.0", port);
  } else {
    // File
    while (true) {
      const auto data = media_searcher::search_media(search_dir);

      std::ofstream output_file(output_file_path);
      output_file << data.dump();
      output_file.close();

      std::this_thread::sleep_for(std::chrono::seconds(interval_seconds));
    }
  }

  return 0;
}
