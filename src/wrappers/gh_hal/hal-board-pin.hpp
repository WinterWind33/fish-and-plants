// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef HAL_BOARD_PIN_HPP
#define HAL_BOARD_PIN_HPP

#include <gh_hal/hal-definitions.hpp>
#include <gh_hal/hal-digital-output.hpp>

#ifdef USE_CPPGPIO
#include <cppgpio.hpp>
#else
#include <chrono>
#endif // USE_CPPGPIO

// C++ STL
#include <memory>

namespace gh_hal {

    namespace details {
#ifdef USE_CPPGPIO
    using BoardPinNativeType = GPIO::DigitalOut;
#else
    class FakeBoardPinNativeType {
    public:
        constexpr FakeBoardPinNativeType(const PinID pinID) noexcept {}

        constexpr void on() const noexcept {}
        constexpr void off() const noexcept {}
        constexpr void on(const std::chrono::nanoseconds nanoSecs) const noexcept {}
        constexpr void off(const std::chrono::nanoseconds nanoSecs) const noexcept {}
    };

    using BoardPinNativeType = FakeBoardPinNativeType;
#endif // USE_CPPGPIO
    } // namespace details

    //! \brief This class represents a pin on the electronic board.
    //! It can be instatiated with the pin ID and it can turn on or off
    //! the board pin.
    class HALBoardPin : public HALDigitalOutput {
    public:
        using native_type = details::BoardPinNativeType;

        HALBoardPin(const PinID pinID);
        ~HALBoardPin() noexcept override = default;

        void turnOn() noexcept override;
        void turnOff() noexcept override;

    private:
        std::unique_ptr<native_type> m_nativeBoardPin;
    };

} // namespace gh_hal

#endif // !HAL_BOARD_PIN_HPP
