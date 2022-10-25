// Copyright (C) 2022 Andrea Ballestrazzi
#ifndef RPI_GC_APPLICATION_HPP
#define RPI_GC_APPLICATION_HPP

// C++ STL
#include <ostream>

namespace rpi_gc {
    using OutputStream = std::ostream;

    //! \brief Represents the basic interface of an application object that
    //!  directs the application logic.
    struct Application {
        virtual ~Application() noexcept = default;

        //! \brief Runs the application logic.
        //!
        virtual void run() noexcept = 0;
    };

} // namespace rpi_gc

#endif // !RPI_GC_APPLICATION_HPP
