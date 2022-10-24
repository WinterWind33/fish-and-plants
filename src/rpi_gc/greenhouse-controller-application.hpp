// Copyright (C) 2022 Andrea Ballestrazzi
#ifndef GREENHOUSE_CONTROLLER_APPLICATION_HPP
#define GREENHOUSE_CONTROLLER_APPLICATION_HPP

#include <application/application.hpp>

// C++ STL
#include <functional>

namespace rpi_gc {

    class GreenhouseControllerApplication : public Application {
    public:
        using ostream_ref = std::reference_wrapper<OutputStream>;

        GreenhouseControllerApplication(ostream_ref outputStream) noexcept;
        ~GreenhouseControllerApplication() noexcept override = default;

        void run() noexcept override;

    private:
        ostream_ref m_outputStream;
    };

} // namespace rpi_gc

#endif // !GREENHOUSE_CONTROLLER_APPLICATION_HPP
