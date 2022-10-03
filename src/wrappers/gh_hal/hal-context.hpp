// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef HAL_CONTEXT_HPP
#define HAL_CONTEXT_HPP

#include <logger.hpp>

// C++ STL
#include <memory>

namespace gh_hal {

    class HALContext {
    public:
        using logger_pointer = std::shared_ptr<gh_log::Logger>;

        HALContext(logger_pointer logger) noexcept;
        virtual ~HALContext() noexcept = default;

        virtual const logger_pointer& getLogger() const noexcept {
            return m_logger;
        }

    private:
        logger_pointer m_logger{};
    };

} // namespace gh_hal

#endif // !HAL_CONTEXT_HPP
