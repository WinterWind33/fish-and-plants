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
                bCanContinue = m_bivalentCommands.at(longName).get().executeAsOption();

                if(!bCanContinue)
                    break;
            }
        }

        return bCanContinue;
    }

    void ApplicationCommand::addBivalentCommand(bivalent_command_ref bivalentCommand) noexcept {
        bivalent_command_ref::type::option_pointer asOption{bivalentCommand.get().getAsOption()};

        assert(asOption != nullptr);
        assert(!m_bivalentCommands.contains(asOption->getLongName()));

        m_optionParser.get().addOption(asOption);
        m_bivalentCommands.emplace(asOption->getLongName(), std::move(bivalentCommand));
    }

} // namespace rpi_gc
