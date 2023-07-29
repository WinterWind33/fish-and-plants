// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <folder-provider/folder-provider.hpp>
#include <gh_log/logger.hpp>
#include <project-management/project.hpp>

// C++ STL
#include <functional> // for std::reference_wrapper
#include <optional>
#include <tuple> // for std::pair

namespace rpi_gc {

//!!
//! \brief Represents an object that will read the project to
//!  load at startup from a file stored in the default.
//!  app data folder of the operating system.
class InitialProjectLoader final {
public:
    explicit InitialProjectLoader(gh_log::Logger& logger,
                                  gc::folder_provider::FolderProvider& folder_provider) noexcept;

    //!!
    //! \brief Searches for the app data folder it the operating system folders
    //!  and tries to load the project from the config file stored in it.
    //!  If the application folder or the config file is not found, this function will create
    //!  them and return an empty optional.
    //!
    //!  If a config file is found then it will be parsed and it will search for the last
    //!  project that was opened and it will try to load it.
    //!
    //! \return An optional containing the project if it was loaded successfully.
    //!  Otherwise an empty optional.
    std::optional<std::pair<gc::project_management::Project, std::filesystem::path>>
    tryLoadCachedProject() noexcept;

private:
    std::reference_wrapper<gh_log::Logger> m_logger;
    std::reference_wrapper<gc::folder_provider::FolderProvider> m_folderProvider;
};

} // namespace rpi_gc
