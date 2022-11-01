// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef OPTION_VISITOR_MOCK_HPP
#define OPTION_VISITOR_MOCK_HPP

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

#include <gh_cmd/gh_cmd.hpp>

namespace gh_cmd::mocks {

    template<typename OptionType>
    class OptionVisitorMock : public OptionVisitor<OptionType> {
    public:
        MOCK_METHOD(void, visit, (OptionType), (noexcept, override));
    };

    template<typename OptionType>
    class ConstOptionVisitorMock : public ConstOptionVisitor<OptionType> {
    public:
        MOCK_METHOD(void, visit, (const OptionType), (const, noexcept, override));
    };

} // namespace gh_cmd::mocks

#endif // !OPTION_VISITOR_MOCK_HPP
