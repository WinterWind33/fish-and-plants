// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef DAILY_CYCLE_AUTOMATIC_WATERING_SYSTEM_HPP
#define DAILY_CYCLE_AUTOMATIC_WATERING_SYSTEM_HPP

#include <automatic-watering/automatic-watering-system.hpp>

#include <abort-system/terminable-system.hpp>
#include <abort-system/emergency-stoppable-system.hpp>

#include <gh_log/logger.hpp>

#include <common/types.hpp>

// C++ STL
#include <memory>
#include <thread>

namespace rpi_gc::automatic_watering {

    class DailyCycleAutomaticWateringSystem :
        public AutomaticWateringSystem,
        public abort_system::TerminableSystem,
        public abort_system::EmergencyStoppableSystem {
    public:
        using logger_pointer = std::shared_ptr<gh_log::Logger>;
        using thread_type = std::jthread;

        ~DailyCycleAutomaticWateringSystem() noexcept override = default;

        DailyCycleAutomaticWateringSystem(logger_pointer mainLogger, logger_pointer userLog) noexcept;

        void requestShutdown() noexcept override;
        void emergencyAbort() noexcept override;

        void startAutomaticWatering() noexcept override;

    private:
        logger_pointer m_mainLogger{};
        logger_pointer m_userLogger{};
        thread_type m_workerThread{};

        void run_automatic_watering(std::stop_token stopToken, logger_pointer logger) noexcept;

        [[nodiscard]] static StringType format_log_string(StringViewType message) noexcept;
    };

} // namespace rpi_gc::automatic_watering

#endif // !DAILY_CYCLE_AUTOMATIC_WATERING_SYSTEM_HPP
