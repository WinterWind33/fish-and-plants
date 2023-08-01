// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <version/version-numbers.hpp>

// Third-party
#include <folder-provider/folder-provider.hpp>
#include <semver.hpp>

// C++ STL
#include <filesystem>
#include <ostream>

namespace rpi_gc {

struct ApplicationConfiguration {
    semver::version applicationVersion;
    std::filesystem::path lastProjectFolder;
};

inline auto operator<<(std::ostream& os, const ApplicationConfiguration& applicationConfiguration)
    -> std::ostream& {
    // We write the application configuration in a JSON format to the given
    // output stream.
    os << R"(
        {
            "version": ")"
       << applicationConfiguration.applicationVersion.to_string() << R"(",
            "lastProjectFolder": ")"
       << applicationConfiguration.lastProjectFolder.string() << R"("
        }
    )" << std::endl;

    return os;
}

//!!
//! \brief Creates the default application configuration with the
//!  project version and the given folder as the last project folder.
//! \param lastProjectFolder The last project folder.
//!
[[nodiscard]] inline auto CreateDefaultApplicationConfiguration(
    std::filesystem::path lastProjectFolder) noexcept -> ApplicationConfiguration {
    return ApplicationConfiguration{version::getApplicationVersion(), std::move(lastProjectFolder)};
}

//!!
//! \brief Retrieves the default application config file path.
//!
[[nodiscard]] inline auto GetDefaultApplicationConfigFilePath(
    const gc::folder_provider::FolderProvider& folderProvider) noexcept -> std::filesystem::path {
    return folderProvider.getAppDataFolder() / "FishAndPlants" / "rpi_gc" / "rpi_gc_config.json";
}

} // namespace rpi_gc
