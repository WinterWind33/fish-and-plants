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

} // namespace rpi_gc
