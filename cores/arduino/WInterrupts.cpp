#include "Arduino.h"
#include "fsl_gpio.h"
#include "wiring_private.h"

typedef struct
{
    IRQn irqnb;
    std::function<void(void)> callback;
} gpio_irq_conf_str;

#define NB_EXTI (10)

static gpio_irq_conf_str gpio_irq_conf[NB_EXTI] = {
    {.irqnb = GPIO1_Combined_0_15_IRQn,  .callback = NULL},
    {.irqnb = GPIO1_Combined_16_31_IRQn, .callback = NULL},
    {.irqnb = GPIO2_Combined_0_15_IRQn,  .callback = NULL},
    {.irqnb = GPIO2_Combined_16_31_IRQn, .callback = NULL},
    {.irqnb = GPIO3_Combined_0_15_IRQn,  .callback = NULL},
    {.irqnb = GPIO3_Combined_16_31_IRQn, .callback = NULL},
    {.irqnb = GPIO4_Combined_0_15_IRQn,  .callback = NULL},
    {.irqnb = GPIO4_Combined_16_31_IRQn, .callback = NULL},
    {.irqnb = GPIO5_Combined_0_15_IRQn,  .callback = NULL},
    {.irqnb = GPIO5_Combined_16_31_IRQn, .callback = NULL}
};

static uint8_t get_irq_pin_map(GPIO_Type *base, uint32_t pin)
{
    uint8_t index = 0;

    if (base == GPIO1 && (pin >= 0 && pin <= 15)) index = 0;
    else if (base == GPIO1 && (pin >= 16 && pin <= 31)) index = 1;
    else if (base == GPIO2 && (pin >= 0 && pin <= 15)) index = 2;
    else if (base == GPIO2 && (pin >= 16 && pin <= 31)) index = 3;
    else if (base == GPIO3 && (pin >= 0 && pin <= 15)) index = 4;
    else if (base == GPIO3 && (pin >= 16 && pin <= 31)) index = 5;
    else if (base == GPIO4 && (pin >= 0 && pin <= 15)) index = 6;
    else if (base == GPIO4 && (pin >= 16 && pin <= 31)) index = 7;
    else if (base == GPIO5 && (pin >= 0 && pin <= 15)) index = 8;
    else if (base == GPIO5 && (pin >= 16 && pin <= 31)) index = 9;

    return index;
}

static uint8_t get_irq_pin_index(uint32_t arduinoPin)
{
    return get_irq_pin_map(g_APinDescription[arduinoPin].GROUP, g_APinDescription[arduinoPin].PIN);
}

void attachInterrupt(uint32_t pin, callback_function_t callback, uint32_t mode)
{
    uint32_t arduinoPin = pin;

    gpio_pin_config_t gpio_config = {
        .direction = kGPIO_DigitalInput, //
        .outputLogic = 0U,               //
        .interruptMode = kGPIO_NoIntmode //
    };

    switch (mode)
    {
    case CHANGE:
        gpio_config.interruptMode = kGPIO_IntRisingOrFallingEdge;
        break;
    case FALLING:
        gpio_config.interruptMode = kGPIO_IntFallingEdge;
        break;
    case LOW:
        gpio_config.interruptMode = kGPIO_IntLowLevel;
        break;
    case RISING:
        gpio_config.interruptMode = kGPIO_IntRisingEdge;
        break;
    case HIGH:
        gpio_config.interruptMode = kGPIO_IntHighLevel;
        break;
    default:
        gpio_config.interruptMode = kGPIO_IntRisingEdge;
        break;
    }

    /*
    转换速率:       No init
    驱动强度:       No init
    速度配置 :      medium(100MHz)
    开漏配置:       Disabled
    拉/保持器配置:  Enable
    拉/保持器选择:  Pull
    上拉/下拉选择:  100K Ohm Pull Up
    滞回器配置:     Enable（仅输入时有效，施密特触发器，使能后可以过滤输入噪声）
    */
    pinPeripheral(arduinoPin, 0U, FUN_GPIO, 0x01B0B0U);
    /* GPIO INIT */
    GPIO_PinInit(g_APinDescription[arduinoPin].GROUP, g_APinDescription[arduinoPin].PIN, &gpio_config);
    /* Enable GPIO pin interrupt */
    GPIO_PortEnableInterrupts(g_APinDescription[arduinoPin].GROUP, 1U << g_APinDescription[arduinoPin].PIN);

    uint8_t id = get_irq_pin_index(arduinoPin);
    gpio_irq_conf[id].callback = callback;

    EnableIRQ(gpio_irq_conf[id].irqnb);
}

void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
    callback_function_t _c = callback;
    attachInterrupt(pin, _c, mode);
}

void detachInterrupt(uint32_t pin)
{
    uint32_t arduinoPin = pin;
    uint8_t id = get_irq_pin_index(arduinoPin);

    gpio_irq_conf[id].callback = NULL;

    for (int i = 0; i < NB_EXTI; i++)
    {
        if (gpio_irq_conf[id].irqnb == gpio_irq_conf[i].irqnb && gpio_irq_conf[i].callback != NULL)
        {
            return;
        }
    }

    GPIO_PortDisableInterrupts(g_APinDescription[arduinoPin].GROUP, 1U << g_APinDescription[arduinoPin].PIN);
    DisableIRQ(gpio_irq_conf[id].irqnb);
}

/**
 * @brief Arduino中断回调函数
 *
 * @param base
 * @param pin
 */
void GPIO_EXTI_Callback(GPIO_Type *base, uint32_t pin)
{
    uint8_t irq_id = get_irq_pin_map(base, pin);

    if (gpio_irq_conf[irq_id].callback != NULL)
    {
        gpio_irq_conf[irq_id].callback();
    }
}

