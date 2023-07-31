// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gh_log/logger.hpp>
#include <project-management/project.hpp>

// C++ STL
#include <filesystem>
#include <optional>

namespace rpi_gc {

//!!
//! \brief Loads a project from a given path and send it to the standard integrity checkers.
//!
[[nodiscard]] auto LoadProjectAndCheckIntegrity(const std::filesystem::path& projectPath,
                                                gh_log::Logger& logger) noexcept
    -> std::optional<gc::project_management::Project>;

} // namespace rpi_gc
