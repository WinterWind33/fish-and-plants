# Copyright (c) 2022 Andrea Ballestrazzi

set(POPL20_INCLUDE_FILES "popl.hpp")

add_library(popl20 INTERFACE ${POPL20_INCLUDE_FILES})
target_include_directories(popl20 INTERFACE ".")
