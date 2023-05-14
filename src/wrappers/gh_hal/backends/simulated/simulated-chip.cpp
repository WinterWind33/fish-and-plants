// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/backends/simulated/simulated-chip.hpp>

// C++ STL

namespace gh_hal::backends::simulated {

    SimulatedChip::SimulatedChip(chip_path chipPath) noexcept :
        m_chipPath{std::move(chipPath)},
        m_bIsOpened{true} {}

    void SimulatedChip::close() noexcept {
        m_bIsOpened = false;
        m_chipPath.clear();
    }

} // namespace gh_hal::backends::simulated
