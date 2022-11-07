// Copyright (C) 2022 Andrea Ballestrazzi
#include <commands/application-help-command.hpp>

namespace rpi_gc {

    ApplicationHelpCommand::ApplicationHelpCommand(ostream_ref outputStream, option_parser_ref optionParser) noexcept :
        m_outputStream{std::move(outputStream)},
        m_optionParser{std::move(optionParser)} {}

} // namespace rpi_gc
