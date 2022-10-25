// Copyright (C) 2022 Andrea Ballestrazzi
#include <greenhouse-controller-application.hpp>

// C++ STL
#include <utility>

namespace rpi_gc {

    GreenhouseControllerApplication::GreenhouseControllerApplication(ostream_ref outputStream, istream_ref inputStream) noexcept :
        m_outputStream{std::move(outputStream)},
        m_inputStream{std::move(inputStream)} {}

    void GreenhouseControllerApplication::run() noexcept {}

} // namespace rpi_gc
