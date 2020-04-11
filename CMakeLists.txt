project(codal-stm32)

include("${CODAL_UTILS_LOCATION}")

list(APPEND INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/stm32"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/stm32/LL"
    "${CMAKE_CURRENT_SOURCE_DIR}/variants/${device.device}"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/${device.processor}xx"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/Drivers/${device.processor}xx_HAL_Driver/Inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/Drivers/CMSIS/Device/ST/${device.processor}xx/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/Drivers/CMSIS/Device/ST/${device.processor}xx/Source/Templates/gcc"
)

string(TOLOWER  "${device.processor}xx" PROCESSOR_NAME)

RECURSIVE_FIND_FILE(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/stm32" "*.c??")
LIST(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/asm/CortexContextSwitch.s")
LIST(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/stm32/startup_stm32yyxx.S")
LIST(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/system/Drivers/CMSIS/Device/ST/${device.processor}xx/Source/Templates/system_${PROCESSOR_NAME}.c")

# add them
include_directories(${INCLUDE_DIRS})

# create our target
add_library(codal-stm32 ${SOURCE_FILES})

target_link_libraries(
    codal-stm32 
    codal-core)

# expose it to parent cmake.
target_include_directories(codal-stm32 PUBLIC ${INCLUDE_DIRS})