// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

// C++ STL
#include <ostream>

namespace rpi_gc::diagnostics {

//!!
//! \brief Represents the basic interface of an object whose status
//!  is probeable for diagnostic reasons.
struct DiagnosticStatusProbeable {
    virtual ~DiagnosticStatusProbeable() noexcept = default;

    //!!
    //! \brief Prints the diagnostics of the object status to the given
    //!  output stream.
    //! \param ost The stream that will receive the diagnostic messages.
    virtual void printDiagnostic(std::ostream& ost) const noexcept = 0;
};

} // namespace rpi_gc::diagnostics
