// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef DAILY_CYCLE_AUTOMATIC_WATERING_SYSTEM_HPP
#define DAILY_CYCLE_AUTOMATIC_WATERING_SYSTEM_HPP

#include <automatic-watering/automatic-watering-system.hpp>

#include <abort-system/terminable-system.hpp>
#include <abort-system/emergency-stoppable-system.hpp>

#include <gh_log/logger.hpp>

// C++ STL
#include <memory>

namespace rpi_gc::automatic_watering {

class DailyCycleAutomaticWateringSystem : public abort_system::TerminableSystem, public abort_system::EmergencyStoppableSystem {
public:
    using logger_pointer = std::shared_ptr<gh_log::Logger>;

    ~DailyCycleAutomaticWateringSystem() noexcept override = default;

    DailyCycleAutomaticWateringSystem(logger_pointer logger) noexcept;

    void requestShutdown() noexcept override;
    void emergencyAbort() noexcept override;

    void startAutomaticWatering() noexcept;

private:
    logger_pointer m_logger{};

    void run_automatic_watering(logger_pointer logger) noexcept;
};

} // namespace rpi_gc::automatic_watering

#endif // !DAILY_CYCLE_AUTOMATIC_WATERING_SYSTEM_HPP
