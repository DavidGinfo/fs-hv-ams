# Integrate ECU:
To add a new ECU to the software project, some steps are needed in order to generate runable code.
This guide covers only setting up ECUs that already have their uController Integrated in our Project.

## CubeMX.ioc
Make sure the following settings are as follows:
- Pinout > System Core > SYS > Timebase Source > Select last timer available
- Pinout > System Core > NVIC > Code generation >  disable the following:
  - System Servuce call via SWI instruction
  - Pendable request for system service
  - System Tick Timer
- Project Manager > Project > Toolchain / IDE > MDK-ARM

After the settings have been applied you can click on `GENERATE CODE` and all necesary files should be generated.

> If you are missing the `STM32xxx_FLASH.ld` and `STM32xxx_RAM.ld` files, change `Project Manager > Project > Toolchain / IDE` to `STM32CubeIDE` and Generate code again, after you can switch back to `MDK-ARM`.

## Edit Generated Files
After all the files have been successfully generated, we need to edit some of them as follows:

### Core/include/main.h
    /* USER CODE BEGIN Private defines */
    int initHAL();                       // <-- Add this
    /* USER CODE END Private defines */

### Core/src/main.c
    /* Private define */
    /* USER CODE BEGIN PD */
    #define main initHAL    // <-- Add this
    /* USER CODE END PD */

...

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
      /* USER CODE END WHILE */
      /* USER CODE BEGIN 3 */
      break;                    // <-- Add the break
    }
    /* USER CODE END 3 */

### Core/src/stm32XXX_it.c
The name of the file depends on wich uController is beeing used.

    /* Private includes */
    /* USER CODE BEGIN Includes */
    #include <util/Interrupt.h>     // <-- Add this 
    /* USER CODE END Includes */