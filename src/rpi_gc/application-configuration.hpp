// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <version/version-numbers.hpp>

// Third-party
#include <folder-provider/folder-provider.hpp>
#include <nlohmann/json.hpp>
#include <semver.hpp>

// C++ STL
#include <filesystem>
#include <ostream>

namespace rpi_gc {

struct ApplicationConfiguration {
    semver::version applicationVersion;
    std::filesystem::path lastProjectLocation;
};

inline auto operator<<(std::ostream& os, const ApplicationConfiguration& applicationConfiguration)
    -> std::ostream& {
    // We write the application configuration in a JSON format to the given
    // output stream.
    nlohmann::json applicationConfigurationJson{};
    applicationConfigurationJson["version"] =
        applicationConfiguration.applicationVersion.to_string();
    applicationConfigurationJson["lastProjectLocation"] =
        applicationConfiguration.lastProjectLocation.string();

    os << applicationConfigurationJson.dump(4);
    return os;
}

//!!
//! \brief Creates the default application configuration with the
//!  project version and the given folder as the last project folder.
//! \param lastProjectLocation The last project folder.
//!
[[nodiscard]] inline auto CreateDefaultApplicationConfiguration(
    std::filesystem::path lastProjectLocation) noexcept -> ApplicationConfiguration {
    return ApplicationConfiguration{version::getApplicationVersion(),
                                    std::move(lastProjectLocation)};
}

//!!
//! \brief Retrieves the default application config file path.
//!
[[nodiscard]] inline auto GetDefaultApplicationConfigFilePath(
    const gc::folder_provider::FolderProvider& folderProvider) noexcept -> std::filesystem::path {
    return folderProvider.getAppDataFolder() / "FishAndPlants" / "rpi_gc" / "rpi_gc_config.json";
}

} // namespace rpi_gc
