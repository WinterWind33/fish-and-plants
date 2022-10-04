// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef HAL_ERROR_HPP
#define HAL_ERROR_HPP

#ifdef USE_CPPGPIO
#include <cppgpio/gpio.hpp>
#else

// C++ STL
#include <stdexcept>
#endif // USE_CPPGPIO

namespace gh_hal {

#ifdef USE_CPPGPIO
    using HALError = GPIO::GPIOError;
#else
    using HALError = std::runtime_error;
#endif // USE_CPPGPIO

} // namespace gh_hal

#endif // !HAL_ERROR_HPP
