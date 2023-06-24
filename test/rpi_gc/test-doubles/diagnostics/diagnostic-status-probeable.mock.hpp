// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <diagnostics/diagnostic-status-probeable.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

namespace rpi_gc::diagnostics::mocks {

class DiagnosticStatusProbeableMock : public DiagnosticStatusProbeable {
public:
    MOCK_METHOD(void, printDiagnostic, (std::ostream&), (const, noexcept, final));
};

} // namespace rpi_gc::diagnostics::mocks
