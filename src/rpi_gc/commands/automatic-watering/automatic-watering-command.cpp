// Copyright (C) 2022 Andrea Ballestrazzi
#include <commands/automatic-watering/automatic-watering-command.hpp>

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
        return true;
    }

    void AutomaticWateringCommand::printHelp(help_ostream_type outputStream) const noexcept {
        assert(m_optionParser != nullptr);

        m_optionParser->printHelp(outputStream.get());
    }

} // namespace rpi_gc
