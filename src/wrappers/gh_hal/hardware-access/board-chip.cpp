// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/hardware-access/board-chip.hpp>

// Implementations
#include <gh_hal/internal/board-chip-impl.hpp>

namespace gh_hal::hardware_access {

std::unique_ptr<BoardChip> BoardChipFactory::openChipByPath(std::filesystem::path chipPath) {
    return std::make_unique<internal::BoardChipImpl>(std::move(chipPath));
}

} // namespace gh_hal::hardware_access
