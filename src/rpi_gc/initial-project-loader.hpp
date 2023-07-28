// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <folder-provider/folder-provider.hpp>
#include <gh_log/logger.hpp>

// C++ STL
#include <functional> // for std::reference_wrapper

namespace rpi_gc {

//!!
//! \brief Represents an object that will read the project to
//!  load at startup from a file stored in the default.
//!  app data folder of the operating system.
class InitialProjectLoader final {
public:
    explicit InitialProjectLoader(gh_log::Logger& logger,
                                  gc::folder_provider::FolderProvider& folder_provider) noexcept;

private:
    std::reference_wrapper<gh_log::Logger> m_logger;
    std::reference_wrapper<gc::folder_provider::FolderProvider> m_folder_provider;
};

} // namespace rpi_gc
