// Copyright (c) 2022 Andrea Ballestrazzi
#include <automatic-watering/daily-cycle-automatic-watering-system.hpp>

// C++ STL
#include <cassert>

namespace rpi_gc::automatic_watering {

    DailyCycleAutomaticWateringSystem::DailyCycleAutomaticWateringSystem(logger_pointer logger) noexcept :
        m_logger{std::move(logger)} {
        assert(m_logger != nullptr);
    }

    void DailyCycleAutomaticWateringSystem::requestShutdown() noexcept {
        m_logger->logInfo("Shutdown requested for the automatic watering system.");

        [[maybe_unused]] const bool bRequestStopSucceded{m_workerThread.request_stop()};
        assert(bRequestStopSucceded);
    }

    void DailyCycleAutomaticWateringSystem::emergencyAbort() noexcept {
        m_logger->logWarning("[AUTOMATIC WATERING SYSTEM]: Emergency abort requested.");
    }

    void DailyCycleAutomaticWateringSystem::startAutomaticWatering() noexcept {
        m_logger->logInfo("Starting automatic watering system...");

        m_workerThread = thread_type{[this](std::stop_token stopToken, logger_pointer logger){
            run_automatic_watering(std::move(stopToken), std::move(logger));
        }, m_logger};
    }

    void DailyCycleAutomaticWateringSystem::run_automatic_watering(std::stop_token stopToken, logger_pointer logger) noexcept {
        logger->logInfo("[Automatic Watering Thread] => Starting job.");
        if(stopToken.stop_requested()) {
            logger->logInfo("[Automatic Watering Thread] => Stop requested.");
        }

        logger->logInfo("[Automatic Watering Thread] => Ending job.");
    }

} // namespace rpi_gc::automatic_watering
