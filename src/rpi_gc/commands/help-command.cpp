// Copyright (c) 2022 Andrea Ballestrazzi
#include <commands/help-command.hpp>
#include <rpi_gc-config-file.hpp>

// C++ STL
#include <cassert>
#include <string_view>

namespace rpi_gc {

    HelpCommand::HelpCommand(ostream_ref outputStream, option_parsers_map optionParsers) noexcept :
        m_outputStream{std::move(outputStream)},
        m_optionParsers{std::move(optionParsers)} {}

    bool HelpCommand::processOptions(const options_vector& options,
            const non_options_vector& nonOptions, const unknown_options_vector& unknown) noexcept {
        // The help command shouldn't have any option.
        assert(options.empty());

        return true;
    }

    bool HelpCommand::execute() noexcept {
        print_header();
        m_outputStream.get() << std::endl;
        print_description();
        m_outputStream.get() << std::endl;

        // For every option parser we print the help page as it should contain both
        // the command description and its options as well.
        m_outputStream.get() << "===> IMPLEMENTED COMMANDS <===" << std::endl;
        m_outputStream.get() << std::endl;
        for(const auto& optionParser : m_optionParsers) {
            m_outputStream.get() << "> " << std::get<0>(optionParser) << std::endl;
            m_outputStream.get() << "Description: ";
            std::get<1>(optionParser).get().printHelp(m_outputStream.get());

            m_outputStream.get() << std::endl;
        }

        // We also print the exit command that is not listed with the option parser.
        m_outputStream.get() << "> " << "exit" << std::endl;
        m_outputStream.get() << "Description: exits the application releasing all the resources (soft exit):" << std::endl;

        return true;
    }

    void HelpCommand::print_header() noexcept {
        constexpr std::string_view APPLICATION_NAME{"Greenhouse Controller"};
        constexpr std::string_view TEAM_CREDIT{"Fish&Plants Team"};
        ostream_ref::type& out = m_outputStream.get();

        out << APPLICATION_NAME << " - Version " << rpi_gc_VERSION_MAJOR << '.' << rpi_gc_VERSION_MINOR << '.' << rpi_gc_VERSION_PATCH << '.' << std::endl;
        out << "Developed by " << TEAM_CREDIT << std::endl;
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
            << "to the greenhouse\'s plants, and provide actions based on these values. Example: if the humidity"
            << "is below a certain temperature value, the software informs the hardware to provide water to the"
            << "plants." << std::endl;
        out << std::endl;
        out << "===> USAGE <===" << std::endl;
        out << "In order to start some controller job you need to use the application terminal and provide commands "
            << "to the terminal prompt. The commands implemented so far are available below." << std::endl;
    }

} // namespace rpi_gc
