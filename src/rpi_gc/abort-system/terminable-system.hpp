// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef TERMINABLE_SYSTEM_HPP
#define TERMINABLE_SYSTEM_HPP

namespace rpi_gc::abort_system {

    //!!
    //! \brief Represents a system that is terminable after a request.
    //!  Sends the FSM of the system to the shutdown mode.
    class TerminableSystem {
    public:
        virtual ~TerminableSystem() noexcept = default;

        //!!
        //! \brief Requests and can perform the shutdown of the system.
        //!  This is tipically an async request that tells the system FSM
        //!  to transition to the shudown mode.
        virtual void requestShutdown() noexcept = 0;
    };

} // namespace rpi_gc::abort_system

#endif // !TERMINABLE_SYSTEM_HPP
