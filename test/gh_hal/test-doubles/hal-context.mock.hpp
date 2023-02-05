// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef HAL_CONTEXT_MOCK_HPP
#define HAL_CONTEXT_MOCK_HPP

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

#include <hal-context.hpp>

namespace gh_hal::mocks {

class HALContextMock : public gh_hal::HALContext {
public:
    ~HALContextMock() noexcept override = default;

    MOCK_METHOD((const logger_pointer&), getLogger, (), (const, noexcept, override));
    MOCK_METHOD(bool, isSimulation, (), (const, noexcept, override));
};

} // namespace mocks

#endif // !HAL_CONTEXT_MOCK_HPP
