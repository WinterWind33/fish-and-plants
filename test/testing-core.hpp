// Copyright (c) 2023 Andrea Ballestrazzi
#pragma once

#if USE_CATCH2_AS_TESTING_FRAMEWORK
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#else
#error "No core framework support for testing apart from Catch2"
#endif // USE_CATCH2_AS_TESTING_FRAMEWORK
