// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef TERMINAL_COMMAND_MOCK_HPP
#define TERMINAL_COMMAND_MOCK_HPP

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

#include <commands/terminal-command.hpp>

namespace rpi_gc::mocks {

    template<typename CharT>
    class TerminalCommandMock : public TerminalCommand<CharT> {
    public:
        using typename TerminalCommand<CharT>::name_type;
        using typename TerminalCommand<CharT>::string_type;
        using typename TerminalCommand<CharT>::help_ostream_type;

        ~TerminalCommandMock() noexcept override = default;

        // From Command
        MOCK_METHOD(name_type, getName, (), (const, noexcept, override));
        MOCK_METHOD(bool, execute, (), (noexcept, override));

        // From TerminalCommand
        MOCK_METHOD(bool, processInputOptions, (const std::vector<string_type>&), (noexcept, override));
        MOCK_METHOD(void, printHelp, (help_ostream_type&), (const, noexcept, override));
    };

} // namespace rpi_gc::mocks

#endif // !TERMINAL_COMMAND_MOCK_HPP
