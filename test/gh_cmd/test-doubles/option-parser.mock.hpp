// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef OPTION_PARSER_MOCK_HPP
#define OPTION_PARSER_MOCK_HPP

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

#include <gh_cmd/gh_cmd.hpp>

namespace gh_cmd::mocks {

    template<typename CharType>
    class OptionParserMock : public OptionParser<CharType> {
    public:
        using typename OptionParser<CharType>::char_type;
        using typename OptionParser<CharType>::string_type;

        ~OptionParserMock() noexcept override = default;

        MOCK_METHOD(void, addSwitch, (std::shared_ptr<Switch<char_type>>), (noexcept, override));
        MOCK_METHOD(void, parse, (const std::vector<string_type>&), (noexcept, override));
        MOCK_METHOD(void, reset, (), (noexcept, override));
        MOCK_METHOD(void, printHelp, (std::basic_ostream<char_type>&), (const, noexcept, override));
        MOCK_METHOD((std::vector<std::shared_ptr<const CommandOption<char_type>>>), getOptions, (), (const, noexcept, override));
        MOCK_METHOD(std::vector<string_type>, getNonOptionArguments, (), (const, noexcept, override));
        MOCK_METHOD(std::vector<string_type>, getUnknownOptions, (), (const, noexcept, override));
    };

} // namespace gh_cmd

#endif // !OPTION_PARSER_MOCK_HPP
