// Copyright (c) 2022 Andrea Ballestrazzi
#include <commands/help-command.hpp>

// C++ STL
#include <cassert>

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
        // For every option parser we print the help page as it should contain both
        // the command description and its options as well.
        m_outputStream.get() << "Commands list:" << std::endl;
        for(const auto& optionParser : m_optionParsers) {
            m_outputStream.get() << "\t=> " << std::get<0>(optionParser) << std::endl;
            std::get<1>(optionParser).get().printHelp(m_outputStream.get());

            m_outputStream.get() << std::endl;
        }

        return true;
    }

} // namespace rpi_gc
