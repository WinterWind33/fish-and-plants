// Copyright (C) 2023 Andrea Ballestrazzi
#ifndef RPI_GC_APPLICATION_HPP
#define RPI_GC_APPLICATION_HPP

#include <common/types.hpp>

// C++ STL
#include <cstdint>
#include <string>
#include <vector>

namespace rpi_gc {
//! \brief Represents the basic interface of an application object that
//!  directs the application logic.
struct Application {
    virtual ~Application() noexcept = default;

    //! \brief Runs the application logic.
    //!
    virtual void run() noexcept = 0;

    //! \brief Processes the given terminal input options. It can decide whether or not the
    //! application can execute (maybe the user typed --help or --version).
    //! \return True if the application can run, false otherwise.
    virtual bool processInputOptions(const std::vector<std::string>& inputArgs) noexcept = 0;
};

} // namespace rpi_gc

#endif // !RPI_GC_APPLICATION_HPP
