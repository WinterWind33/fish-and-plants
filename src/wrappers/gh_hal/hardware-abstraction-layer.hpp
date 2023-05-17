// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <string>
#include <cstdint>

namespace gh_hal {

    //!!
    //! \brief Utility class, used to retrieve info about the
    //! backend module.
    //!
    class BackendModule final {
    public:
        using name_type = std::string;
        using version_type = std::string;

        //!!
        //! \brief Get the Backend Module Name
        //!
        static auto GetBackendModuleName() noexcept -> name_type;

        //!!
        //! \brief Get the Backend Module Version
        //!
        static version_type GetBackendModuleVersion() noexcept;
    };

} // namespace gh_hal