#ifdef __cplusplus
extern "C" {
#endif

// static inline uint32_t get_pin_interrupt_flag(GPIO_Type *base, uint32_t pin)
// {
//     return (((base->ISR) >> pin) & 0x1U);
// }

// static uint32_t get_pin_interrupt_flag(GPIO_Type *base, uint32_t pin)
// {
//     return (base->ISR & (1 << pin)) & (base->IMR & (1 << pin));
// }

/**
 * @brief Get the pin interrupt flag object
 *
 * @param base
 * @param pin
 * @return uint32_t
 */
static uint32_t get_pin_interrupt_flag(GPIO_Type *base, uint32_t pin)
{
    // ISR(中断状态寄存器)和IMR(中断使能寄存器)同时为 1 对应引脚的中断触发
    return (((base->ISR >> pin) & 0x1U) & ((base->IMR >> pin) & 0x1U));
}

/**
 * @brief 所有中断统一处理函数
 *
 * @param base
 * @param pin
 */
static void gpio_irq_handler(GPIO_Type *base, uint32_t pin)
{
    if (1U == get_pin_interrupt_flag(base, pin))
    {
        /* clear the interrupt status */
        GPIO_PortClearInterruptFlags(base, 1U << pin);
        /* Call Callback Func */
        GPIO_EXTI_Callback(base, pin);
        /* Sync Cache */
        SDK_ISR_EXIT_BARRIER;
    }
}

// ----------------------------------------------------------------------
void GPIO1_Combined_0_15_IRQHandler(void)
{
    for (uint32_t pin = 0; pin <= 15; pin++) gpio_irq_handler(GPIO1, pin);
}


void GPIO1_Combined_16_31_IRQHandler(void)
{
#ifdef USE_AICAMERA    
    extern void Compatible_GPIO1_Combined_16_31_IRQHandler(void);
    Compatible_GPIO1_Combined_16_31_IRQHandler();
#endif

    for (uint32_t pin = 16; pin <= 31; pin++) gpio_irq_handler(GPIO1, pin);
}

// ----------------------------------------------------------------------
void GPIO2_Combined_0_15_IRQHandler(void)
{
    for (uint32_t pin = 0; pin <= 15; pin++) gpio_irq_handler(GPIO2, pin);
}

void GPIO2_Combined_16_31_IRQHandler(void)
{
    for (uint32_t pin = 16; pin <= 31; pin++) gpio_irq_handler(GPIO2, pin);
}

// ----------------------------------------------------------------------
void GPIO3_Combined_0_15_IRQHandler(void)
{
    for (uint32_t pin = 0; pin <= 15; pin++) gpio_irq_handler(GPIO3, pin);
}

void GPIO3_Combined_16_31_IRQHandler(void)
{
    for (uint32_t pin = 16; pin <= 31; pin++) gpio_irq_handler(GPIO3, pin);
}

// ----------------------------------------------------------------------
void GPIO4_Combined_0_15_IRQHandler(void)
{
    for (uint32_t pin = 0; pin <= 15; pin++) gpio_irq_handler(GPIO4, pin);
}

void GPIO4_Combined_16_31_IRQHandler(void)
{
    for (uint32_t pin = 16; pin <= 31; pin++) gpio_irq_handler(GPIO4, pin);
}

// ----------------------------------------------------------------------
void GPIO5_Combined_0_15_IRQHandler(void)
{
    for (uint32_t pin = 0; pin <= 15; pin++) gpio_irq_handler(GPIO5, pin);
}

void GPIO5_Combined_16_31_IRQHandler(void)
{
    for (uint32_t pin = 16; pin <= 31; pin++) gpio_irq_handler(GPIO5, pin);
}


// void GPIO3_Combined_0_15_IRQHandler(void)
// {
//     for (uint32_t pin = 0; pin <= 15; pin++)
//     {
//         if (1U == get_pin_interrupt_flag(GPIO3, pin))
//         {
//             /* clear the interrupt status */
//             GPIO_PortClearInterruptFlags(GPIO3, 1U << pin);
//             /* Call Callback Func */
//             GPIO_EXTI_Callback(GPIO3, pin);
//             /* Sync Cache */
//             SDK_ISR_EXIT_BARRIER;
//         }
//     }
// }

#ifdef __cplusplus
}
#endif

// .long   GPIO1_Combined_0_15_IRQHandler                  /* Combined interrupt indication for GPIO1 signal 0 throughout 15*/
// .long   GPIO1_Combined_16_31_IRQHandler                 /* Combined interrupt indication for GPIO1 signal 16 throughout 31*/
// .long   GPIO2_Combined_0_15_IRQHandler                  /* Combined interrupt indication for GPIO2 signal 0 throughout 15*/
// .long   GPIO2_Combined_16_31_IRQHandler                 /* Combined interrupt indication for GPIO2 signal 16 throughout 31*/
// .long   GPIO3_Combined_0_15_IRQHandler                  /* Combined interrupt indication for GPIO3 signal 0 throughout 15*/
// .long   GPIO3_Combined_16_31_IRQHandler                 /* Combined interrupt indication for GPIO3 signal 16 throughout 31*/
// .long   GPIO4_Combined_0_15_IRQHandler                  /* Combined interrupt indication for GPIO4 signal 0 throughout 15*/
// .long   GPIO4_Combined_16_31_IRQHandler                 /* Combined interrupt indication for GPIO4 signal 16 throughout 31*/
// .long   GPIO5_Combined_0_15_IRQHandler                  /* Combined interrupt indication for GPIO5 signal 0 throughout 15*/
// .long   GPIO5_Combined_16_31_IRQHandler                 /* Combined interrupt indication for GPIO5 signal 16 throughout 31*/
