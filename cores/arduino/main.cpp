#ifdef ARDUINO_MAIN

#include "Arduino.h"
#include "virtual_com.h"

// __attribute__((constructor))修饰，该函数在mian之前被调用
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
  // init();

  setup();
  for (;;)
  {
    // extern "C"
    // {
    //     extern void APPTask();
    //     APPTask();
    // }
    APPTask();
    loop();
  }
}

#endif
