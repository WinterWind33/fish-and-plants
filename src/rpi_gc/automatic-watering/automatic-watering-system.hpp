// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef AUTOMATIC_WATERING_SYSTEM_HPP
#define AUTOMATIC_WATERING_SYSTEM_HPP

#include <abort-system/terminable-system.hpp>
#include <abort-system/emergency-stoppable-system.hpp>

#include <gh_log/logger.hpp>

// C++ STL
#include <memory>

namespace rpi_gc::automatic_watering {

class AutomaticWateringSystem : public abort_system::TerminableSystem, public abort_system::EmergencyStoppableSystem {
public:
    using logger_pointer = std::shared_ptr<gh_log::Logger>;

    ~AutomaticWateringSystem() noexcept override = default;

    AutomaticWateringSystem(logger_pointer logger) noexcept;

    void requestShutdown() noexcept override;
    void emergencyAbort() noexcept override;

private:
    logger_pointer m_logger{};
};

} // namespace rpi_gc::automatic_watering

#endif // !AUTOMATIC_WATERING_SYSTEM_HPP
