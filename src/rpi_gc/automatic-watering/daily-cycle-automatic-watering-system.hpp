// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#include <automatic-watering/automatic-watering-system.hpp>
#include <automatic-watering/hardware-controllers/watering-system-hardware-controller.hpp>
#include <automatic-watering/time-providers/watering-system-time-provider.hpp>

#include <diagnostics/diagnostic-status-probeable.hpp>
#include <abort-system/terminable-system.hpp>
#include <abort-system/emergency-stoppable-system.hpp>

#include <gh_log/logger.hpp>

#include <common/types.hpp>

// C++ STL
#include <cstdint>
#include <memory>
#include <thread>
#include <atomic>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <tuple>

namespace rpi_gc::automatic_watering {

    //!!
    //! \brief Represents a basic FSM for the AWS.
    //!  TODO: this need to be refactored in the future.
    enum class EDailyCycleAWSState {
        Disabled,
        Idling,
        Irrigating,
        TearingDown
    };

    //!!
    //! \brief Represents the automatic watering system that manages the greenhouse
    //!  irrigation on a daily basis using timers to stop and start the greenhouse's hardware.
    class DailyCycleAutomaticWateringSystem final :
        public AutomaticWateringSystem,
        public abort_system::TerminableSystem,
        public abort_system::EmergencyStoppableSystem,
        public diagnostics::DiagnosticStatusProbeable {
    public:
        using logger_pointer = std::shared_ptr<gh_log::Logger>;
        using main_logger_pointer = logger_pointer;
        using user_logger_pointer = logger_pointer;
        using thread_type = std::jthread;
        using hardware_controller_pointer = std::atomic<WateringSystemHardwareController*>;
        using hardware_controller_atomic_ref = std::reference_wrapper<hardware_controller_pointer>;
        using time_provider_pointer = std::atomic<WateringSystemTimeProvider*>;
        using time_provider_atomic_ref = std::reference_wrapper<time_provider_pointer>;
        using stop_event_listener = std::condition_variable;
        using stop_event_mutex = std::mutex;
        using hardware_access_mutex_reference = std::reference_wrapper<std::mutex>;

        ~DailyCycleAutomaticWateringSystem() noexcept override = default;

        //!!
        //! \brief Construct a new Daily Cycle Automatic Watering System object using the specified
        //!  main logger and user logger
        //!
        //! \param[in] mainLogger The logger that writes to the application main log file
        //! \param[in] userLog The logger that prints the messages to the preferred user display device (std::cout mainly)
        DailyCycleAutomaticWateringSystem(hardware_access_mutex_reference hardwareMutex, main_logger_pointer mainLogger, user_logger_pointer userLog,
            hardware_controller_atomic_ref hardwareController, time_provider_atomic_ref timeProvider) noexcept;

        //!!
        //! \brief Requests the automatic watering system to shutdown if the worker thread is running.
        //!  If the worker thread is running, this calls wait for its completion.
        //! \note Possibly a blocking call.
        void requestShutdown() noexcept override;

        //!!
        //! \brief (FOR NOW) Same as requestShutdown.
        //! \note Possibly a blocking call.
        void emergencyAbort() noexcept override;

        //!!
        //! \brief Starts the automatic watering system job in a separate worker thread.
        //!
        //! \note The automatic watering system won't start if both the water pump and the water valve
        //!  are disabled before this call. In that case, a log is printed to the main and user loggers.
        //!
        void startAutomaticWatering() noexcept override;

        //!!
        //! \brief Set the water pump enabled state.
        //!
        //! \param bEnabled True if the water pump need to be enabled.
        void setWaterPumpEnabled(const bool bEnabled) noexcept;

        //!!
        //! \brief Set the water valve enabled state.
        //!
        //! \param bEnabled True if the water valve need to be enabled.
        void setWaterValveEnabled(const bool bEnabled) noexcept;

        void printDiagnostic(std::ostream& ost) const noexcept override;

        [[nodiscard]]
        inline bool isRunning() const noexcept { return (m_state.load() != EDailyCycleAWSState::Disabled); }

    private:
        main_logger_pointer m_mainLogger{};
        user_logger_pointer m_userLogger{};
        thread_type m_workerThread{};
        hardware_controller_atomic_ref m_hardwareController;
        time_provider_atomic_ref m_timeProvider;
        stop_event_listener m_stopListener{};
        stop_event_mutex m_stopMutex{};
        hardware_access_mutex_reference m_hardwareAccessMutex;
        std::atomic_bool m_bWaterPumpEnabled{true};
        std::atomic_bool m_bWaterValveEnabled{true};
        std::atomic<EDailyCycleAWSState> m_state{EDailyCycleAWSState::Disabled};
        std::atomic<std::uint64_t> m_cyclesCounter{};

        void run_automatic_watering(std::stop_token stopToken, const main_logger_pointer& logger) noexcept;

        // Updates the status of the valve and pump devices according to the given
        // initial status. If the initial status of a device was "enabled" then this will
        // set it to "disable" and it will deactivate the corresponding hardware PIN.
        [[nodiscard]]
        std::pair<bool, bool> update_devices_status(const bool bWasValveEnabled, const bool bWasPumpEnabled) noexcept;

        void activate_watering_hardware() noexcept;
        void disable_watering_hardware() noexcept;

        [[nodiscard]] static StringType format_log_string(StringViewType message) noexcept;
    };

} // namespace rpi_gc::automatic_watering
