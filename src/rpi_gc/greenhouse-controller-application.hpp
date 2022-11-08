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
        using option_parser = gh_cmd::OptionParser<CharType>;
        using ostream_ref = std::reference_wrapper<OutputStream>;
        using istream_ref = std::reference_wrapper<InputStream>;

        GreenhouseControllerApplication(ostream_ref outputStream, istream_ref inputStream, std::unique_ptr<option_parser> optionParser) noexcept;
        ~GreenhouseControllerApplication() noexcept override = default;

        //! \brief Does nothing for now.
        void run() noexcept override;

        bool processInputOptions(const std::int32_t argc, const CharType* const argv[]) noexcept override;

        //! \brief Adds a command with its option parser to the internal command parsers map.
        //!  The command MUST not exist inside the internal pool. The option parser must be valid.
        void addSupportedCommand(StringType commandName, std::unique_ptr<option_parser> commandOptionParser) noexcept;

        //!!
        //! \brief Sets the supported terminal command for this application, i.e. the command that the user
        //!  types when he launches the application along with its options.
        void setApplicationCommand(std::unique_ptr<TerminalCommandType> applicationCommand) noexcept;

    private:
        ostream_ref m_outputStream;
        istream_ref m_inputStream;

        std::map<StringType, std::unique_ptr<option_parser>> m_commandsOptionParsers{};
        std::unique_ptr<option_parser> m_terminalInputOptionParser{};
        std::unique_ptr<TerminalCommandType> m_applicationCommand{};

        bool m_bCanApplicationCommandExecute{};

        void print_app_header() noexcept;
        void teardown() noexcept;
        static StringType create_version_string() noexcept;
    };

} // namespace rpi_gc

#endif // !GREENHOUSE_CONTROLLER_APPLICATION_HPP
