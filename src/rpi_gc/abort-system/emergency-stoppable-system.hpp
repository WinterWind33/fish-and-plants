// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef EMERGENCY_STOPPABLE_SYSTEM
#define EMERGENCY_STOPPABLE_SYSTEM

namespace rpi_gc::abort_system {

    //!!
    //! \brief Represents the basic interface used to represent a system that is
    //!  stoppable under an emergency.
    class EmergencyStoppableSystem {
    public:

        //!!
        //! \brief Notifies the system that an emergency is under way and
        //!  it must shutdown immediately.
        virtual void emergencyAbort() noexcept = 0;
    };

} // namespace rpi_gc

#endif //! EMERGENCY_STOPPABLE_SYSTEM
