// Copyright (C) 2022 Andrea Ballestrazzi
#ifndef GREENHOUSE_CONTROLLER_APPLICATION_HPP
#define GREENHOUSE_CONTROLLER_APPLICATION_HPP

#include <common/types.hpp>
#include <application/application.hpp>
#include <commands/terminal-command.hpp>

// Wrappers
#include <gh_cmd/gh_cmd.hpp>

// C++ STL
#include <functional>
#include <memory>
#include <map>

namespace rpi_gc {

    //! \brief Represents the main greenhouse controller application.
    class GreenhouseControllerApplication : public Application {
    public:
        using ostream_ref = std::reference_wrapper<OutputStream>;
        using istream_ref = std::reference_wrapper<InputStream>;

        GreenhouseControllerApplication(ostream_ref outputStream, istream_ref inputStream) noexcept;
        ~GreenhouseControllerApplication() noexcept override = default;

        //! \brief Does nothing for now.
        void run() noexcept override;

        bool processInputOptions(const std::int32_t argc, const CharType* const argv[]) noexcept override;

        //! \brief Adds a command that can be executed during the application loop.
        //!  The command MUST not exist inside the internal pool.
        void addSupportedCommand(std::unique_ptr<TerminalCommandType> command) noexcept;

        //!!
        //! \brief Sets the supported terminal command for this application, i.e. the command that the user
        //!  types when he launches the application along with its options.
        void setApplicationCommand(std::unique_ptr<TerminalCommandType> applicationCommand) noexcept;

    private:
        ostream_ref m_outputStream;
        istream_ref m_inputStream;

        std::map<StringType, std::unique_ptr<TerminalCommandType>> m_commands{};
        std::unique_ptr<TerminalCommandType> m_applicationCommand{};

        bool m_bCanApplicationCommandExecute{};

        void print_app_header() noexcept;
        void teardown() noexcept;
        static StringType create_version_string() noexcept;
    };

} // namespace rpi_gc

#endif // !GREENHOUSE_CONTROLLER_APPLICATION_HPP
