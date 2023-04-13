// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <concepts> // for std::equality_comparable

namespace gh_log::implementation {

    //!!
    //! \brief Represents the basic interface of an automatic flusher, i.e.
    //!  an object that is capable to automatically flush a log message when
    //!  its severity is greater or equal the specified treshold.
    //! \tparam LoggingLevel The type of the logging level used to compare severity.
    template<std::equality_comparable LoggingLevel>
    struct AutomaticFlusher {
        using logging_level = LoggingLevel;

        virtual ~AutomaticFlusher() noexcept = default;

        //!!
        //! \brief Set the Automatic Flush Level that will trigger an automatic flush after
        //!  a log method call that specifies that particular logging level or greater (greater =
        //!  bigger severity that the flush level)/
        //!
        //! \param logLevel The minimum logging level that will trigger the automatic flush
        //!  after a message log operation.
        virtual void setFlushStartingLevel(const logging_level level) noexcept = 0;
    };

} // namespace gh_log::implementation
