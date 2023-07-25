// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <filesystem>

namespace gh_hal::backends::simulated {

//!!
//! \brief Representation implemented via a software API of a chip. This
//!  represents only a simulated chip and won't performa any system call or
//!  hardware access.
//! \note As this is only a representation, no failure is expected and no error
//!  will ever be set.
class SimulatedChip final {
public:
    using chip_path = std::filesystem::path;

    //!!
    //! \brief Construct a new Simulated Chip object with the given chip path.
    //!
    //! \param chipPath The path used to open the requested chip.
    explicit SimulatedChip(chip_path chipPath) noexcept;

    //!!
    //! \brief Closes the simulation, setting the state to false and resetting
    //!  the chip path.
    //! \note If this method is called more than once on the same object no
    //!  error or exception will be thrown.
    void close() noexcept;

    //!!
    //! \brief Retrieves the path used to open this chip.
    //!
    chip_path getChipPath() const noexcept {
        return m_chipPath;
    }

    //!!
    //! \brief Checks whether or not the simulation is active for this chip object.
    //!
    explicit operator bool() const noexcept {
        return m_bIsOpened;
    }

private:
    chip_path m_chipPath{};
    bool m_bIsOpened{};
};

} // namespace gh_hal::backends::simulated
