cmake_minimum_required(VERSION 3.5)

include(cmake/CPM.cmake)
CPMAddPackage("gh:raysan5/raylib#5.0")

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(calcos C ASM)

# CPU-configurations
set(MCU   atmega328p)
set(F_CPU 16000000)
set(BAUD  9600)
add_definitions(-DF_CPU=${F_CPU})

# Toolchain
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)

set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_C_STANDARD_REQUIRED 99)

if(TARGET ${PROJECT_NAME}-avr) 
    set(CMAKE_CXX_COMPILER ${AVRCPP})
    set(CMAKE_C_COMPILER   ${AVRC})
    set(CMAKE_ASM_COMPILER   ${AVRC})

    # Compiler flags
    set(CSTANDARD "-std=gnu${CMAKE_C_STANDARD_REQUIRED}")
    set(CWARN     "-Wall")
    set(COPT      "-Os")
    set(CMCU      "-mmcu=${MCU}")
    set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")
endif()

set(SHARED_SRC
    src/gfx.c
    src/screen.c
    src/font.c
    src/screens/home_screen.c
)

set(SHARED_INCLUDES
    src/
    src/screens/
)

include_directories(${PROJECT_NAME}-avr
    ${SHARED_INCLUDES}
)

include_directories(${PROJECT_NAME}-sim
    ${SHARED_INCLUDES}
    ${raylib_SOURCE_DIR}/src
)

add_custom_target(${PROJECT_NAME}-avr
    ${AVRC} ${CMCU} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} src/main-avr.c ${SHARED_SRC} -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-avr.elf
    COMMAND ${OBJCOPY} -j .text -j .data -O ihex ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-avr.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-avr.hex
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(${PROJECT_NAME}-sim ${SHARED_SRC} src/main-sim.c)
target_link_libraries(${PROJECT_NAME}-sim raylib)