// Copyright (C) 2022 Andrea Ballestrazzi
#include <commands/automatic-watering/automatic-watering-command.hpp>

// C++ STL
#include <algorithm>

namespace rpi_gc {

    AutomaticWateringCommand::AutomaticWateringCommand(ostream_ref outputStream, option_parser_pointer optionParser) noexcept :
        m_outputStream{std::move(outputStream)},
        m_optionParser{std::move(optionParser)} {}

    bool AutomaticWateringCommand::processInputOptions(const std::vector<string_type>& inputTokens) noexcept {
        assert(m_optionParser != nullptr);

        m_optionParser->parse(inputTokens);
        return true;
    }

    bool AutomaticWateringCommand::execute() noexcept {
        const auto& options{m_optionParser->getOptions()};

        // First of all we need to check if the help
        // option is set. If this is the case we need to
        // execute it first and return.
        auto optionIt = std::find_if(options.cbegin(), options.cend(), [](const option_parser::const_option_pointer& option){
            assert(option != nullptr);

            return option->getLongName() == strings::commands::HELP;
        });

        if(optionIt != options.cend() && (*optionIt)->isSet()) {
            // We have found the command help and it's set. We can execute it.
            printHelp(m_outputStream.get());

            return true;
        }

        for(const auto& option : options) {
            assert(option != nullptr);

            if(option->isSet() && m_optionsEvents.contains(option->getLongName())) {
                // We retrieve all the events in the map.
                auto iteratorPair = m_optionsEvents.equal_range(option->getLongName());

                for(auto it = std::get<0>(iteratorPair); it != std::get<1>(iteratorPair); ++it) {
                    const option_event& event{std::get<1>(*it)};
                    assert((bool)event);

                    // We trigger the event.
                    event(option);
                }
            }
        }

        return true;
    }

    void AutomaticWateringCommand::printHelp(help_ostream_type outputStream) const noexcept {
        assert(m_optionParser != nullptr);

        outputStream.get() << "[NAME]" << std::endl;
        outputStream.get() << "\t" << strings::commands::AUTOMATIC_WATERING << " - manages the automatic irrigation of the greenhouse" << std::endl;
        outputStream.get() << "[SYNOPSIS]" << std::endl;
        outputStream.get() << "\t" << strings::commands::AUTOMATIC_WATERING << " [OPTION]..." << std::endl;
        outputStream.get() << "[DESCRIPTION]" << std::endl;
        outputStream.get() << "\t" << "Manages the automatic irrigation of the greenhouse activating or" << std::endl;
        outputStream.get() << "\tdisactivating the pump and the valve in order to make the water flow." << std::endl;
        outputStream.get() << std::endl;

        m_optionParser->printHelp(outputStream.get());
    }

    void AutomaticWateringCommand::registerOptionEvent(option_type::long_name_type optionName, option_event event) noexcept {
        assert(!optionName.empty());

        if(optionName == strings::commands::HELP) {
            // The user can't register an event for the "--help" command.
            assert(false);

            return;
        }

        m_optionsEvents.emplace(std::move(optionName), std::move(event));
    }

} // namespace rpi_gc
