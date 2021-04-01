project(codal-stm32)

include("${CODAL_UTILS_LOCATION}")

#set(CMAKE_CXX_CLANG_TIDY
#    clang-tidy;
#    --extra-arg=-std=c++17;
#    --extra-arg=-Wno-error;
#    --extra-arg=-Wno-register;)

RECURSIVE_FIND_DIR(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/inc" "*.h")
list(APPEND INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/stm32"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/stm32/LL"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/stm32/usb"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/stm32/usb/cdc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/stm32/usb/hid"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/CMSIS/CMSIS/Core/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/Drivers/CMSIS/Device/ST/${device.processor}xx/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/Drivers/CMSIS/Device/ST/${device.processor}xx/Source/Templates/gcc"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/${device.processor}xx"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/Drivers/${device.processor}xx_HAL_Driver/Inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/Drivers/${device.processor}xx_HAL_Driver/Src"
    "${CMAKE_CURRENT_SOURCE_DIR}/variants/${device.device}"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/Middlewares/ST/STM32_USB_Device_Library/Core/Inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/Middlewares/ST/STM32_USB_Device_Library/Core/Src"
    )

string(TOLOWER  "${device.processor}xx" PROCESSOR_NAME)

RECURSIVE_FIND_FILE(C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/" "*.c")
LIST(APPEND C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/asm/CortexContextSwitch.s")
LIST(APPEND C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/stm32/startup_stm32yyxx.S")
LIST(APPEND C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/system/Drivers/CMSIS/Device/ST/${device.processor}xx/Source/Templates/system_${PROCESSOR_NAME}.c")
LIST(APPEND C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/variants/${device.device}/variant.cpp")
LIST(APPEND C_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/variants/${device.device}/PeripheralPins.c")



RECURSIVE_FIND_FILE(CPP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/" "*.c??")

set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -T\"${CMAKE_CURRENT_LIST_DIR}/variants/${device.device}/ldscript.ld\"" PARENT_SCOPE)
set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -T\"${CMAKE_CURRENT_LIST_DIR}/variants/${device.device}/ldscript.ld\"" PARENT_SCOPE)
set(CMAKE_SYSTEM_PROCESSOR "armv7-m" PARENT_SCOPE)

# add them
include_directories(${INCLUDE_DIRS})

# create our target
add_library(codal-stm32 ${C_SOURCE_FILES} ${CPP_SOURCE_FILES})

target_link_libraries(
    codal-stm32 
    codal-core
)

# expose it to parent cmake.
target_include_directories(codal-stm32 PUBLIC ${INCLUDE_DIRS})
