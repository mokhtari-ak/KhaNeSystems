add_library(supervisor STATIC CpuThrottler.cpp)
target_include_directories(supervisor PUBLIC .)
