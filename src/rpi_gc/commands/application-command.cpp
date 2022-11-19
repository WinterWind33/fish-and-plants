// Copyright (C) 2022 Andrea Ballestrazzi
#include <commands/application-command.hpp>

#include <rpi_gc-config-file.hpp> // For printing the version.

// C++ STL
#include <cassert>
#include <algorithm> // for std::find_if

namespace rpi_gc {

    ApplicationCommand::ApplicationCommand(ostream_ref outputStream, option_parser_ref optionParser) noexcept :
        m_outputStream{std::move(outputStream)},
        m_optionParser{std::move(optionParser)} {}

    bool ApplicationCommand::processOptions(const options_vector& options, const non_options_vector& nonOptions,
            const unknown_options_vector& unknowns) noexcept {

        // Here we don't need to do anything because we already have the reference to the option parser that
        // contains all the supported options.
        // Here we simply check that the given parameters are the same as the ones inside the option
        // parser.
        assert(m_optionParser.get().getOptions().size() == options.size());
        assert(m_optionParser.get().getNonOptionArguments().size() == nonOptions.size());
        assert(m_optionParser.get().getUnknownOptions().size() == unknowns.size());

        return true;
    }

    bool ApplicationCommand::processInputOptions(const std::vector<string_type>& options) noexcept {
        m_optionParser.get().parse(options);

        return true;
    }

    bool ApplicationCommand::execute() noexcept {
        // For each option we check if a bivalent command is set as an option.
        bool bCanContinue{true};
        const auto commandOptions{m_optionParser.get().getOptions()};
        for(const auto& option : commandOptions) {
            assert(option != nullptr);
            const bool bIsSet = option->isSet();
            const auto longName = option->getLongName();

            if(bIsSet && m_bivalentCommands.contains(longName)) {
                bCanContinue = m_bivalentCommands.at(option->getLongName()).get().executeAsOption();

                if(!bCanContinue)
                break;
            }
        }

        return bCanContinue;
    }

    void ApplicationCommand::print_help() noexcept {
        // We initially print the help page header.
        print_version();
        m_outputStream.get() << strings::application::COPYRIGHT_DISCLAIMER << std::endl;
        m_outputStream.get() << std::endl;
        m_outputStream.get() << "Developed by " << strings::application::TEAM_NAME << std::endl;
        m_outputStream.get() << std::endl;

        m_optionParser.get().printHelp(m_outputStream.get());
        m_outputStream.get() << std::endl;
    }

    void ApplicationCommand::print_version() noexcept {
        m_outputStream.get() << strings::application::NAME << " - Version " <<
            rpi_gc_VERSION_MAJOR << "." <<
            rpi_gc_VERSION_MINOR << "." <<
            rpi_gc_VERSION_PATCH << std::endl;
    }

    void ApplicationCommand::addBivalentCommand(bivalent_command_ref bivalentCommand) noexcept {
        bivalent_command_ref::type::option_pointer asOption{bivalentCommand.get().getAsOption()};

        assert(asOption != nullptr);
        assert(!m_bivalentCommands.contains(asOption->getLongName()));

        m_optionParser.get().addOption(asOption);
        m_bivalentCommands.emplace(asOption->getLongName(), std::move(bivalentCommand));
    }

} // namespace rpi_gc
