# 4. 高级功能

## 4.1 FreeRTOS操作系统

![img](arduino-4.assets/FreeRTOS-logo-removebg-preview.png)

freeRTOS是一款开源免费的嵌入式实时操作系统。

RM扩展模块开发板底层使用FreeRTOS构建，因此在用户程序中，您可以调用任意freeRTOS接口来创建自己的任务。

### 示例程序

这是一个操作系统使用的示例，您可以从`示例`->`RM_Examples`->`HelloFreeRTOS`直接打开。

```c++
#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

void TaskBlink(void *pvParameters);
void TaskHello(void *pvParameters);

void setup()
{
    SerialUSB.begin();
    xTaskCreate(TaskBlink, "Blink", 128, NULL, 2, NULL);
    xTaskCreate(TaskHello, "Hello", 128, NULL, 1, NULL);
}

void loop()
{
}

void TaskBlink(void *pvParameters)
{
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        vTaskDelay(500);
        digitalWrite(LED_BUILTIN, LOW);
        vTaskDelay(500);
    }
}

void TaskHello(void *pvParameters)
{
    for (;;)
    {
        SerialUSB.println("Hello RoboMaster !!!");
        vTaskDelay(1000);
    }
}
```

### 代码分析

第一部分，创建和申明两个任务函数，使用`Task+XXX`来命名利于区分任务函数与普通函数，将函数的参数定义为`void *pvParameters`（void 类型的指针）。

每个任务函数可以被看做一个独立的线程，在进入之后按顺序执行，遇到for(;;)函数后将不断的循环。

以下代码定义了两个任务：1.控制LED灯闪烁  2.读取串口输出字符串。

```c++
void TaskBlink( void *pvParameters );
void TaskAnalogRead( void *pvParameters );

void TaskBlink(void *pvParameters)
{
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        vTaskDelay(500);
        digitalWrite(LED_BUILTIN, LOW);
        vTaskDelay(500);
    }
}

void TaskHello(void *pvParameters)
{
    for (;;)
    {
        SerialUSB.println("Hello RoboMaster !!!");
        vTaskDelay(1000);
    }
}
```

第二部分，初始化调试串口，调用`xTaskCreate`函数创建任务，给定任务函数、任务名、栈大小、优先级。

```c++
void setup()
{
    SerialUSB.begin();
    xTaskCreate(TaskBlink, "Blink", 128, NULL, 2, NULL);
    xTaskCreate(TaskHello, "Hello", 128, NULL, 1, NULL);
}
```

细心的同学们现在已经发现Loop函数中我们没有写任何东西，因为此时我们的逻辑在其他的线程中执行。

```c++
void loop()
{
}
```

连接USB，打开串口，此时会以1s一次的频率输出"Hello RoboMaster !!!"文本。

LED指示灯会以1S每次的频率闪烁。

## 4.2 Cmake支持

Arduino核心基于C++，本项目也可以使用Cmake编译。

首先确保您了解Cmake工程体系，以及安装Cmake、GCC工具链。

建立工程：

```shell
git clone https://github.com/RoboMaster/ArduinoCore-imxrt
cd ArduinoCore-imxrt
mkdir -p devel/Blink
#或手动创建
touch  devel/Blink/Blink.ino
```

生成Makefile：

```shell
mkdir build 
cd build
#use windows and mingw
#请将工具链地址更换为正确的路径
cmake .. -DPROJ=Blink -DBOARD=RMEM_01 -DCPU=MIMXRT1064DVL6A -DTOOLCHAIN=E:/GNUTools/gcc-arm-none-eabi/bin -G "MinGW Makefiles"
```

编译：

```shell
#mingw
mingw32-make -j16
```

