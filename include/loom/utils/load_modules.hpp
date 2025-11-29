#include "sol/state.hpp"
#include "spdlog/spdlog.h"
#include <RLL/RLL.hpp>
#include <filesystem>
#include <iostream>

namespace loom::utils {

// Global module list, otherwise they get unloaded and no longer can be called
// from lua
static std::vector<std::unique_ptr<rll::shared_library>>
    g_loaded_modules; // NOLINT

inline void load_and_run_register(sol::state &lua, sol::environment &sandbox) {
  namespace fs = std::filesystem;

  for (const auto &entry : fs::directory_iterator(fs::current_path())) {
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
