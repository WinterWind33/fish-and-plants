// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/internal/board-digital-pin-impl.hpp>

#include <gh_hal/internal/line-request.hpp>

namespace gh_hal::internal {

    namespace details {
#ifdef USE_LIBGPIOD
        void activateImpl(BoardDigitalPinImpl::backend_type_reference lineRequest,
            const hardware_access::BoardDigitalPin::offset_type offsetValue) noexcept {
            lineRequest.get().set_value(::gpiod::line::offset{offsetValue}, ::gpiod::line::value::ACTIVE);
        }

        void deactivateImpl(BoardDigitalPinImpl::backend_type_reference lineRequest,
            const hardware_access::BoardDigitalPin::offset_type offsetValue) noexcept {
            lineRequest.get().set_value(::gpiod::line::offset{offsetValue}, ::gpiod::line::value::INACTIVE);
        }
#else
        constexpr void activateImpl(BoardDigitalPinImpl::backend_type_reference /*lineRequest*/,
            const hardware_access::BoardDigitalPin::offset_type /*offsetValue*/) noexcept {}
        constexpr void deactivateImpl(BoardDigitalPinImpl::backend_type_reference /*lineRequest*/,
            const hardware_access::BoardDigitalPin::offset_type /*offsetValue*/) noexcept {}
#endif // USE_LIBGPIOD
    } // namespace details

    BoardDigitalPinImpl::BoardDigitalPinImpl(
        const hardware_access::BoardDigitalPin::offset_type offsetValue,
        const hardware_access::DigitalPinRequestDirection direction,
        backend_type_reference backImpl) noexcept :
        m_offset{offsetValue},
        m_direction{direction},
        m_backendReference{backImpl} {}

    void BoardDigitalPinImpl::activate() noexcept {
        details::activateImpl(m_backendReference, m_offset);
    }

    void BoardDigitalPinImpl::deactivate() noexcept {
        details::deactivateImpl(m_backendReference, m_offset);
    }

} // namespace gh_hal::internal
