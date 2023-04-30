// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef HAL_DEFINITIONS_HPP
#define HAL_DEFINITIONS_HPP

// C++ STL
#include <cstdint>

namespace gh_hal {

    //! \brief Represents the ID of a PIN inside the used board.
    using PinID [[deprecated("Use hardware_access::BoardDigitalPin::offset_type instead.")]] = std::uint32_t;

} // namespace gh_hal

#endif // !HAL_DEFINITIONS_HPP
