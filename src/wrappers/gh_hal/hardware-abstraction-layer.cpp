// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/hardware-abstraction-layer.hpp>

#ifdef USE_LIBGPIOD
#include <gh_hal/backends/libgpiod/miscellaneous.hpp>
#endif // USE_LIBGPIOD

// C++ STL
#include <string_view>

namespace gh_hal {

    namespace details {

#ifdef USE_LIBGPIOD
        constexpr std::string_view BACKEND_MODULE_NAME{"libgpiod"};

        backends::libgpiod_impl::VersionString GetBackendModuleVersionString() noexcept {
            return backends::libgpiod_impl::GetLibraryVersion();
        }
#else
        constexpr std::string_view BACKEND_MODULE_NAME{"[None - Simulated]"};
        constexpr std::string_view BACKEND_MODULE_VERSION_STRING{"[None - Simulated]"};

        constexpr std::string_view GetBackendModuleVersionString() noexcept {
            return BACKEND_MODULE_VERSION_STRING;
        }
#endif // USE_LIBGPIOD

    } // namespace details

    auto BackendModule::GetBackendModuleName() noexcept -> name_type {
        return name_type{details::BACKEND_MODULE_NAME};
    }

    auto BackendModule::GetBackendModuleVersion() noexcept -> version_type {
        return version_type{details::GetBackendModuleVersionString()};
    }

} // namespace gh_hal
