// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gh_hal/hardware-access/board-digital-pin.hpp>

// C++ STL
#include <vector>
#include <filesystem>
#include <memory>
#include <type_traits>

namespace gh_hal::hardware_access {

    //!!
    //! \brief Basic interface of a board chip that can be used to
    //!  access the hardware and it can provide access to the board's PINs.
    struct BoardChip {
        virtual ~BoardChip() noexcept = default;

        virtual std::unique_ptr<BoardDigitalPin> requestDigitalPin(std::string consumer,
            BoardDigitalPin::offset_type offset, const DigitalPinRequestDirection direction) noexcept = 0;

        virtual std::vector<std::unique_ptr<BoardDigitalPin>> requestDigitalPinPool(std::string consumer,
            std::vector<BoardDigitalPin::offset_type> offset, const DigitalPinRequestDirection direction) noexcept = 0;

        //!!
        //! \brief Releases the request identified by the given offset.
        //! \note The line request must have all the offset specified in the given vector otherwise
        //!  nothing will be released.
        //!
        //! \param offsets The offsets pool that identifies the line request to be released.
        //! \return Trus if a request has beed freed, false otherwise.
        virtual bool releaseRequest(std::vector<BoardDigitalPin::offset_type> offsets) noexcept = 0;
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
