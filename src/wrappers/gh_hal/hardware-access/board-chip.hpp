// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <filesystem>
#include <memory>

namespace gh_hal::hardware_access {

    struct BoardChip {
        virtual ~BoardChip() noexcept = default;
    };

} // namespace hardware_access
