// Copyright (c) 2022 Andrea Ballestrazzi
#include <automatic-watering/automatic-watering-system.hpp>

// C++ STL
#include <cassert>
#include <thread>

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

    void AutomaticWateringSystem::startAutomaticWatering() noexcept {
        m_logger->logInfo("Starting automatic watering system...");

        std::thread automaticWateringThread{[this](logger_pointer logger){
            run_automatic_watering(logger);
        }, m_logger};
    }

    void AutomaticWateringSystem::run_automatic_watering(logger_pointer logger) noexcept {
        logger->logInfo("[Automatic Watering Thread] => Starting job.");

        logger->logInfo("[Automatic Watering Thread] => Ending job.");
    }

} // namespace rpi_gc::automatic_watering
