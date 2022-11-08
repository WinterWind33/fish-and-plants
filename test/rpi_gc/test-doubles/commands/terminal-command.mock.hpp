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
        using Command::name_type;
        using typename TerminalCommand<CharT>::options_vector;
        using typename TerminalCommand<CharT>::non_options_vector;
        using typename TerminalCommand<CharT>::unknown_options_vector;

        ~TerminalCommandMock() noexcept override = default;

        // From Command
        MOCK_METHOD(name_type, getName, (), (const, noexcept, override));
        MOCK_METHOD(bool, execute, (), (noexcept, override));

        // From TerminalCommand
        MOCK_METHOD(bool, processOptions, (const options_vector&, const non_options_vector&, const unknown_options_vector&), (noexcept, override));
    };

} // namespace rpi_gc::mocks

#endif // !TERMINAL_COMMAND_MOCK_HPP
