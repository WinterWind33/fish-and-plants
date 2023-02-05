// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/help-command.hpp>
#include <rpi_gc-config-file.hpp>

// C++ STL
#include <cassert>
#include <iomanip>

namespace rpi_gc {

    HelpCommand::HelpCommand(ostream_ref outputStream, std::vector<TerminalCommandType*> commands) noexcept :
        m_outputStream{std::move(outputStream)},
        m_asOption{std::make_shared<gh_cmd::Switch<char_type>>('h', "help", "Displays this help page.")} {

        // Here we need to retrieve the help page of all the input commands.
        for(const auto& command : commands) {
            OutputStringStream formatStream{};
            command->printHelp(formatStream);
            m_commandsHelpPages.push_back(formatStream.str());
        }
    }

    bool HelpCommand::execute() noexcept {
        print_header();
        m_outputStream.get() << std::endl;
        print_description();
        m_outputStream.get() << std::endl;

        m_outputStream.get() << "===> COMMANDS <===" << std::endl;
        m_outputStream.get() << std::endl;

        // Time to print the help page of this command. We print it first
        // as the help is the most important command.
        printHelp(m_outputStream.get());
        m_outputStream.get() << std::endl;
        m_outputStream.get() << std::setfill('-') << std::setw(80);
        m_outputStream.get() << '-' << std::setfill(' ') << std::endl;

        // Next we print the "exit" command help page so it doesn't get buried
        // under the others
        m_outputStream.get() << "[NAME]" << std::endl;
        m_outputStream.get() << "\t" << strings::commands::EXIT << " - Stops the application execution and exits" << std::endl;
        m_outputStream.get() << "[DESCRIPTION]" << std::endl;
        m_outputStream.get() << "\tStops the application execution releasing the resources and waiting for them (soft exit)." << std::endl;
        m_outputStream.get() << std::setfill('-') << std::setw(80);
        m_outputStream.get() << '-' << std::setfill(' ') << std::endl;

        for(const auto& helpPage : m_commandsHelpPages) {
            m_outputStream.get() << helpPage << std::endl;
            m_outputStream.get() << std::setfill('-') << std::setw(80);
            m_outputStream.get() << '-' << std::setfill(' ') << std::endl;
        }

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

        out << strings::application::NAME << " - Version " << rpi_gc_VERSION_MAJOR << '.' << rpi_gc_VERSION_MINOR << '.' << rpi_gc_VERSION_PATCH << '.' << std::endl;
        out << "Developed by " << strings::application::TEAM_NAME << std::endl;
    }

    void HelpCommand::printHelp(help_ostream_type outputStream) const noexcept {
        outputStream.get() << "[NAME]" << std::endl;
        outputStream.get() << "\t" << strings::commands::HELP << " - Prints the help page for the application.";
    }

    void HelpCommand::print_description() noexcept {
        ostream_ref::type& out = m_outputStream.get();

        out << "=======================> WARNING <=======================" << std::endl;
        out << "The features implemented inside this software are experimental and can be highly unstable! "
            << "USE WITH CAUTION!" << std::endl;
        out << "THIS SOFTWARE DOESN\'T COME WITH ANY WARRANTIES." << std::endl;
        out << "=========================================================" << std::endl;
        out << std::endl;
        out << "Greenhouse Controller is an experimental application that runs on a Raspberry PI or other hardware platforms "
            << "that serve as aeroponic greenhouses\' automatic controllers." << std::endl;
        out << std::endl;
        out << "===> DESCRIPTION <===" << std::endl;
        out << "The main scope of this software is to read values from the external sensors, that are attached "
            << "to the greenhouse\'s plants, and provide actions based on these values. Example: if the humidity "
            << "is below a certain temperature value, the software informs the hardware to provide water to the "
            << "plants." << std::endl;
        out << std::endl;
        out << "===> USAGE <===" << std::endl;
        out << "In order to start some controller job you need to use the application terminal and provide commands "
            << "to the terminal prompt. The commands implemented so far are available below." << std::endl;
    }

} // namespace rpi_gc
