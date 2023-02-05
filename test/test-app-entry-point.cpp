// Copyright (c) 2023 Andrea Ballestrazzi
#include <catch2-gmock-listener.hpp>

#ifdef USE_GMOCK
#include <gmock/gmock.h>
#endif // USE_GMOCK

// C++ STL
#include <cstdint>
#include <iostream>
#include <memory>

//! \brief Utility function used to sink a pointer, i.e. delete it and set it to nullptr.
//!  If the passed pointer is nullptr or the pointed pointer is nullptr, nothing will happen.
//!
//! \tparam T The type of the pointed-to value by *pointer that needs to be deleted.
//! \param pointer The pointer-to the pointed value that needs to be deleted.
template<typename T>
static void SinkPointer(T** pointer) {
    if (pointer == nullptr || *pointer == nullptr)
        return;

    delete *pointer;
    *pointer = nullptr;
}

int main(int argc, char* argv[]) {
    std::unique_ptr<Catch::Session> catch2Session{};

    // Before starting Catch2, we need to initialize
    // GMock library. In order to do so, we need to call
    // `testing::InitGoogleMock` but we DO NOT need to pass every argument
    // to GMock. We build the following int variable so it doesn't
    // parse every argument, just the first one.
    std::int32_t gmockArgC = 1;
    testing::InitGoogleMock(&gmockArgC, argv);

    testing::UnitTest* gmockUnitTestInstance = testing::UnitTest::GetInstance();
    if (!gmockUnitTestInstance) {
        std::cerr << "[FATAL] => Failed to initialize GMock: unit test instance in NULL." << std::endl;

        // INVALID_HANDLE error
        return 6;
    }

    testing::TestEventListeners& listeners = gmockUnitTestInstance->listeners();
    testing::TestEventListener* defaultResultPrinter = listeners.default_result_printer();

    // We can happend our Catch2 listener.
    listeners.Append(new GMCatch2Listener{});

    // Here we need to remove the default result listener, as we are providing
    // our own listener.
    // NOTE: when a GMock listener is released, we become the owner, so we need to delete the
    // released listener.
    testing::TestEventListener* releasedListener = listeners.Release(defaultResultPrinter);
    SinkPointer(&releasedListener);
    defaultResultPrinter = nullptr;

    catch2Session = std::make_unique<Catch::Session>();
    if (!catch2Session) {
        std::cerr << "[FATAL] => Cannot create Catch2 session, system internal error." << std::endl;
        return 6;
    }

    catch2Session->applyCommandLine(argc, argv);
    return catch2Session->run();
}
