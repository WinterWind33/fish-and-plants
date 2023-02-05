// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/hal-board-pin.hpp>

namespace gh_hal {

    HALBoardPin::HALBoardPin(const PinID pinID) :
        m_nativeBoardPin{std::make_unique<native_type>(pinID)} {}

    void HALBoardPin::turnOn() noexcept {
        m_nativeBoardPin->on();
    }

    void HALBoardPin::turnOff() noexcept {
        m_nativeBoardPin->off();
    }

} // namespace gh_hal
