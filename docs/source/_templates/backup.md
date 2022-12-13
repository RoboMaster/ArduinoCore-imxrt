

## Arduino函数

### 1 入口函数

#### setup()

Arduino控制器通电或复位后，即会开始执行setup() 函数中的程序，该部分只会执行一次。通常我们会在setup() 函数中完成Arduino的初始化设置，如配置I/O口状态，初始化串口等操作。

#### loop()

在setup() 函数中的程序执行完后，Arduino会接着执行loop() 函数中的程序。而loop()函数是一个死循环，其中的程序会不断的重复运行。通常我们会在loop() 函数中完成程序的主要功能，如驱动各种模块，采集数据等。

### 2 数字输入/输出

- [x] pinMode(pin, mode) 

  在使用输入或输出功能前，你需要先通过pinMode() 函数配置引脚的模式为输入模式或输出模式。

- [x] digitalWrite(pin, value)

  配置成输出模式后，你还需要使用digitalWrite() 让其输出高电平或者是低电平。

- [x] digitalRead(pin)

  读取指定的电平值

### 3 模拟输入/输出

- [x] analogRead(pin) 

  模拟输入引脚是带有ADC（Analog-to-Digital Converter，模数转换器）功能的引脚。
  它可以将外部输入的模拟信号转换为芯片运算时可以识别的数字信号，从而实现读入模拟值的功能。
  模拟输入功能需要使用analogRead() 函数。

- [x] analogWrite(pin,value)

  使用analogWrite() 函数实现PWM输出功能。
  在analogWrite() 和analogRead() 函数内部，已经完成了引脚的初始化，因此不用在Setup() 函数中进行初始化操作。

### 4 高级输入、输出

- [ ] tone()

  tone() 主要用于Arduino连接蜂鸣器或扬声器发声。可以让指定引脚产生一个占空比为50%的指定频率的方波。

- [ ] pulseIn()

  检测指定引脚上的脉冲信号宽度。
  例如当要检测高电平脉冲时，pulseIn() 会等待指定引脚输入的电平变高，当变高后开始记时，直到输入电平变低，停止计时。
  pulseln() 函数会返回这个脉冲信号持续的时间，即这个脉冲的宽度。
  函数还可以设定超时时间。如果超过设定时间，仍未检测到脉冲，则会退出pulseIn()函数并返回0。
  当没有设定超时时间时，pulseIn() 会默认1秒钟的超时时间。

- [ ] attachInterrupt(pin, ISR, mode)

  对中断引脚进行初始化配置，以开启Arduino的外部中断功能

- [ ] detachInterrupt(pin)

  如果你不需要使用外部中断了，你可以用中断分离函数detachInterrupt() 来关闭中断功能。

### 5 时间控制

- [x] millis()

  获取Arduino通电后（或复位后）到现在的时间,单位毫秒ms

- [x] micros()

  获取Arduino通电后（或复位后）到现在的时间，单位微秒us

- [x] delay( ms)

  毫秒级延时

- [x] micros()

  微秒级延时

### 6 串口通信函数

- [x] Serial.begin(speed)
- [x] Serial.print(val)
- [x] Serial.println(val)
- [x] Serial.read()

### 7 I2C通信

### 8 spi通信



## 其他接口

### HardwareSerial



### HardwareTimer



## 构建在核心中的库

### SPI



### I2C



### Servo



### Stepper



### SoftwareSerial



## 其他



