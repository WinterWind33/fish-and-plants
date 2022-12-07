// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef AUTOMATIC_WATERING_SYSTEM_HPP
#define AUTOMATIC_WATERING_SYSTEM_HPP

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

#endif // !AUTOMATIC_WATERING_SYSTEM_HPP
