// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

// C++ STL
#include <utility>

namespace rpi_gc {

    GreenhouseControllerApplication::GreenhouseControllerApplication(ostream_ref outputStream) noexcept :
        m_outputStream{std::move(outputStream)} {}

    void GreenhouseControllerApplication::run() noexcept {}

} // namespace rpi_gc
