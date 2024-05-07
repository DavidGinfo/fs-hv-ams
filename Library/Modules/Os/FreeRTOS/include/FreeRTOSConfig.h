/**
* Base RTOS config for all ECUs.
* Configure ECU-specific options in Core/Config/RTOS.h.
*/

/**
 * You can overwrite the default values one by one inside "Os_Config.h"
 * The file should be located inside ECU-Folder/Core/Config
*/
#pragma once
#if __has_include("Os_Config.h")
#include <Os_Config.h>
#endif
#ifndef configUSE_PREEMPTION
#define configUSE_PREEMPTION                     1    // Tasks can be suspended by the RTOS
#endif
#ifndef configSUPPORT_STATIC_ALLOCATION
#define configSUPPORT_STATIC_ALLOCATION          1    // Preferred method
#endif
#ifndef configSUPPORT_DYNAMIC_ALLOCATION
#define configSUPPORT_DYNAMIC_ALLOCATION         1    // Only activated for specific situations
#endif
#ifndef configTICK_RATE_HZ
#define configTICK_RATE_HZ                       ((TickType_t)1000)
#endif
#ifndef configMAX_PRIORITIES
#define configMAX_PRIORITIES                     ( 56 )
#endif
#ifndef configMINIMAL_STACK_SIZE
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)
#endif
#ifndef configTOTAL_HEAP_SIZE
#define configTOTAL_HEAP_SIZE                    ((size_t)2048)
#endif
#ifndef configMAX_TASK_NAME_LEN
#define configMAX_TASK_NAME_LEN                  ( 24 )
#endif
#ifndef configUSE_TRACE_FACILITY
#define configUSE_TRACE_FACILITY                 1
#endif
#ifndef configUSE_16_BIT_TICKS
#define configUSE_16_BIT_TICKS                   0
#endif
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  0    // Might improve performance when enabled
#endif
#ifndef configENABLE_FPU
#define configENABLE_FPU                         1
#endif
#ifndef configUSE_MUTEXES
#define configUSE_MUTEXES 1
#endif
#ifndef configUSE_APPLICATION_TASK_TAG
#define configUSE_APPLICATION_TASK_TAG           1    // Back pointer to Task object
#endif
// TODO MALLOC_FAILED_HOOK

/* Software timer definitions. */
#define configUSE_TIMERS                         1
#define configTIMER_TASK_PRIORITY                ( 2 )
#define configTIMER_QUEUE_LENGTH                 10

// API functions
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_eTaskGetState               1
#define INCLUDE_vApplicationGetIdleTaskMemory            1

// SystemView
#define INCLUDE_xTaskGetIdleTaskHandle      1
#define INCLUDE_pxTaskGetStackStart         1

// CLion FreeRTOS integration
#define configRECORD_STACK_HIGH_ADDRESS     1
#define configQUEUE_REGISTRY_SIZE           10

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
#define configPRIO_BITS         __NVIC_PRIO_BITS
#else
#define configPRIO_BITS         4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

// ECU specific config options
#ifndef config_USE_IDLE_HOOK
#define configUSE_IDLE_HOOK                      0
#endif
#ifndef configUSE_TICK_HOOK
#define configUSE_TICK_HOOK                      0
#endif
// TODO Stack overflow detection

// Timer options
#ifndef TIMER_TASK_STACK_DEPTH
#define configTIMER_TASK_STACK_DEPTH             256
#endif

#if __has_include("FreeRTOSConfig_Base.h")
#include <FreeRTOSConfig_Base.h>
#endif
#include <FreeRTOSConfig_MCU.h>

#ifdef MODULE_SEGGER_SYSTEMVIEW_SRC
#if !MODULE_FREERTOS_STATISTICS_SYSTEMVIEW_INTEGRATION
#error The FreeRTOS SystemView integration is not activated
#endif
#endif

// SystemView include
#if MODULE_FREERTOS_STATISTICS_SYSTEMVIEW_INTEGRATION
#include <SEGGER_SYSVIEW_FreeRTOS.h>
#else
// Contains empty macros so FreeRTOS works without SystemView
#include <SystemViewRTOSDefaults.h>
#endif
