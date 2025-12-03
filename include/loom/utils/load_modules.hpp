#include "sol/state.hpp"
#include "spdlog/spdlog.h"
#include <RLL/RLL.hpp>
#include <filesystem>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#else
#include <climits>
#include <unistd.h>
#endif

namespace loom::utils {

inline auto get_executable_dir() -> std::filesystem::path {
#if defined(_WIN32)
  std::array<wchar_t, MAX_PATH> buffer{};
  DWORD len = GetModuleFileNameW(NULL, buffer.data(),
                                 static_cast<DWORD>(buffer.size()));
  if (len == 0 || len == buffer.size()) {
    throw std::runtime_error("Failed to get executable path");
  }
  return std::filesystem::path(buffer.data()).parent_path();
#else
  constexpr size_t bufferSize = PATH_MAX;
  std::array<char, bufferSize> buffer{};
  ssize_t len = readlink("/proc/self/exe", buffer.data(), buffer.size());
  if (len == -1) {
    throw std::runtime_error("Failed to get executable path");
  }
  return std::filesystem::path(std::string(buffer.data(), len)).parent_path();
#endif
}

// Global module list, otherwise they get unloaded and no longer can be called
// from lua
static std::vector<std::unique_ptr<rll::shared_library>>
    g_loaded_modules; // NOLINT

inline void load_and_run_register(sol::state &lua, sol::environment &sandbox) {
  namespace fs = std::filesystem;

  for (const auto &entry : fs::directory_iterator(get_executable_dir())) {
    if (!entry.is_regular_file()) {
      continue;
    }

    const auto &path = entry.path();
    auto filename = path.filename().string();

    if (!filename.starts_with("libloom_") ||
        !(path.extension() == ".so" || path.extension() == ".dll")) {
      continue;
    }

    try {
      // Create library object on heap
      auto lib = std::make_unique<rll::shared_library>();
      lib->load(path);

      // Register if symbol exists
      if (lib->has_symbol("register_loom_module")) {
        auto fn =
            lib->get_function_symbol<void(sol::state &, sol::environment &)>(
                "register_loom_module");
        fn(lua, sandbox);
        spdlog::info("Module {} registered", filename);
      } else {
        spdlog::error("Module {} missing register_loom_module", filename);
      }

      g_loaded_modules.push_back(std::move(lib));

    } catch (const std::exception &e) {
      spdlog::critical("Failed to load {}: {}", filename, e.what());
    }
  }
}

} // namespace loom::utils
