// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <folder-provider/folder-provider.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

namespace gc::folder_provider::mocks {

//!!
//! \brief Mock class for FolderProvider
//!
class FolderProviderMock : public FolderProvider {
public:
    MOCK_METHOD(std::filesystem::path, getAppDataFolder, (), (const, final));
};

} // namespace gc::folder_provider::mocks
