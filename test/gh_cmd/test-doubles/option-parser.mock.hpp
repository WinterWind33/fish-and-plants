// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef OPTION_PARSER_MOCK_HPP
#define OPTION_PARSER_MOCK_HPP

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

#include <gh_cmd/gh_cmd.hpp>

namespace gh_cmd::mocks {

template <typename CharT>
class OptionParserMock : public OptionParser<CharT> {
public:
    using typename OptionParser<CharT>::char_type;
    using typename OptionParser<CharT>::string_type;
    using typename OptionParser<CharT>::option_pointer;
    using typename OptionParser<CharT>::const_option_pointer;

    ~OptionParserMock() noexcept override = default;

    MOCK_METHOD(void, addOption, (option_pointer), (noexcept, override));
    MOCK_METHOD(void, addSwitch, (std::shared_ptr<Switch<char_type>>), (noexcept, override));
    MOCK_METHOD(void, parse, (const std::vector<string_type>&), (noexcept, override));
    MOCK_METHOD(void, reset, (), (noexcept, override));
    MOCK_METHOD(void, printHelp, (std::basic_ostream<char_type>&), (const, noexcept, override));
    MOCK_METHOD((std::vector<const_option_pointer>), getOptions, (), (const, noexcept, override));
    MOCK_METHOD((std::vector<option_pointer>), getOptions, (), (noexcept, override));
    MOCK_METHOD(std::vector<string_type>, getNonOptionArguments, (), (const, noexcept, override));
    MOCK_METHOD(std::vector<string_type>, getUnknownOptions, (), (const, noexcept, override));
};

} // namespace gh_cmd::mocks

#endif // !OPTION_PARSER_MOCK_HPP
