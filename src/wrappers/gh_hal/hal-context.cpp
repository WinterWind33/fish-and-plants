// Copyright (c) 2023 Andrea Ballestrazzi
#include <gh_hal/hal-context.hpp>

#ifdef USE_CPPGPIO
#include <cppgpio.hpp>
#endif // USE_CPPGPIO

#ifdef USE_LIBGPIOD
#include <gpiod.h>
#include <cerrno> // For errors with libgpiod

// C++ STL
#endif // USE_LIBGPIOD

#include <filesystem>
// C++ STL
#include <sstream>
#include <string_view>

namespace gh_hal
{

    HALContext::HALContext(logger_pointer logger, const bool bIsSim, [[maybe_unused]] const bool bForceFullMap) noexcept :
        m_logger{std::move(logger)},
        m_bIsSimulation{bIsSim} {

        assert(m_logger != nullptr);

        std::ostringstream logStream{};

#ifdef USE_CPPGPIO
        logStream << "[Hardware Abstraction Layer] => Setting simulation to " << m_bIsSimulation << ".";
        m_logger->logInfo(logStream.str());

        GPIO::GPIOBase::simulation(m_bIsSimulation);

        logStream.str("");
        logStream << "[Hardware Abstraction Layer] => Forcing full mapping.";

        m_logger->logInfo(logStream.str());

        if (bForceFullMap)
        {
            const bool bIsInFullMapMode{GPIO::GPIOBase::force_full_mapping()};

            if (bIsInFullMapMode)
                m_logger->logWarning("[Hardware Abstraction Layer] => Full map mode enabled.");
            else
                m_logger->logError("[Hardware Abstraction Layer] => Full map mode is not enabled.");
        }
#endif // USE_CPPGPIO
#ifdef USE_LIBGPIOD
        constexpr std::string_view GPIO_CHIP_PATH{"/dev/gpiochip0"};

        logStream << "[Hardware Abstraction Layer] => Opening chip: " << GPIO_CHIP_PATH;
        const std::filesystem::path chipPath{GPIO_CHIP_PATH};

        m_boardChip = std::make_unique<chip_type>(chipPath);
        if(!m_boardChip) {
            logStream.str("");
            logStream << "[Hardware Abstraction Layer] => Failed to open the chip.";
            logStream << " Error code: " << errno << "; Error message: " << std::strerror(errno);
            m_logger->logError(logStream.str());
            return;
        }

        logStream.str("");
        logStream << "[Hardware Abstraction Layer] => Chip opened successfully.";
        m_logger->logInfo(logStream.str());
#endif // USE_LIBGPIOD
    }
} // namespace gh_hal
