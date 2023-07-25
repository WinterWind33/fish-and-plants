// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef TERMINABLE_SYSTEM_MOCK_HPP
#define TERMINABLE_SYSTEM_MOCK_HPP

#include <abort-system/terminable-system.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

namespace rpi_gc::abort_system::mocks {

class TerminableSystemMock : public TerminableSystem {
public:
    ~TerminableSystemMock() noexcept override = default;

    MOCK_METHOD(void, requestShutdown, (), (noexcept, override));
};

} // namespace rpi_gc::abort_system::mocks

#endif // !TERMINABLE_SYSTEM_MOCK_HPP
