// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef APPLICATION_STRINGS_HPP
#define APPLICATION_STRINGS_HPP

#include <common/types.hpp>

// C++ STL
#include <string_view>

namespace rpi_gc::strings::application {

    constexpr StringViewType NAME{"Greenhouse Controller"};
    constexpr StringViewType COPYRIGHT_DISCLAIMER{"Copyright (c) 2022 Andrea Ballestrazzi"};
    constexpr StringViewType TEAM_NAME{"Fish&Plants Team"};

} // namespace rpi_gc

#endif // !APPLICATION_STRINGS_HPP
