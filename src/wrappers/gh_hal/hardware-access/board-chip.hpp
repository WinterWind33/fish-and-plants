// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <filesystem>
#include <memory>
#include <type_traits>

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
        using board_chip_type = BoardChip;

        //!!
        //! \brief Opens a chip that is represented by the given path, usually something
        //!  like "/dev/gpiochip0".
        //!  This can throw an exception if the chip cannot be opened.
        //!
        //! \param chipPath The path of the chip to be opened.
        //! \return A pointer to the opened chip or nullptr in case of an error.
        static std::unique_ptr<board_chip_type> openChipByPath(std::filesystem::path chipPath);
    };

    //!!
    //! \brief Represents the concept of a BoardChipFactory, i.e. a class that has a static method
    //!  called openChipByPath that accepts a std::filesystem::path and returns a unique_ptr to a board
    //!  chip interface.
    //!
    template<typename T>
    concept BoardChipFactoryType = std::is_same_v<T, BoardChipFactory> || requires (std::filesystem::path P){
        typename T::board_chip_type;
        { T::openChipByPath(P) } -> std::same_as<std::unique_ptr<typename T::board_chip_type>>;
    };

} // namespace hardware_access
