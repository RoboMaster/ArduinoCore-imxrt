#include "Arduino.h"

#include "clock_config.h"
#include "pin_mux.h"
// #include "virtual_com.h"

#include "board.h"
#include "fsl_adc.h"

#include "user_main.h"

#include "FreeRTOS.h"
#include "task.h"

void adc_init()
{
    // Initialize Analog Controller
    adc_config_t adcConfigStrcut;
    /*  config->enableAsynchronousClockOutput = true;
     *  config->enableOverWrite =               false;
     *  config->enableContinuousConversion =    false;
     *  config->enableHighSpeed =               false;
     *  config->enableLowPower =                false;
     *  config->enableLongSample =              false;
     *  config->referenceVoltageSource =        kADC_ReferenceVoltageSourceVref;
     *  config->samplePeriodMode =              kADC_SamplePeriod2or12Clocks;
     *  config->clockSource =                   kADC_ClockSourceAD;
     *  config->clockDriver =                   kADC_ClockDriver1;
     *  config->resolution =                    kADC_Resolution12Bit;
     */
    ADC_GetDefaultConfig(&adcConfigStrcut);

    ADC_Init(ADC1, &adcConfigStrcut);
    ADC_Init(ADC2, &adcConfigStrcut);

#if !(defined(FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE) && FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE)
    ADC_EnableHardwareTrigger(ADC1, false);
    ADC_EnableHardwareTrigger(ADC2, false);
#endif

    // waitting for ADC Auto Calibiration
    while (!(kStatus_Success == ADC_DoAutoCalibration(ADC1)))
    {
    }
    while (!(kStatus_Success == ADC_DoAutoCalibration(ADC2)))
    {
    }
}

void init(void)
{
    /* Init board hardware. */
    // BOARD_ConfigMPU();
    // BOARD_InitBootPins();
    // BOARD_InitBootClocks();

    /* Debug Uart1 */
    // BOARD_InitDebugConsole();

    // PRINTF("DebugConsole Start !!!!\n");

    /* ADC first init */
    // adc_init();

    /* Init usb cdc. */
    // vcom_cdc_init();

    internal_fun_start();
}

/*!
 * @brief tick hook is executed every tick.
 */
void vApplicationTickHook(void)
{
    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static uint32_t ulCount = 0;

    /* The RTOS tick hook function is enabled by setting configUSE_TICK_HOOK to
    1 in FreeRTOSConfig.h.

    "Give" the semaphore on every 500th tick interrupt. */
    ulCount++;
    if (ulCount >= 500UL)
    {
        /* This function is called from an interrupt context (the RTOS tick
        interrupt),    so only ISR safe API functions can be used (those that end
        in "FromISR()".

        xHigherPriorityTaskWoken was initialised to pdFALSE, and will be set to
        pdTRUE by xSemaphoreGiveFromISR() if giving the semaphore unblocked a
        task that has equal or higher priority than the interrupted task. */
        // xSemaphoreGiveFromISR(xEventSemaphore, &xHigherPriorityTaskWoken);
        ulCount = 0UL;
    }

    /* If xHigherPriorityTaskWoken is pdTRUE then a context switch should
    normally be performed before leaving the interrupt (because during the
    execution of the interrupt a task of equal or higher priority than the
    running task was unblocked).  The syntax required to context switch from
    an interrupt is port dependent, so check the documentation of the port you
    are using.

    In this case, the function is running in the context of the tick interrupt,
    which will automatically check for the higher priority task to run anyway,
    so no further action is required. */
}

/*!
 * @brief Malloc failed hook.
 */
void vApplicationMallocFailedHook(void)
{
    /* The malloc failed hook is enabled by setting
    configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

    Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
    for (;;)
        ;
}

/*!
 * @brief Stack overflow hook.
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)pcTaskName;
    (void)xTask;

    /* Run time stack overflow checking is performed if
    configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected.  pxCurrentTCB can be
    inspected in the debugger if the task name passed into this function is
    corrupt. */
    for (;;)
        ;
}

/*!
 * @brief Idle hook.
 */
void vApplicationIdleHook(void)
{
    volatile size_t xFreeStackSpace;

    /* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
    FreeRTOSConfig.h.

    This function is called on each cycle of the idle task.  In this case it
    does nothing useful, other than report the amount of FreeRTOS heap that
    remains unallocated. */
    xFreeStackSpace = xPortGetFreeHeapSize();

    if (xFreeStackSpace > 100)
    {
        /* By now, the kernel has allocated everything it is going to, so
        if there is a lot of heap remaining unallocated then
        the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
        reduced accordingly. */
    }
}