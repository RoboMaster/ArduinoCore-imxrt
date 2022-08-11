#ifdef ARDUINO_MAIN

#include "Arduino.h"

// __attribute__((constructor))修饰符
// 该函数会在mian之前被调用
__attribute__((constructor(101))) void premain()
{
  init();
}

void __attribute__((weak)) setup()
{
}

void __attribute__((weak)) loop()
{
}

int __attribute__((weak)) main()
{

  setup();

  for (;;)
  {
    loop();
  }
}

#endif
