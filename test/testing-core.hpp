// Copyright (c) 2023 Andrea Ballestrazzi
#ifndef TESTING_CORE_HPP
#define TESTING_CORE_HPP

#if USE_CATCH2_AS_TESTING_FRAMEWORK
#include<catch2/catch_amalgamated.hpp>
#else
#error "No core framework support for testing apart from Catch2"
#endif // USE_CATCH2_AS_TESTING_FRAMEWORK

#endif // !TESTING_CORE_HPP
