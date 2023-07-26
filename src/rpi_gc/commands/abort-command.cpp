// Copyright (c) 2023 Andrea Ballestrazzi
#include <commands/abort-command.hpp>

// C++ STL
#include <cassert>
#include <version>

#include <iostream> // TODO: This is terrible. Need to be refactored once the commands are refactored.

#ifdef __cpp_lib_format
#include <format>
#else
#include <sstream>
#endif // __cpp_lib_format

namespace rpi_gc::commands {

AbortCommand::AbortCommand(logger_pointer mainLogger,
                           std::vector<emergency_stoppable_system_pointer> systems,
                           option_parser_pointer optionParser) noexcept
    : m_mainLogger{std::move(mainLogger)},
      m_stoppableSystems{std::move(systems)},
      m_optionParser{std::move(optionParser)} {
    assert(static_cast<bool>(m_mainLogger));
    assert(static_cast<bool>(m_optionParser));
}

bool AbortCommand::processInputOptions(const std::vector<string_type>& inputTokens) {
    m_optionParser->parse(inputTokens);
    return true;
}

bool AbortCommand::execute() noexcept {
    auto options{m_optionParser->getOptions()};

    // If the --help option is selected we need to print the help.
    auto helpIt = std::find_if(options.begin(), options.end(),
                               [](const option_parser::option_pointer& option) -> bool {
                                   return option->getLongName() == "help";
                               });

    if (helpIt != options.end() && (*helpIt)->isSet()) {
        printHelp(std::cout);
        for (auto& option : options)
            option->clear();

        return true;
    }

    m_mainLogger->logWarning(format_log_message(strings::commands::feedbacks::abort::ABORT_ISSUED));
    m_mainLogger->logWarning(
        format_log_message(strings::commands::feedbacks::abort::STARTING_EMERGENCY_ABORT));

    for (auto& stoppableSystem : m_stoppableSystems) {
        assert(static_cast<bool>(stoppableSystem));

        stoppableSystem->emergencyAbort();
    }

    m_mainLogger->logWarning(
        format_log_message(strings::commands::feedbacks::abort::EMERGENCY_ABORT_FINISHED));
    return true;
}

void AbortCommand::printHelp(help_ostream_type outputStream) const noexcept {
    outputStream.get() << "[NAME]" << std::endl;
    outputStream.get() << "\t" << strings::commands::ABORT
                       << " - Aborts the greenhouse controller's automatic systems without ";
    outputStream.get() << "exiting the application." << std::endl;
    outputStream.get() << "[DESCRIPTION]" << std::endl;
    outputStream.get() << "\t"
                       << "The abort system is designed to stop every automatic job, like "
                          "automatic irrigation, in an emergency situation."
                       << std::endl;
    outputStream.get() << "\tThis means that all the running jobs are interrupted and hardware is "
                          "shutted down to prevent any damage ";
    outputStream.get() << "to equipment and plants.";
    outputStream.get() << std::endl;
}

StringType AbortCommand::format_log_message(StringViewType message) noexcept {
    // We print the log name for the system as the first argument and then the message.
    // The final string will be:
    // [Abort System] My custom message.
#ifdef __cpp_lib_format
    return std::format("[{}] {}",
                       strings::commands::feedbacks::abort::EMERGENCY_ABORT_SYSTEM_LOGGER_NAME,
                       message);
#else
    std::ostringstream outputStream{};
    outputStream
        << "["
        << StringType{strings::commands::feedbacks::abort::EMERGENCY_ABORT_SYSTEM_LOGGER_NAME}
        << "] ";
    outputStream << StringType{message};

    return outputStream.str();
#endif // __cpp_lib_format
}

} // namespace rpi_gc::commands
