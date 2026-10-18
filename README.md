# Formula Student High-Voltage BMS Software

This repository contains an exported excerpt of a GitLab project for the
high-voltage accumulator management system (AMS/BMS) of a Formula Student
vehicle.

It is intended primarily as a reference for the design and implementation of a
Formula Student BMS. It is **not a standalone, immediately usable firmware
project**: the files were exported from a different development infrastructure,
and parts of the original repository, tooling, generated files, configuration,
and integration environment are not included here.

## What is included

The repository brings together three related layers:

- A fully implemented driver for the Texas Instruments **BQ7961x battery
  monitor IC family**, represented here by the BQ79612 implementation.
  It includes register definitions, transaction and response-frame handling,
  CRC calculation, daisy-chain communication, auto-addressing, ADC
  configuration, cell-voltage and temperature acquisition, die-temperature
  measurement, and fault-summary access.
- A reusable embedded software library with functionality used by a
  high-voltage BMS, including STM32/CMSIS and HAL integration, FreeRTOS
  support, CAN abstractions, generated CAN message handling, EEPROM and
  e-fuse interfaces, GPIO/UART/SPI/I2C support, timing, interrupts, and
  common utilities.
- STM32 integration examples showing how the driver and library are connected
  to a microcontroller application. The NUCLEO-G474RE project integrates the
  BQ79612 shield code, UART communication, RTOS tasks, CAN signals, and
  generated Simulink BMS model code.

## Repository structure

| Path | Purpose |
| --- | --- |
| [`ECUs/`](ECUs/) | Microcontroller application projects and their STM32-specific integration. |
| [`ECUs/NUCLEO-G474RE/`](ECUs/NUCLEO-G474RE/) | Main BQ79612 shield integration example for an STM32G474RE Nucleo target. |
| [`ECUs/NUCLEO-G474RE/Core/TestModules/BQ79612_Shield/`](ECUs/NUCLEO-G474RE/Core/TestModules/BQ79612_Shield/) | BQ79612/BQ7961x driver, register data, communication types, AMS wrapper, and test-model integration. |
| [`ECUs/FCU/`](ECUs/FCU/) | Separate STM32G473-based ECU project with CAN, SPI, e-fuse, EEPROM, ADC, and fuse-related application code. |
| [`Library/`](Library/) | Shared CMake modules, STM32 firmware support, FreeRTOS integration, CAN/EEPROM/e-fuse interfaces, and hardware abstraction helpers. |
| [`CAN/`](CAN/) | CAN database files and the Python-based CAN code generator. |
| [`docs/`](docs/) | Existing development-environment and ECU-integration notes. |
| [`ci-pipeline/`](ci-pipeline/) | Firmware-related CI checking utility. |

## Suggested way to explore the code

If you are using this repository to understand or implement a BMS, the
following order provides a useful path:

1. Read this overview and the [development setup notes](docs/Setup.md) to
   understand the limitations of the export.
2. Start with [`AMS_Shield.cpp`](ECUs/NUCLEO-G474RE/Core/Task/AMS_Shield.cpp)
   to see the application-level sequence.
3. Follow the calls into the
   [`BQ79612_Shield`](ECUs/NUCLEO-G474RE/Core/TestModules/BQ79612_Shield/)
   headers and sources to understand device communication and measurements.
4. Inspect the shared interfaces under
   [`Library/include/`](Library/include/) and the corresponding implementations
   under [`Library/src/`](Library/src/).
5. Review the target CMake file and the
   [ECU integration guide](docs/IntegrateECU.md) to see how generated STM32
   code is connected to the shared software stack.
6. Use the CAN DBC files and generator to understand the communication boundary
   between the BMS application and the rest of the vehicle.

## Important limitations

- This is an exported repository excerpt, not the original GitLab development
  environment.
- The original project structure, dependencies, generated artifacts, board
  configuration, and external tooling may be required to reproduce a complete
  build.
- The included projects target different STM32-based applications and are
  examples of integration rather than a single drop-in BMS firmware.
- Hardware configuration, pin assignments, calibration values, communication
  interfaces, safety concepts, and vehicle-specific requirements must be
  reviewed before adapting any part of the code.
- The code should therefore be treated as an engineering reference and source
  of implementation ideas, not as a claim of production readiness or a
  complete safety case.

## Documentation

- [Development environment setup](docs/Setup.md)
- [Integrating an ECU](docs/IntegrateECU.md)
- [STM32 CubeH7 library module](Library/Modules/Firmware/STM32_CubeH7/README.md)
