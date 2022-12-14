// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef HAL_DIGITAL_OUTPUT_HPP
#define HAL_DIGITAL_OUTPUT_HPP

namespace gh_hal {

    //! \brief Represents the basic interface of a digital output, i.e.
    //!  an output on the electronic board that sends a digital signal.
    struct HALDigitalOutput {
        virtual ~HALDigitalOutput() noexcept = default;

        virtual void turnOn() noexcept = 0;
        virtual void turnOff() noexcept = 0;
    };

} // namespace gh_hal

#endif // !HAL_DIGITAL_OUTPUT_HPP
