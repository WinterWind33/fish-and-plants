// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <cstdint>
#include <ostream>

namespace gh_hal::hardware_access {

    //!!
    //! \brief Represents the direction of a line request, i.e. if the
    //!  pins of the line request must be configured in output or input modes.
    enum class DigitalPinRequestDirection {
        // The PIN's resource will read from the hardware PIN.
        Input,

        // The PIN's resouce will write to the hardware PIN.
        Output
    };

    //!!
    //! \brief Represents the basic interface of a resource that is
    //!  used to activate/deactivate or read to/from an hardware PIN on
    //!  a board chip.
    struct BoardDigitalPin {
        using offset_type = std::uint32_t;

        virtual ~BoardDigitalPin() noexcept = default;

        //!!
        //! \brief Retrieves the direction of this digital pin.
        //!
        //! \return The direction of the digital pin.
        virtual DigitalPinRequestDirection getDirection() const noexcept = 0;

        //!!
        //! \brief If on the OUTPUT mode, it will activate the hardware's PIN.
        //!
        virtual void activate() noexcept = 0;

        //!!
        //! \brief If on the OUTPUT mode, it will deactivate the hardware's PIN.
        //!
        virtual void deactivate() noexcept = 0;

        //!!
        //! \brief Prints the status of the PIN (line number and direction) to the
        //!  given output stream.
        //!
        virtual void printStatus(std::ostream& ost) const noexcept = 0;
    };

    inline std::ostream& operator<<(std::ostream& ost, const BoardDigitalPin& digPin) noexcept {
        digPin.printStatus(ost);
        return ost;
    }

} // namespace gh_hal::hardware_access
