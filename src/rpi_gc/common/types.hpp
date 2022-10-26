// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef TYPES_HPP
#define TYPES_HPP

// C++ STL
#include <string>
#include <istream>
#include <ostream>
#include <sstream>

namespace rpi_gc {
    // Here are defined the aliases used throughout the application.

    using CharType = char;
    using StringType = std::basic_string<CharType>;
    using InputStringStream = std::istringstream;
    using OutputStringStream = std::ostringstream;
    using InputStream = std::istream;
    using OutputStream = std::ostream;

} // namespace rpi_gc

#endif // !TYPES_HPP
