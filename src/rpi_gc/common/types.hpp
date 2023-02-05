// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef TYPES_HPP
#define TYPES_HPP

// C++ STL
#include <string>
#include <istream>
#include <ostream>
#include <sstream>
#include <string_view>

namespace rpi_gc {
    // Here are defined the aliases used throughout the application.

    using CharType = char;
    using StringType = std::basic_string<CharType>;
    using InputStringStream = std::basic_istringstream<CharType>;
    using OutputStringStream = std::basic_ostringstream<CharType>;
    using InputStream = std::basic_istream<CharType>;
    using OutputStream = std::basic_ostream<CharType>;

    using StringViewType = std::basic_string_view<CharType>;

} // namespace rpi_gc

#endif // !TYPES_HPP
