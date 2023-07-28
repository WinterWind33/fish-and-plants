// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <filesystem>
#include <memory>

namespace gc::folder_provider {

//!!
//! \brief Interface for a folder provider. It retrieves
//!  the utilities OS folders independently from the platform.
struct FolderProvider {
    virtual ~FolderProvider() noexcept = default;

    [[nodiscard]] static std::unique_ptr<FolderProvider> create();

    //!!
    //! \brief Retrieves the "AppData" folder, i.e. the folder
    //!  where the application can store its data.
    //!
    //! \return The path to the "AppData" folder.
    [[nodiscard]] virtual std::filesystem::path getAppDataFolder() const = 0;
};

} // namespace gc::folder_provider
