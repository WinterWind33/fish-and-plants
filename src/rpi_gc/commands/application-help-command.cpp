// Copyright (C) 2022 Andrea Ballestrazzi
#include <commands/application-help-command.hpp>

#include <rpi_gc-config-file.hpp> // For printing the version.

// C++ STL
#include <cassert>
#include <string_view>

namespace rpi_gc {

    ApplicationHelpCommand::ApplicationHelpCommand(ostream_ref outputStream, option_parser_ref optionParser) noexcept :
        m_outputStream{std::move(outputStream)},
        m_optionParser{std::move(optionParser)} {}

    bool ApplicationHelpCommand::processOptions(const options_vector& options, const non_options_vector& nonOptions,
            const unknown_options_vector& unknowns) noexcept {
        // Note: this command shouldn't have any option. There isn't something like help --help.
        // So here we should have only unknowns options. If there are valid options then the developer
        // made some mistakes.
        assert(options.empty());

        return true;
    }

    bool ApplicationHelpCommand::execute() noexcept {
        constexpr std::string_view APPLICATION_NAME{"Greenhouse Controller"};
        constexpr std::string_view APPLICATION_LICENSE{"Copyright (c) 2022 Andrea Ballestrazzi"};
        constexpr std::string_view TEAM_NAME{"Fish&Plants Team"};

        // We initially print the help page header.
        m_outputStream.get() << APPLICATION_NAME << " - Version " <<
            rpi_gc_VERSION_MAJOR << "." <<
            rpi_gc_VERSION_MINOR << "." <<
            rpi_gc_VERSION_PATCH << std::endl;
        m_outputStream.get() << APPLICATION_LICENSE << std::endl;
        m_outputStream.get() << std::endl;
        m_outputStream.get() << "Developed by " << TEAM_NAME << std::endl;
        m_outputStream.get() << std::endl;

        return true;
    }

} // namespace rpi_gc
