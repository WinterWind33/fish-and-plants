// Copyright (C) 2023 Andrea Ballestrazzi
#ifndef GREENHOUSE_CONTROLLER_APPLICATION_HPP
#define GREENHOUSE_CONTROLLER_APPLICATION_HPP

#include <common/types.hpp>
#include <application/application.hpp>
#include <commands/terminal-command.hpp>
#include <gc-project/project-controller.hpp>

// Abort system
#include <abort-system/terminable-system.hpp>
#include <abort-system/emergency-stoppable-system.hpp>

// Wrappers
#include <gh_cmd/gh_cmd.hpp>
#include <gh_log/logger.hpp>

// C++ STL
#include <functional>
#include <memory>
#include <map>
#include <vector>

namespace rpi_gc {

    //! \brief Represents the main greenhouse controller application.
    class GreenhouseControllerApplication : public Application {
    public:
        using ostream_ref = std::reference_wrapper<OutputStream>;
        using istream_ref = std::reference_wrapper<InputStream>;
        using logger_pointer = std::shared_ptr<gh_log::Logger>;

        GreenhouseControllerApplication(ostream_ref outputStream, istream_ref inputStream, logger_pointer mainLogger, gc_project::ProjectController& projectController) noexcept;

        //! \brief Runs the main greenhousse controller application.
        void run() noexcept override;

        bool processInputOptions(const std::vector<std::string>& inputArgs) noexcept override;

        //! \brief Adds a command that can be executed during the application loop.
        //!  The command MUST not exist inside the internal pool.
        void addSupportedCommand(std::unique_ptr<TerminalCommandType> command) noexcept;

        //!!
        //! \brief Sets the supported terminal command for this application, i.e. the command that the user
        //!  types when he launches the application along with its options.
        void setApplicationCommand(std::unique_ptr<TerminalCommandType> applicationCommand) noexcept;

        void addEmergencyStoppableSystem(std::shared_ptr<abort_system::EmergencyStoppableSystem> system) noexcept;
        void addTerminableSystem(std::shared_ptr<abort_system::TerminableSystem> system) noexcept;

    private:
        ostream_ref m_outputStream;
        istream_ref m_inputStream;
        logger_pointer m_mainLogger{};
        std::reference_wrapper<gc_project::ProjectController> m_projectController;

        std::map<StringType, std::unique_ptr<TerminalCommandType>> m_commands{};
        std::unique_ptr<TerminalCommandType> m_applicationCommand{};

        std::vector<std::shared_ptr<abort_system::EmergencyStoppableSystem>> m_emergencyStoppableSystems{};
        std::vector<std::shared_ptr<abort_system::TerminableSystem>> m_terminableSystems{};

        bool m_bCanApplicationCommandExecute{};

        void print_app_header() noexcept;
        void teardown() noexcept;

        void print_project_path() const;

        static StringType create_version_string() noexcept;
    };

} // namespace rpi_gc

#endif // !GREENHOUSE_CONTROLLER_APPLICATION_HPP
