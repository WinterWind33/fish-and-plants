// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef GH_LOG_LOGGER_HPP
#define GH_LOG_LOGGER_HPP

#include <gh_log/gh-log-lib-base.hpp>

namespace gh_log {

    //!!
    //! \brief Represents the logging level to be used to log a message
    //!  using a logger object.
    enum class ELoggingLevel {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    //! \brief Represents the basic interface of a logger capable of logging
    //!  different levels of severity.
    struct Logger {
        virtual ~Logger() noexcept = default;

        //!!
        //! \brief Logs a message to the sinks registered inside this logger object with the
        //!  specified logging level.
        //!
        //! \param logLevel The desired log level.
        //! \param message The desired message to be logged.
        virtual void logMessage(const ELoggingLevel logLevel, LogStringType message) = 0;

        virtual void logTrace(const LogStringType& msg) = 0;
        virtual void logDebug(const LogStringType& msg) = 0;
        virtual void logInfo(const LogStringType& msg) = 0;
        virtual void logWarning(const LogStringType& msg) = 0;
        virtual void logError(const LogStringType& msg) = 0;
        virtual void logCritical(const LogStringType& msg) = 0;

        //!!
        //! \brief Flushes the logger to the desired sink without waiting the
        //!  destruction of the object.
        //!  Normally, the flush is automatically done during the logger destruction but
        //!  this will force the operation.
        virtual void flush() = 0;

        //!!
        //! \brief Set the Automatic Flush Level that will trigger an automatic flush after
        //!  a log method call that specifies that particular logging level or greater (greater =
        //!  bigger severity that the flush level)/
        //!
        //! \param logLevel The minimum logging level that will trigger the automatic flush
        //!  after a message log operation.
        virtual void setAutomaticFlushLevel(const ELoggingLevel logLevel) = 0;
    };

} // namespace gh_log

#endif // !GH_LOG_LOGGER_HPP
