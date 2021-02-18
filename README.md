# Codal target for STM32 MCU ![GitHub Workflow Status](https://img.shields.io/github/workflow/status/letssteam/codal-stm32/codal-buildtarget?label=codal-stm32)

[![codal-stm32-DISCO_L475VG_IOT](https://img.shields.io/github/workflow/status/letssteam/codal-stm32-DISCO_L475VG_IOT/codal-buildtarget?label=codal-stm32-DISCO_L475VG_IOT)](https://github.com/letssteam/codal-stm32-DISCO_L475VG_IOT)

![codal-stm32-NUCLEO_F4x1RE](https://img.shields.io/github/workflow/status/letssteam/codal-stm32-NUCLEO_F4x1RE/codal-buildtarget?label=codal-stm32-NUCLEO_F4x1RE)

![codal-stm32-PNUCLEO_WB55RG](https://img.shields.io/github/workflow/status/letssteam/codal-stm32-PNUCLEO_WB55RG/codal-buildtarget?label=codal-stm32-PNUCLEO_WB55RG)

Codal target for the STM32 familly. Codal is the core set of drivers, mechanisms and types that make up the runtime for a board in Makecode.

## Overview

The codal runtime provides an easy to use environment for programming the board in the C/C++ language, written by Lancaster University. It contains device drivers for all the hardware capabilities, and also a suite of runtime mechanisms to make programming the easier and more flexible. These range from control of the LED matrix display to peer-to-peer radio communication and secure Bluetooth Low Energy services.

In addition to supporting development in C/C++, the runtime is also designed specifically to support higher level languages provided by our partners that target physical computing and computer science education. It is currently used as a support library for Microsoft MakeCode

Codal-core must be implemented by third party developper to support new hardware target. This repository constain the codal implementation for the STM32 MCU familly.

This porting is based on:
* [STM32Cube MCU Packages](https://www.st.com/en/embedded-software/stm32cube-mcu-packages.html) including:
    * The HAL hardware abstraction layer, enabling portability between different STM32 devices via standardized API calls
    * The Low-Layer (LL) APIs, a light-weight, optimized, expert oriented set of APIs designed for both performance and runtime efficiency
    * CMSIS device defintion for STM32
* [CMSIS](https://developer.arm.com/embedded/cmsis): Cortex Microcontroller Software Interface Standard (CMSIS) is a vendor-independent hardware abstraction layer for the Cortex®-M processor series and defines generic tool interfaces. It has been packaged as a module for Arduino IDE: https://github.com/stm32duino/ArduinoModule-CMSIS
* [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm): Arm Embedded GCC compiler, libraries and other GNU tools necessary for bare-metal software development on devices based on the Arm Cortex-M. Packages are provided thanks [The xPack GNU Arm Embedded GCC](https://xpack.github.io/arm-none-eabi-gcc/): https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack


## Boards available

| Status | [Nucleo 144](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-nucleo-boards.html) | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [Nucleo F207ZG](http://www.st.com/en/evaluation-tools/nucleo-f207zg.html) |   |  |
| :broken_heart: | [Nucleo F429ZI](http://www.st.com/en/evaluation-tools/nucleo-f429zi.html) |   |  |
| :broken_heart: | [Nucleo F767ZI](http://www.st.com/en/evaluation-tools/nucleo-f767zi.html) |   |  |
| :broken_heart: | [Nucleo F746ZG](https://www.st.com/en/evaluation-tools/nucleo-f746zg.html) |   |  |
| :broken_heart: | [Nucleo F756ZG](https://www.st.com/en/evaluation-tools/nucleo-f756zg.html) |   |  |
| :broken_heart: | [Nucleo L496ZG](http://www.st.com/en/evaluation-tools/nucleo-l496zg.html) |   |  |
| :broken_heart: | [Nucleo L496ZG-P](http://www.st.com/en/evaluation-tools/nucleo-l496zg-p.html) |   |  |
| :broken_heart: | [Nucleo L4R5ZI](http://www.st.com/en/evaluation-tools/nucleo-l4r5zi.html) |   |  |
| :broken_heart: | [Nucleo L4R5ZI-P](http://www.st.com/en/evaluation-tools/nucleo-l4r5zi-p.html) |   |  |
| :broken_heart: | [Nucleo H743ZI(2)](https://www.st.com/en/evaluation-tools/nucleo-h743zi.html) |   |  |

| Status | [Nucleo 64](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-nucleo-boards.html) | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [Nucleo F030R8](http://www.st.com/en/evaluation-tools/nucleo-f030r8.html) |   |  |
| :broken_heart: | [Nucleo F091RC](http://www.st.com/en/evaluation-tools/nucleo-f091rc.html) |   |  |
| :broken_heart: | [Nucleo F103RB](http://www.st.com/en/evaluation-tools/nucleo-f103rb.html) |   |  |
| :broken_heart: | [Nucleo F302R8](http://www.st.com/en/evaluation-tools/nucleo-f302r8.html) |   |  |
| :broken_heart: | [Nucleo F303RE](http://www.st.com/en/evaluation-tools/nucleo-f303re.html) |   |  |
| :broken_heart: | [Nucleo F401RE](http://www.st.com/en/evaluation-tools/nucleo-f401re.html) |   |  |
| :broken_heart: | [Nucleo F411RE](http://www.st.com/en/evaluation-tools/nucleo-f411re.html) |   |  |
| :broken_heart: | [Nucleo F446RE](http://www.st.com/en/evaluation-tools/nucleo-f446re.html) |   |  |
| :broken_heart: | [Nucleo G071RB](https://www.st.com/en/evaluation-tools/nucleo-g071rb.html) |   |  |
| :broken_heart: | [Nucleo G431RB](https://www.st.com/en/evaluation-tools/nucleo-g431rb.html) |   |  |
| :broken_heart: | [Nucleo G474RE](https://www.st.com/en/evaluation-tools/nucleo-g474re.html) |   |  |
| :broken_heart: | [Nucleo L053R8](http://www.st.com/en/evaluation-tools/nucleo-l053r8.html) |   |  |
| :broken_heart: | [Nucleo L073RZ](http://www.st.com/en/evaluation-tools/nucleo-l073rz.html) |   |  |
| :broken_heart: | [Nucleo L152RE](http://www.st.com/en/evaluation-tools/nucleo-l152re.html) |   |  |
| :broken_heart: | [Nucleo L452RE](http://www.st.com/en/evaluation-tools/nucleo-l452re.html) |   |  |
| :broken_heart: | [Nucleo L452RE-P](http://www.st.com/en/evaluation-tools/nucleo-l452re-p.html) |   |  |
| :broken_heart: | [Nucleo L476RG](http://www.st.com/en/evaluation-tools/nucleo-l476rg.html) |   |  |
| :yellow_heart: | [P-Nucleo-WB55RG](https://www.st.com/en/evaluation-tools/p-nucleo-wb55.html) | **0.0.1** | |

| Status | [Nucleo 32](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-nucleo-boards.html) | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [Nucleo F031K6](https://www.st.com/en/evaluation-tools/nucleo-f031k6.html) |   |  |
| :broken_heart: | [Nucleo F303K8](http://www.st.com/en/evaluation-tools/nucleo-f303k8.html) |   |  |
| :broken_heart: | [Nucleo G431KB](https://www.st.com/en/evaluation-tools/nucleo-g431kb.html) |   |  |
| :broken_heart: | [Nucleo L031K6](http://www.st.com/en/evaluation-tools/nucleo-l031k6.html) |   |  |
| :broken_heart: | [Nucleo L412KB](http://www.st.com/en/evaluation-tools/nucleo-l412kb.html) |   |  |
| :broken_heart: | [Nucleo L432KC](http://www.st.com/en/evaluation-tools/nucleo-l432kc.html) |   |  |

| Status | [Discovery](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-discovery-kits.html) | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [32F0308DISCOVERY](http://www.st.com/en/evaluation-tools/32f0308discovery.html) |   |  |
| :broken_heart: | [32F072BDISCOVERY](https://www.st.com/en/evaluation-tools/32f072bdiscovery.html) |   |  |
| :broken_heart: | [STM32VLDISCOVERY](https://www.st.com/en/evaluation-tools/stm32vldiscovery.html) |  |  |
| :broken_heart: | [STM32F407G-DISC1](http://www.st.com/en/evaluation-tools/stm32f4discovery.html) |   |  |
| :broken_heart: | [STM32F746G-DISCOVERY](http://www.st.com/en/evaluation-tools/32f746gdiscovery.html) |   |  |
| :broken_heart: | [STM32G0316-DISCO](https://www.st.com/en/evaluation-tools/stm32g0316-disco.html) |   |  |
| :broken_heart: | [B-L072Z-LRWAN1](http://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html) |   |  |
| :yellow_heart: | [B-L475E-IOT01A](http://www.st.com/en/evaluation-tools/b-l475e-iot01a.html) | **0.0.1** |  |
| :broken_heart: | [32F413HDISCOVERY](https://www.st.com/en/evaluation-tools/32f413hdiscovery.html) |   |  |

| Status | [Eval](https://www.st.com/en/evaluation-tools/stm32-eval-boards.html) | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [STEVAL-MKSBOX1V1 (SensorTile.box)](https://www.st.com/en/evaluation-tools/steval-mksbox1v1.html) |   |  |

| Status | [STM32MP1 series coprocessor](https://www.st.com/content/st_com/en/products/microcontrollers-microprocessors/stm32-arm-cortex-mpus/stm32mp1-series.html) | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [STM32MP157A-DK1](https://www.st.com/en/evaluation-tools/stm32mp157a-dk1.html) |   | |
| :broken_heart: | [STM32MP157C-DK2](https://www.st.com/en/evaluation-tools/stm32mp157c-dk2.html) |   | |


| Status | Generic STM32F0xx | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [STM32F030F4 Demo board](https://stm32-base.org/boards/STM32F030F4P6-STM32F030-DEMO-BOARD-V1.1) |   |  |

| Status | Generic STM32F1xx | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [BluePill F103C(6-8-B)](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill) |   |  |
| :broken_heart: | [BlackPill F103C(8-B)](https://stm32-base.org/boards/STM32F103C8T6-Black-Pill) |   |  |
| :broken_heart: | [Generic F103R(8-B-C-E)T6](https://stm32-base.org/boards/STM32F103RET6-Generic-Board) |   |  |
| :broken_heart: | Generic F103Z(C-D-E-F-G) |   |  |
| :broken_heart: | HY-TinySTM103T |   |  |
| :broken_heart: | MapleMini F103CB |   | |
| :broken_heart: | [Vcc-Gnd F103ZET6](http://www.vcc-gnd.com/rtd/html/STM32/quickref.html#stm32f1) |   | |


| Status | Generic STM32F3xx | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [RobotDyn BlackPill F303CC](https://robotdyn.com/catalog/development-boards/stm-boards-and-shields.html) |   |  |

| Status | Generic STM32F4xx | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [Adafruit Feather STM32F405 Express](https://www.adafruit.com/product/4382) |   |  |
| :broken_heart: | BlackPill F401CC |   |  |
| :broken_heart: | BlackPill F411CE |   |  |
| :broken_heart: | [Black F407VET6](https://stm32-base.org/boards/STM32F407VET6-STM32-F4VE-V2.0) |   | |
| :broken_heart: | [Blue F407VET6 Mini](https://stm32-base.org/boards/STM32F407VET6-VCC-GND-Small) |   |  |
| :broken_heart: | Core Board F401RC |   |  |
| :broken_heart: | [DIYMORE STM32F407VGT](https://stm32-base.org/boards/STM32F407VGT6-diymore) |   |  |
| :broken_heart: | FK407M1 |   |  |
| :broken_heart: | Generic F401C(B-C-D-E) |   |  |
| :broken_heart: | Generic F401R(B-C-D-E) |   |  |
| :broken_heart: | Generic F405RG |   |  |
| :broken_heart: | Generic F407V(E-G) |   |  |
| :broken_heart: | Generic F410C(8-B) |   |  |
| :broken_heart: | Generic F410R(8-B) |   |  |
| :broken_heart: | Generic F411C(C-E) |   |  |
| :broken_heart: | Generic F411R(C-E) |   |  |
| :broken_heart: | Generic F412C(E-G) |   |  |
| :broken_heart: | Generic F412R(E-G) |   |  |
| :broken_heart: | Generic F413C(G-H) |   |  |
| :broken_heart: | Generic F413R(G-H) |   |  |
| :broken_heart: | Generic F415RG |   |  |
| :broken_heart: | Generic F417V(E-G) |   |  |
| :broken_heart: | Generic F423CH |   |  |
| :broken_heart: | Generic F423RH |   |  |
| :broken_heart: | Generic F446R(C-E) |   |  |
| :broken_heart: | [ThunderPack v1.1+](https://github.com/jgillick/ThunderPack) |   | |

| Status | Generic STM32L0xx | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [PX-HER0](https://piconomix.com/fwlib/_p_i_c_o_n_o_m_i_x__s_t_m32__h_e_r_o__b_o_a_r_d.html) |   |  |
| :broken_heart: | [ThunderPack v1.0](https://github.com/jgillick/ThunderPack) |   |  |

| Status | LoRa boards | Release | Comment |
| :---: | --- | :---: | :--- |
| :broken_heart: | [RAK811 LoRa Tracker](https://www.rakwireless.com/en/) |   |  |
| :broken_heart: | [RHF76-052](https://www.ai-thinker.com/) |   | |

## Installation

Before using this target you need to configure your platforms with some software.
Codal is also a build system to simplify as much as possible the experience of novice users.  

1. Install `git`, ensure it is available on your platforms path.
2. Install the `arm-none-eabi-*` command line utilities for ARM based devices.
3. Install [CMake](https://cmake.org)(Cross platform make), this is the entirety of the build system.
4. Install `Python 2.7` (if you are unfamiliar with CMake), python scripts are used to simplify the build process.
5. Clone the repository <https://github.com/lancaster-university/codal>

## Building

- Generate or create a `codal.json` file
  - `python build.py codal-stm32` generates a codal.json file for our target
- In the root of this repository type `python build.py` the `-c` option cleans before building.

## Next release

  See [milestones](https://github.com/letssteam/codal-stm32/milestones) to have an overview of the next release content.

## Troubleshooting

For question, support, ...,  you could submit a topic on the [gitter](https://gitter.im/letssteam/community).

If you have any issue, you could [file an issue on Github](https://github.com/letssteam/codal-stm32/issues/new).

In any case, it always fine to search if your issue was not already existing before submit a new one.
