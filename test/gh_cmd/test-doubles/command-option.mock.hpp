// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef COMMAND_OPTION_MOCK_HPP
#define COMMAND_OPTION_MOCK_HPP

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

#include <gh_cmd/gh_cmd.hpp>

namespace gh_cmd::mocks {

    template<typename CharType>
    class CommandOptionMock : public CommandOption<CharType> {
    public:
        using typename CommandOption<CharType>::base_impl_type;
        using typename CommandOption<CharType>::short_name_type;
        using typename CommandOption<CharType>::long_name_type;
        using typename CommandOption<CharType>::string_type;

        ~CommandOptionMock() noexcept override = default;

        MOCK_METHOD(short_name_type, getShortName, (), (const, noexcept, override));
        MOCK_METHOD(long_name_type, getLongName, (), (const, noexcept, override));
        MOCK_METHOD(string_type, getDescription, (), (const, noexcept, override));
        MOCK_METHOD(void, acceptVisitor, (const ConstOptionVisitor<std::shared_ptr<const base_impl_type>>&), (const, noexcept, override));
        MOCK_METHOD(void, acceptVisitor, (OptionVisitor<std::shared_ptr<base_impl_type>>&), (noexcept, override));
        MOCK_METHOD(bool, isSet, (), (const, noexcept, override));
    };

} // namespace gh_cmd

#endif // !COMMAND_OPTION_MOCK_HPP
