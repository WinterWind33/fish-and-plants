// Copyright (c) 2022 Andrea Ballestrazzi
#include <automatic-watering/automatic-watering-system.hpp>

// C++ STL
#include <cassert>

namespace rpi_gc::automatic_watering {

    AutomaticWateringSystem::AutomaticWateringSystem(logger_pointer logger) noexcept :
        m_logger{std::move(logger)} {
        assert(m_logger != nullptr);
    }

    void AutomaticWateringSystem::requestShutdown() noexcept {
        m_logger->logInfo("Shutdown requested for the automatic watering system.");
    }

    void AutomaticWateringSystem::emergencyAbort() noexcept {
        m_logger->logWarning("[AUTOMATIC WATERING SYSTEM]: Emergency abort requested.");
    }

} // namespace rpi_gc::automatic_watering
