// Copyright (c) 2022 Andrea Ballestrazzi
#include <commands/abort-command.hpp>

// C++ STL
#include <cassert>
#include <version>

#ifdef __cpp_lib_format
#include <format>
#else
#include <sstream>
#endif // __cpp_lib_format

namespace rpi_gc::commands {

    AbortCommand::AbortCommand(logger_pointer mainLogger, std::vector<emergency_stoppable_system_pointer> systems) noexcept :
        m_mainLogger{std::move(mainLogger)},
        m_stoppableSystems{std::move(systems)} {
        assert(static_cast<bool>(m_mainLogger));
    }

    bool AbortCommand::execute() noexcept {
        m_mainLogger->logWarning(format_log_message(strings::commands::feedbacks::abort::ABORT_ISSUED));
        m_mainLogger->logWarning(format_log_message(strings::commands::feedbacks::abort::STARTING_EMERGENCY_ABORT));

        for(auto& stoppableSystem : m_stoppableSystems) {
            assert(static_cast<bool>(stoppableSystem));

            stoppableSystem->emergencyAbort();
        }

        m_mainLogger->logWarning(format_log_message(strings::commands::feedbacks::abort::EMERGENCY_ABORT_FINISHED));
        return true;
    }

    void AbortCommand::printHelp(help_ostream_type outputStream) const noexcept {
        outputStream.get() << "[NAME]" << std::endl;
        outputStream.get() << "\t" << strings::commands::ABORT << " - Aborts the greenhouse controller's automatic systems without ";
        outputStream.get() << "exiting the application." << std::endl;
        outputStream.get() << "[DESCRIPTION]" << std::endl;
        outputStream.get() << "\t" << "The abort system is designed to stop every automatic job without taking too much time. It only ";
        outputStream.get() << "releases important resources in order to do no harm to humans or plants. These resources can be ";
        outputStream.get() << "digital outputs, hardware I/O etc...";
    }

    StringType AbortCommand::format_log_message(StringViewType message) noexcept {
        // We print the log name for the system as the first argument and then the message.
        // The final string will be:
        // [Abort System] My custom message.
#ifdef __cpp_lib_format
        return std::format("[{}] {}", strings::commands::feedbacks::abort::EMERGENCY_ABORT_SYSTEM_LOGGER_NAME, message);
#else
        std::ostringstream outputStream{};
        outputStream << "[" << StringType{strings::commands::feedbacks::abort::EMERGENCY_ABORT_SYSTEM_LOGGER_NAME} << "] ";
        outputStream << StringType{message};

        return outputStream.str();
#endif // __cpp_lib_format
    }

} // namespace rpi_gc::commands
