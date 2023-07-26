// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <folder-provider/folder-provider.hpp>

namespace gc::folder_provider::internal {

class FolderProviderImpl final : public FolderProvider {
public:
    std::filesystem::path getAppDataFolder() const override;
};

} // namespace gc::folder_provider::internal
