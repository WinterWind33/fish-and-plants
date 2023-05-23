// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/help-command.hpp>
#include <version/version-numbers.hpp>

// C++ STL
#include <cassert>
#include <iomanip>

namespace rpi_gc {

    HelpCommand::HelpCommand(ostream_ref outputStream, std::vector<terminal_command_const_ref> commands) noexcept :
        m_outputStream{std::move(outputStream)},
        m_asOption{std::make_shared<gh_cmd::Switch<char_type>>('h', "help", "Displays this help page.")},
        m_terminalCommands{std::move(commands)} {}

    bool HelpCommand::execute() noexcept {
        print_header();
        m_outputStream.get() << std::endl;
        print_description();
        m_outputStream.get() << std::endl;

        m_outputStream.get() << "## COMMANDS" << std::endl;
        m_outputStream.get() << "Here is a list of all the implemented commands: " << std::endl;

        m_outputStream.get() << "\t" << "- help : displays this page;" << std::endl;
        m_outputStream.get() << "\t" << "- exit : exits the application performing a resource soft-destruction." << std::endl;
        m_outputStream.get() << "\t" << "\tThis means that the application waits the resources and controller to be release before exiting;" << std::endl;

        for(const auto& terminalCommand : m_terminalCommands) {
            m_outputStream.get() << "\t" << "- " << terminalCommand.get().getName() << std::endl;
        }

        m_outputStream.get() << std::endl;
        m_outputStream.get() << "If you want to know more about these commands just type the command followed by \'help\':" << std::endl;
        m_outputStream.get() << "Example: " << std::endl;
        m_outputStream.get() << "$ auto-watering --help" << std::endl;


        return true;
    }

    bool HelpCommand::executeAsOption() noexcept {
        print_header();
        m_outputStream.get() << m_applicationHelp << std::endl;

        // We don't want to continue with the application if the user
        // requrested the application help.
        return false;
    }

    void HelpCommand::print_header() noexcept {
        ostream_ref::type& out = m_outputStream.get();

        out << strings::application::NAME << " - Version " << HelpCommand::get_app_version() << std::endl;
        out << "Developed by " << strings::application::TEAM_NAME << std::endl;
    }

    void HelpCommand::printHelp(help_ostream_type outputStream) const noexcept {
        outputStream.get() << "[NAME]" << std::endl;
        outputStream.get() << "\t" << strings::commands::HELP << " - Prints the help page for the application.";
    }

    void HelpCommand::print_description() noexcept {
        ostream_ref::type& out = m_outputStream.get();

        out << "!!! WARNING !!!" << std::endl;
        out << "The features implemented inside this software are experimental and can be highly unstable! "
            << "USE WITH CAUTION!" << std::endl;
        out << "THIS SOFTWARE DOESN\'T COME WITH ANY WARRANTIES." << std::endl;
        out << std::endl;
        out << "Greenhouse Controller is an experimental application that is capable to run on chips with GPIO PINs that support Linux OS. "
            << "RPI_PG serves as an aeroponic greenhouse controller." << std::endl;
        out << std::endl;
        out << "## DESCRIPTION" << std::endl;
        out << "The software allows the user to run automatic irrigation inside the greenhouse in Daily Cycle mode. "
            << "This means that the automatic irrigation is activated for X time and deactivated for Y time. The timings "
            << "are fully customizable through the \'auto-watering\' command options. The software handles the activation "
            << "and deactivation of the PINs that control the hardware pump and valves." << std::endl;
        out << std::endl;
        out << "### USAGE" << std::endl;
        out << "The application has a terminal-like interface so the user have to interact with the terminal prompt to "
            << "control the automatic irrigation. Below you can find a list with all of the implemented commands." << std::endl;
    }

    std::string HelpCommand::get_app_version() noexcept {
        std::ostringstream outputStream{};
        outputStream <<
            version::RPI_GC_VERSION_MAJOR << '.' << version::RPI_GC_VERSION_MINOR << '.' << version::RPI_GC_VERSION_PATCH;

        if constexpr (!version::RPI_GC_VERSION_RC_NAME.empty())
            outputStream << "-" << version::RPI_GC_VERSION_RC_NAME;

        return outputStream.str();
    }

} // namespace rpi_gc
