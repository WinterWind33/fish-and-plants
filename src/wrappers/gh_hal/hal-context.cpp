// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/hal-context.hpp>

#ifdef USE_CPPGPIO
#include <cppgpio.hpp>
#endif // USE_CPPGPIO

// C++ STL
#include <sstream>

namespace gh_hal {

    HALContext::HALContext(logger_pointer logger, const bool bIsSim) noexcept :
        m_logger{std::move(logger)},
        m_bIsSimulation{bIsSim} {
        assert(m_logger != nullptr);

        std::ostringstream logStream{};
        logStream << "[Hardware Abstraction Layer] => Setting simulation to " << m_bIsSimulation << ".";

        m_logger->logInfo(logStream.str());
#ifdef USE_CPPGPIO
        GPIO::GPIOBase::simulation(m_bIsSimulation);
#endif // USE_CPPGPIO

        logStream.str("");
        logStream << "[Hardware Abstraction Layer] => Forcing full mapping.";

        m_logger->logInfo(logStream.str());

#ifdef USE_CPPGPIO
        const bool bIsInFullMapMode{GPIO::GPIOBase::force_full_mapping()};

        if(bIsInFullMapMode)
            m_logger->logWarning("[Hardware Abstraction Layer] => Full map mode enabled.");
        else
            m_logger->logError("[Hardware Abstraction Layer] => Full map mode is not enabled.");
#endif // USE_CPPGPIO
    }

} // namespace gh_hal
