// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef HAL_CONTEXT_HPP
#define HAL_CONTEXT_HPP

#include <gh_log/logger.hpp>

#ifdef USE_LIBGPIOD
#include <gpiod.hpp>
#endif // USE_LIBGPIOD

// C++ STL
#include <memory>

namespace gh_hal
{

    class HALContext
    {
    public:
        using logger_pointer = std::shared_ptr<gh_log::Logger>;

        HALContext(logger_pointer logger, const bool bIsSim = true, const bool bForceFullMap = false) noexcept;
        virtual ~HALContext() noexcept = default;

        virtual const logger_pointer &getLogger() const noexcept
        {
            return m_logger;
        }

        virtual bool isSimulation() const noexcept { return m_bIsSimulation; }

    private:
        logger_pointer m_logger{};
        bool m_bIsSimulation{};

#ifdef USE_LIBGPIOD
        using chip_type = gpiod::chip;

        std::unique_ptr<chip_type> m_boardChip{};
#endif // USE_LIBGPIOD
    };

} // namespace gh_hal

#endif // !HAL_CONTEXT_HPP
