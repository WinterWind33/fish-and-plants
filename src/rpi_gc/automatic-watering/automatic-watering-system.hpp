// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <string>

namespace rpi_gc::automatic_watering {

//!!
//! \brief Represents the basic interface for an automatic watering system.
//!
class AutomaticWateringSystem {
public:
    using name_type = std::string;

    virtual ~AutomaticWateringSystem() noexcept = default;

    //!!
    //! \brief Starts the automatic watering system.
    //!
    //! \param awsName The name of the automatic watering system.
    //!
    virtual void startAutomaticWatering(name_type awsName = name_type{}) noexcept = 0;
};

} // namespace rpi_gc::automatic_watering
