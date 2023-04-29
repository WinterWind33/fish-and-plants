// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <gh_hal/hardware-access/board-chip.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

namespace gh_hal::hardware_access::mocks {

    class BoardChipMock : public BoardChip {
    public:
        MOCK_METHOD((std::unique_ptr<BoardDigitalPin>), requestDigitalPin, (std::string, BoardDigitalPin::offset_type, const DigitalPinRequestDirection), (noexcept, final));
        MOCK_METHOD((std::vector<std::unique_ptr<BoardDigitalPin>>), requestDigitalPinPool, (std::string, std::vector<BoardDigitalPin::offset_type>, const DigitalPinRequestDirection), (noexcept, final));
        MOCK_METHOD(bool, releaseRequest, (std::vector<BoardDigitalPin::offset_type>), (noexcept, final));
    };

} // namespace gh_hal::hardware_access::mocks
