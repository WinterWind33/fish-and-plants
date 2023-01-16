// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef DAILY_CYCLE_AUTOMATIC_WATERING_SYSTEM_HPP
#define DAILY_CYCLE_AUTOMATIC_WATERING_SYSTEM_HPP

#include <automatic-watering/automatic-watering-system.hpp>
#include <automatic-watering/hardware-controllers/watering-system-hardware-controller.hpp>
#include <automatic-watering/time-providers/watering-system-time-provider.hpp>

#include <abort-system/terminable-system.hpp>
#include <abort-system/emergency-stoppable-system.hpp>

#include <gh_log/logger.hpp>

#include <common/types.hpp>

// C++ STL
#include <memory>
#include <thread>
#include <atomic>

namespace rpi_gc::automatic_watering {

    //!!
    //! \brief Represents the automatic watering system that manages the greenhouse
    //!  irrigation on a daily basis using timers to stop and start the greenhouse's hardware.
    class DailyCycleAutomaticWateringSystem :
        public AutomaticWateringSystem,
        public abort_system::TerminableSystem,
        public abort_system::EmergencyStoppableSystem {
    public:
        using logger_pointer = std::shared_ptr<gh_log::Logger>;
        using main_logger_pointer = logger_pointer;
        using user_logger_pointer = logger_pointer;
        using thread_type = std::jthread;
        using hardware_controller_pointer = std::unique_ptr<WateringSystemHardwareController>;
        using time_provider_pointer = std::atomic<std::shared_ptr<WateringSystemTimeProvider>>;

        ~DailyCycleAutomaticWateringSystem() noexcept override = default;

        //!!
        //! \brief Construct a new Daily Cycle Automatic Watering System object using the specified
        //!  main logger and user logger
        //!
        //! \param[in] mainLogger The logger that writes to the application main log file
        //! \param[in] userLog The logger that prints the messages to the preferred user display device (std::cout mainly)
        DailyCycleAutomaticWateringSystem(main_logger_pointer mainLogger, user_logger_pointer userLog,
            hardware_controller_pointer hardwareController, time_provider_pointer::value_type timeProvider) noexcept;

        //!!
        //! \brief Requests the automatic watering system to shutdown if the worker thread is running.
        //!  If the worker thread is running, this calls wait for its completion.
        //! \note Possibly a blocking call.
        void requestShutdown() noexcept override;

        //!!
        //! \brief (FOR NOW) Same as requestShutdown.
        //! \note Possibly a blocking call.
        void emergencyAbort() noexcept override;

        void startAutomaticWatering() noexcept override;

    private:
        main_logger_pointer m_mainLogger{};
        user_logger_pointer m_userLogger{};
        thread_type m_workerThread{};
        hardware_controller_pointer m_hardwareController{};
        time_provider_pointer m_timeProvider{};

        bool m_bIsRunning{};

        void run_automatic_watering(std::stop_token stopToken, main_logger_pointer logger) noexcept;

        void activate_watering_hardware() noexcept;
        void disable_watering_hardware() noexcept;

        [[nodiscard]] static StringType format_log_string(StringViewType message) noexcept;
    };

} // namespace rpi_gc::automatic_watering

#endif // !DAILY_CYCLE_AUTOMATIC_WATERING_SYSTEM_HPP
