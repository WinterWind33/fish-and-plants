// Copyright (C) 2023 Andrea Ballestrazzi
#include <commands/application-command.hpp>

// C++ STL
#include <algorithm> // for std::find_if
#include <cassert>

namespace rpi_gc {

ApplicationCommand::ApplicationCommand(ostream_ref outputStream,
                                       option_parser_ref optionParser) noexcept
    : m_outputStream{outputStream},
      m_optionParser{optionParser} {}

bool ApplicationCommand::processInputOptions(const std::vector<string_type>& options) {
    m_optionParser.get().parse(options);

    return true;
}

bool ApplicationCommand::execute() noexcept {
    using OptionPointer = std::shared_ptr<const option_type>;

    // For each option we check if a bivalent command is set as an option.
    // For now we only have bivalent commands
    bool bCanContinue{true};
    const auto commandOptions{m_optionParser.get().getOptions()};

    // The help option has the highest priority throughout the supported
    // commands. If the help option is set we need to execute it and return
    // so other options are not executed.
    auto endIterator = commandOptions.cend();
    auto helpIt =
        std::find_if(commandOptions.cbegin(), endIterator, [](const OptionPointer& option) {
            assert(option != nullptr);

            return option->getLongName() == strings::commands::HELP;
        });

    if (helpIt != endIterator && (*helpIt)->isSet()) {
        // Help option is set, we can execute it.
        m_bivalentCommands.at((*helpIt)->getLongName()).get().executeAsOption();
        return false;
    }

    for (const auto& option : commandOptions) {
        assert(option != nullptr);

        const auto longName = option->getLongName();
        assert(m_bivalentCommands.contains(longName));

        if (option->isSet()) {
            // If the option is set we execute th ebivalent command as option.
            bCanContinue = m_bivalentCommands.at(longName).get().executeAsOption();

            if (!bCanContinue)
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
    m_bivalentCommands.emplace(asOption->getLongName(), bivalentCommand);
}

} // namespace rpi_gc
