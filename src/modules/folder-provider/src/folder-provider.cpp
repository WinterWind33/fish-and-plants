// Copyright (c) 2023 Andrea Ballestrazzi
#include <folder-provider/folder-provider.hpp>

#include <folder-provider-impl.hpp>

namespace gc::folder_provider {

std::unique_ptr<FolderProvider> FolderProvider::create() {
    // We simply create a FolderProviderImpl object and we return
    // it.
    return std::make_unique<internal::FolderProviderImpl>();
}

} // namespace gc::folder_provider
