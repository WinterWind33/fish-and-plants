// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef BIVALENT_COMMAND_MOCK_HPP
#define BIVALENT_COMMAND_MOCK_HPP

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

#include <commands/bivalent-command.hpp>

namespace rpi_gc::mocks {

    template<typename CharT>
    class BivalentCommandMock : public BivalentCommand {
    public:
        using typename TerminalCommand<CharT>::name_type;
        using typename TerminalCommand<CharT>::string_type;
        using typename TerminalCommand<CharT>::help_ostream_type;

        ~BivalentCommandMock() noexcept override = default;

        // From Command
        MOCK_METHOD(name_type, getName, (), (const, noexcept, override));
        MOCK_METHOD(bool, execute, (), (noexcept, override));

        // From TerminalCommand
        MOCK_METHOD(bool, processInputOptions, (const std::vector<string_type>&), (noexcept, override));
        MOCK_METHOD(void, printHelp, (help_ostream_type), (const, noexcept, override));

        // From BivalentCommand
        MOCK_METHOD(option_pointer, getAsOption, (),  (const, noexcept, override));
        MOCK_METHOD(bool, executeAsOption, (), (noexcept, override));
    };

} // namespace rpi_gc::mocks

#endif // !BIVALENT_COMMAND_MOCK_HPP
