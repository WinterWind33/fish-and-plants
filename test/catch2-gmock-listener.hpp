// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef CATCH2_GMOCK_LISTENER_HPP
#define CATCH2_GMOCK_LISTENER_HPP

#if defined(USE_CATCH2_AS_TESTING_FRAMEWORK) && defined(USE_GMOCK)

#include <catch2/catch_amalgamated.hpp>
#include <gmock/gmock.h>

//! \brief Represents the listener used by GMock when using its
//!  constructs to do tests (e.g. EXPECT_CALL, ON_CALL etc...)
//!	 This basically inherits from a defaulted event listener.
class GMCatch2Listener : public testing::EmptyTestEventListener {
public:

    //! \brief Called after test partial result. This will transform
    //!	 GMock tests data into Catch2 test data that can be used by
    //!  Catch2 to provide feedback and fail tests.
    //!
    //! \param result The GMock test partial result data that needs to be
    //!	 converted into Catch2 data.
    void OnTestPartResult(const testing::TestPartResult& result) override;
};

#endif // USE_CATCH2_AS_TESTING_FRAMEWORK && USE_GMOCK
#endif // !CATCH2_GMOCK_LISTENER_HPP
