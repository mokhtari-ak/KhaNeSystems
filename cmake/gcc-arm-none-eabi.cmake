set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# Some default GCC settings
# arm-none-eabi- can be in PATH, or ARM_NONE_EABI_ROOT can point at the
# toolchain install directory.
set(TOOLCHAIN_PREFIX arm-none-eabi- CACHE STRING "ARM GCC executable prefix")
set(ARM_NONE_EABI_ROOT "" CACHE PATH "ARM GNU Toolchain root directory")

set(_ARM_TOOLCHAIN_HINTS)
if(ARM_NONE_EABI_ROOT)
    list(APPEND _ARM_TOOLCHAIN_HINTS "${ARM_NONE_EABI_ROOT}")
endif()
if(DEFINED ENV{ARM_NONE_EABI_ROOT})
    list(APPEND _ARM_TOOLCHAIN_HINTS "$ENV{ARM_NONE_EABI_ROOT}")
endif()
list(APPEND _ARM_TOOLCHAIN_HINTS
    "C:/Program Files (x86)/Arm/GNU Toolchain mingw-w64-i686-arm-none-eabi"
    "C:/Program Files/Arm/GNU Toolchain mingw-w64-i686-arm-none-eabi"
)

find_program(CMAKE_C_COMPILER NAMES ${TOOLCHAIN_PREFIX}gcc HINTS ${_ARM_TOOLCHAIN_HINTS} PATH_SUFFIXES bin REQUIRED)
find_program(CMAKE_CXX_COMPILER NAMES ${TOOLCHAIN_PREFIX}g++ HINTS ${_ARM_TOOLCHAIN_HINTS} PATH_SUFFIXES bin REQUIRED)
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
find_program(CMAKE_LINKER NAMES ${TOOLCHAIN_PREFIX}g++ HINTS ${_ARM_TOOLCHAIN_HINTS} PATH_SUFFIXES bin REQUIRED)
find_program(CMAKE_OBJCOPY NAMES ${TOOLCHAIN_PREFIX}objcopy HINTS ${_ARM_TOOLCHAIN_HINTS} PATH_SUFFIXES bin REQUIRED)
find_program(CMAKE_SIZE NAMES ${TOOLCHAIN_PREFIX}size HINTS ${_ARM_TOOLCHAIN_HINTS} PATH_SUFFIXES bin REQUIRED)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# MCU specific flags
set(TARGET_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard ")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -fdata-sections -ffunction-sections")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_C_LINK_FLAGS "${TARGET_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${CMAKE_SOURCE_DIR}/STM32F407XX_FLASH.ld\"")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --specs=nano.specs")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lc -lm -Wl,--end-group")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--print-memory-usage")

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group")
