// Copyright (c) 2022 Andrea Ballestrazzi
#ifndef GH_LOG_LIB_BASE_HPP
#define GH_LOG_LIB_BASE_HPP

#ifdef USE_SPDLOG
#include <spdlog/spdlog.h>
#else
#error "There is no alternative to SPDLOG now."
#endif

#endif // !GH_LOG_LIB_BASE_HPP
