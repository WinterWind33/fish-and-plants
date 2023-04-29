// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef HAL_DIGITAL_OUTPUT_HPP
#define HAL_DIGITAL_OUTPUT_HPP

namespace gh_hal {

    //! \brief Represents the basic interface of a digital output, i.e.
    //!  an output on the electronic board that sends a digital signal.
    struct [[deprecated("Do not use this, use hardware_access::BoardDigitalPin instead.")]]
    HALDigitalOutput {
        virtual ~HALDigitalOutput() noexcept = default;

        virtual void turnOn() noexcept = 0;
        virtual void turnOff() noexcept = 0;
    };

} // namespace gh_hal

#endif // !HAL_DIGITAL_OUTPUT_HPP
