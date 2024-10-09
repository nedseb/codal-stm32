# Codal target for STM32 MCU [![codal-buildtarget](https://img.shields.io/github/actions/workflow/status/letssteam/codal-stm32/codal-buildtarget.yml?branch=main&label=codal-stm32)](https://github.com/letssteam/codal-stm32/actions)

[![codal-stm32-DISCO_L475VG_IOT](https://img.shields.io/github/actions/workflow/status/letssteam/codal-stm32-DISCO_L475VG_IOT/codal-buildtarget.yml?branch=main&label=codal-stm32-DISCO_L475VG_IOT)](https://github.com/letssteam/codal-stm32-DISCO_L475VG_IOT)

[![codal-stm32-STEAM32_WB55RG](https://img.shields.io/github/actions/workflow/status/letssteam/codal-stm32-STEAM32_WB55RG/codal-buildtarget.yml?branch=main&label=codal-stm32-STEAM32_WB55RG)](https://github.com/letssteam/codal-stm32-STEAM32_WB55RG/)

[![codal-stm32-PNUCLEO_WB55RG](https://img.shields.io/github/actions/workflow/status/letssteam/codal-stm32-PNUCLEO_WB55RG/codal-buildtarget.yml?branch=main&label=codal-stm32-PNUCLEO_WB55RG)](https://github.com/letssteam/codal-stm32-PNUCLEO_WB55RG)

This repository provides the Codal library for the STM32 family of microcontrollers. It contains the drivers and core functionalities necessary to work with STM32-based boards, enabling developers to create applications using the Codal runtime.

## Overview

Codal STM32 is part of the Codal (Component-oriented Device Abstraction Layer) project, which aims to simplify embedded software development by providing a uniform interface for different microcontrollers. This library specifically targets the STM32 family, making it possible to leverage the full capabilities of these popular microcontrollers through Codal's high-level APIs.

This porting is based on:

- [STM32Cube MCU Packages](https://www.st.com/en/embedded-software/stm32cube-mcu-packages.html) including:
  - The HAL hardware abstraction layer, enabling portability between different STM32 devices via standardized API calls
  - The Low-Layer (LL) APIs, a light-weight, optimized, expert oriented set of APIs designed for both performance and runtime efficiency
  - CMSIS device defintion for STM32
- [CMSIS](https://developer.arm.com/embedded/cmsis): Cortex Microcontroller Software Interface Standard (CMSIS) is a vendor-independent hardware abstraction layer for the Cortex®-M processor series and defines generic tool interfaces. It has been packaged as a module for Arduino IDE: https://github.com/stm32duino/ArduinoModule-CMSIS
- [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm): Arm Embedded GCC compiler, libraries and other GNU tools necessary for bare-metal software development on devices based on the Arm Cortex-M. Packages are provided thanks [The xPack GNU Arm Embedded GCC](https://xpack.github.io/arm-none-eabi-gcc/): https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack

### Key Features

- **Unified Interface for STM32 Devices**: The library provides a consistent API for STM32 devices, allowing easy integration with Codal's ecosystem.
- **Hardware Abstraction**: It abstracts the hardware details, allowing developers to focus on building applications without needing to manage low-level operations.
- **Driver Support**: Includes drivers for various peripherals and features specific to STM32 microcontrollers, such as GPIO, I2C, SPI, PWM, UART, and ADC.
- **Compatibility**: Supports a wide range of STM32 chips, providing flexibility for different hardware configurations.

## Prerequisites

- **Toolchain**: You will need a GCC ARM toolchain installed to compile the library.
- **CMake**: CMake is used to generate the build files needed for compilation.

## Getting Started

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/letssteam/codal-stm32.git
   cd codal-stm32
   ```

2. **Set Up Dependencies**:

   - Install the required toolchain and ensure it is accessible through your system's PATH.

3. **Build the Library**:
   - Create a build directory:
     ```bash
     mkdir build && cd build
     ```
   - Run CMake to generate the build files:
     ```bash
     cmake ..
     ```
   - Compile the library:
     ```bash
     make
     ```

## Integration with Projects

Once compiled, you can link the resulting library with your application project. The Codal STM32 library provides a simplified interface to interact with various peripherals of the STM32, making it ideal for educational projects, prototyping, and even complex embedded systems.

## Supported Features and Drivers

- **Digital I/O**: GPIO control for input and output.
- **Analog I/O**: ADC support for reading analog signals.
- **Communication Protocols**:
  - **I2C**: Driver for communication with I2C devices.
  - **SPI**: SPI protocol driver for fast data exchange.
  - **UART**: Serial communication for debugging or data transfer.
- **PWM**: Pulse Width Modulation for motor control, LED dimming, and more.
- **Timers**: Various timers for event management and time tracking.

## Boards available

|    Status     |     MCU      | Nucleo 64                                                                    |  Release  | Comment |
| :-----------: | :----------: | ---------------------------------------------------------------------------- | :-------: | :------ |
| :green_heart: | STM32WB55RGV | [P-Nucleo-WB55RG](https://www.st.com/en/evaluation-tools/p-nucleo-wb55.html) | **0.0.1** |         |

|    Status     |      MCU      | Discovery                                                                   |  Release  | Comment |
| :-----------: | :-----------: | --------------------------------------------------------------------------- | :-------: | :------ |
| :green_heart: | STM32L475VGTx | [B-L475E-IOT01A](http://www.st.com/en/evaluation-tools/b-l475e-iot01a.html) | **0.0.1** |         |

|    Status     |     MCU      | Name                             |  Release  | Notes |
| :-----------: | :----------: | -------------------------------- | :-------: | :---- |
| :green_heart: | STM32WB55RGV | [STeaMi](https://www.steami.cc/) | **0.0.1** |       |

## Troubleshooting

For question, support, ..., you could submit a topic on the [gitter](https://gitter.im/letssteam/community).

If you have any issue, you could [file an issue on Github](https://github.com/letssteam/codal-stm32/issues/new).

In any case, it always fine to search if your issue was not already existing before submit a new one.

## Contributions

We welcome contributions to improve the Codal STM32 library. Please feel free to open an issue for discussion or submit a pull request with your improvements.

## License

This project is licensed under the MIT License. Please see the LICENSE file for more details.

## Useful Resources

- [Codal Project Documentation](https://lancaster-university.github.io/codal)
- [STM32 Official Documentation](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html)
