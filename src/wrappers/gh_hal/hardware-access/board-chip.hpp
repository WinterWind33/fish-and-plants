// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <filesystem>
#include <memory>

namespace gh_hal::hardware_access {

    //!!
    //! \brief Basic interface of a board chip that can be used to
    //!  access the hardware and it can provide access to the board's PINs.
    struct BoardChip {
        virtual ~BoardChip() noexcept = default;
    };

    //!!
    //! \brief Represents the standard factory that can open chips.
    //!
    class BoardChipFactory final {
    public:

        //!!
        //! \brief Opens a chip that is represented by the given path, usually something
        //!  like "/dev/gpiochip0".
        //!  This can throw an exception if the chip cannot be opened.
        //!
        //! \param chipPath The path of the chip to be opened.
        //! \return A pointer to the opened chip or nullptr in case of an error.
        static std::unique_ptr<BoardChip> openChipByPath(std::filesystem::path chipPath);
    };

} // namespace hardware_access
