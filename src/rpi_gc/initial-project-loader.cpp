// Copyright (c) 2023 Andrea Ballestrazzi
#include <initial-project-loader.hpp>

namespace rpi_gc {

InitialProjectLoader::InitialProjectLoader(
    gh_log::Logger& logger, gc::folder_provider::FolderProvider& folder_provider) noexcept
    : m_logger(logger),
      m_folder_provider(folder_provider) {}

} // namespace rpi_gc
