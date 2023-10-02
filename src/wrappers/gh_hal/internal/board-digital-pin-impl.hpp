// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gh_hal/hardware-access/board-digital-pin.hpp>

// Backends
#ifdef USE_LIBGPIOD
#include <gh_hal/backends/libgpiod/chip-api.hpp>
#endif // USE_LIBGPIOD

#include <gh_hal/backends/simulated/simulated-digital-board-pin.hpp>

// C++ STL
#include <functional> // for std::reference_wrapper

namespace gh_hal::internal {

class BoardDigitalPinImpl final : public hardware_access::BoardDigitalPin {
public:
#ifdef USE_LIBGPIOD
    using backend_type = backends::libgpiod_impl::NativeLineRequestType;
#else
    using backend_type = const backends::simulated::DigitalBoardPin;
#endif // USE_LIBGPIOD

    using backend_type_reference = std::reference_wrapper<backend_type>;

    explicit BoardDigitalPinImpl(
        const hardware_access::BoardDigitalPin::offset_type offsetValue,
        const hardware_access::DigitalPinRequestDirection direction,
        backend_type_reference backImpl,
        const hardware_access::DigitalOutPinActivationState activationState) noexcept;

    [[nodiscard]] hardware_access::DigitalPinRequestDirection getDirection()
        const noexcept override {
        return m_direction;
    }

    void activate() noexcept override;
    void deactivate() noexcept override;

    void printStatus(std::ostream& ostream) const noexcept override;

    [[nodiscard]] inline offset_type getOffset() const noexcept override {
        return m_offset;
    }

    [[nodiscard]] inline hardware_access::DigitalOutPinActivationState getActivationState()
        const noexcept override {
        return m_activationState;
    }

private:
    const hardware_access::BoardDigitalPin::offset_type m_offset{};
    const hardware_access::DigitalPinRequestDirection m_direction{};
    const hardware_access::DigitalOutPinActivationState m_activationState{};
    backend_type_reference m_backendReference;
};

} // namespace gh_hal::internal
