/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-18     robomaster   first version
 */

#include "Common.h"
#include <Arduino.h>
#include <Servo.h>

#include "fsl_adc.h"
#include "fsl_pwm.h"
#include "wiring_private.h"

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)

#define TIMER_ID(_timer)       ((timer_id_e)(_timer))
#define SERVO_TIMER(_timer_id) ((timer16_Sequence_t)(_timer_id))

Servo::Servo()
{
}

uint8_t Servo::attach(int pin, int value)
{
    return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, value);
}

uint8_t Servo::attach(int pin, int min, int max, int value)
{
    this->servoPin = pin;
    this->min = min;
    this->max = max;

    PinDescription const *g_pinDes = &g_APinDescription[pin];

    if (g_pinDes->PWM == NULL) return 0;

    // first use init
    if (!(g_pinDes->PWM->MCTRL & (PWM_MCTRL_RUN((1U << g_pinDes->pwm_submodule)))))
    {
        CLOCK_EnableClock(kCLOCK_Iomuxc); /* iomuxc clock (iomuxc_clk_enable): 0x03U */

        IOMUXC_SetPinMux(g_pinDes->FUN_PWM.muxRegister,       //
                         g_pinDes->FUN_PWM.muxMode,           //
                         g_pinDes->FUN_PWM.inputRegister,     //
                         g_pinDes->FUN_PWM.inputDaisy,        //
                         g_pinDes->FUN_PWM.configRegister,    //
                         0U);                                 /* Software Input On Field: Input Path is determined by functionality */
        IOMUXC_SetPinConfig(g_pinDes->FUN_PWM.muxRegister,    //
                            g_pinDes->FUN_PWM.muxMode,        //
                            g_pinDes->FUN_PWM.inputRegister,  //
                            g_pinDes->FUN_PWM.inputDaisy,     //
                            g_pinDes->FUN_PWM.configRegister, //
                            0x10B0u);

        CLOCK_SetDiv(kCLOCK_AhbDiv, 0x2); /* Set AHB PODF to 2, divide by 3  200MHZ*/
        CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divede by 4  200MHZ/4 = 50MHZ */

        pwm_signal_param_t pwmSignal[1];
        pwm_config_t pwmConfig;

        PWM_GetDefaultConfig(&pwmConfig);

        pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
        pwmConfig.pairOperation = kPWM_Independent;
        pwmConfig.prescale = kPWM_Prescale_Divide_16; // 50MHZ/16 = 3.125MHZ
        PWM_Init(g_pinDes->PWM, g_pinDes->pwm_submodule, &pwmConfig);

        pwmSignal[0].pwmChannel = g_pinDes->pwm_channel;
        pwmSignal[0].level = kPWM_HighTrue;
        pwmSignal[0].dutyCyclePercent = 50;

        PWM_SetupPwm(g_pinDes->PWM, g_pinDes->pwm_submodule, pwmSignal, 2, kPWM_SignedCenterAligned, SERVO_PWM_FREQ_HZ, CLOCK_GetFreq(kCLOCK_IpgClk));

        PWM_SetPwmLdok(g_pinDes->PWM, (1U << g_pinDes->pwm_submodule), true);

        PWM_StartTimer(g_pinDes->PWM, (1U << g_pinDes->pwm_submodule));

        // turn off falut detection
        g_pinDes->PWM->SM[g_pinDes->pwm_submodule].DISMAP[0] = 0;
    }

    // 写角度
    write(value);

    this->isServoAttached = 1;

    return pin;
}

void Servo::detach()
{
    PinDescription const *g_pinDes = &g_APinDescription[this->servoPin];

    if (g_pinDes->PWM == NULL) return;

    PWM_StopTimer(g_pinDes->PWM, (1U << g_pinDes->pwm_submodule));

    PWM_Deinit(g_pinDes->PWM, g_pinDes->pwm_submodule);

    this->isServoAttached = 0;
}

void Servo::write(int value)
{
    constrain(value, 0, 180);

    value = map(value, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);

    writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
    // 输入value为脉宽us 500 - 2500
    constrain(value, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);

    // map到百分比
    // 0.5ms / 20ms = 2.5
    // 放大100倍 = 250 此时舵机 0度 对应value=250
    // 2.5ms / 20ms = 12.5
    // 放大100倍 = 1250 此时舵机 180度 对应value=1250
    value = map(value, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, 250, 1250);

    PinDescription const *g_pinDes = &g_APinDescription[this->servoPin];

    if (g_pinDes->PWM == NULL) return;

    uint16_t pulseCnt = 0, pwmHighPulse = 0;

    pulseCnt = g_pinDes->PWM->SM[g_pinDes->pwm_submodule].VAL1 * 2U;
    /* Calculate pulse width */
    pwmHighPulse = (pulseCnt * value) / 10000U;

    /* Setup the PWM dutycycle */
    if (g_pinDes->pwm_channel == kPWM_PwmA)
    {
        g_pinDes->PWM->SM[g_pinDes->pwm_submodule].VAL2 = (~(pwmHighPulse / 2U) + 1U);
        g_pinDes->PWM->SM[g_pinDes->pwm_submodule].VAL3 = (pwmHighPulse / 2U);
    }
    else
    {
        g_pinDes->PWM->SM[g_pinDes->pwm_submodule].VAL4 = (~(pwmHighPulse / 2U) + 1U);
        g_pinDes->PWM->SM[g_pinDes->pwm_submodule].VAL5 = (pwmHighPulse / 2U);
    }

    PWM_SetPwmLdok(g_pinDes->PWM, (1U << g_pinDes->pwm_submodule), true);
}

int Servo::read() // return the value as degrees
{
    return map(readMicroseconds() + 1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
    // unsigned int pulsewidth;
    // if (this->servoIndex != INVALID_SERVO)
    // {
    //     pulsewidth = servos[this->servoIndex].ticks;
    // }
    // else
    // {
    //     pulsewidth = 0;
    // }

    // return pulsewidth;
    // return (~(g_pinDes->PWM->SM[g_pinDes->pwm_submodule].VAL2 - 1)) * 2;
    return 0;
}

bool Servo::attached()
{
    return this->isServoAttached;
}