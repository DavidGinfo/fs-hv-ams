# IDE Setup

This guide shows you how to set up a development environment for our software suite.

## Prerequisites
- For development
    - CLion is highly recommended and comes with custom integrations
    - You must set CMake to reload sequentially [here](jetbrains://CLion/settings?name=Advanced+Settings)
    - For proper CLion integration: A Python toolchain named `HSK Python` [here](jetbrains://CLion/settings?name=Build%2C+Execution%2C+Deployment--Python+Interpreter)
- For building
    - arm-none-eabi-gcc 12.2Rel1
    - A matching newlib libstdc++
    - CMake >= 3.25
- For flashing
    - A arm-none-eabi- or multiarch-gdb as it is provided by CLion
    - Using J-Link debug probes: The J-Link software pack and drivers
        - The folder containing the JLinkGDBServer must be in the PATH variable
    - Using ST-Link debug probes: STM32CubeIDE and STM32CubeProgrammer with drivers
        - The STM32CubeProgrammer bin directory in the STM32_Cube_Programmer environment variable
        - The ST_LINK_gdbserver directory inside the PATH variable
        - You can find the ST_LINK_gdbserver like this: `STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.win32_VERSION\tools\bin`

## Setup

> TODO: Add Steps to setup CLion properly

## Basic usage

CMake provides a fully configured build environment and prepares all CLion integrations to enable development as-is after cloning.
As part of these integrations the correct build configurations for your project are created automatically.
