// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

namespace rpi_gc::automatic_watering {

//!!
//! \brief Represents the basic interface for an automatic watering system.
//!
class AutomaticWateringSystem {
public:
    virtual ~AutomaticWateringSystem() noexcept = default;

    //!!
    //! \brief Starts the automatic watering system.
    //!
    virtual void startAutomaticWatering() noexcept = 0;
};

} // namespace rpi_gc::automatic_watering
