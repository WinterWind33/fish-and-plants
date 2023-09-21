// Copyright (c) 2023 Andrea Ballestrazzi
#include <folder-provider-impl.hpp>

// Third-party
#include <sago/platform_folders.h>

namespace gc::folder_provider::internal {

std::filesystem::path FolderProviderImpl::getAppDataFolder() const {
    return sago::getDataHome();
}

} // namespace gc::folder_provider::internal
