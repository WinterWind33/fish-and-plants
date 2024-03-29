// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef APPLICATION_STRINGS_HPP
#define APPLICATION_STRINGS_HPP

#include <common/types.hpp>

namespace rpi_gc::strings::application {

constexpr StringViewType NAME{"Greenhouse Controller"};
constexpr StringViewType EXECUTABLE_NAME{"rpi_gc"};
constexpr StringViewType COPYRIGHT_DISCLAIMER{"Copyright (c) 2023 Andrea Ballestrazzi"};
constexpr StringViewType TEAM_NAME{"Fish&Plants Team"};
constexpr StringViewType MAIN_LOG_FILENAME{"fish_and_plants.log"};

} // namespace rpi_gc::strings::application

#endif // !APPLICATION_STRINGS_HPP
