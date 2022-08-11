add_compile_flags(LD 
        -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16
        -Wall -fno-common -ffunction-sections 
        -fdata-sections -ffreestanding -fno-builtin 
        -mthumb -mapcs 
        -std=gnu99  -MMD -MP   
        --specs=nano.specs --specs=nosys.specs 
        -Xlinker --gc-sections -Xlinker -static -Xlinker -z -Xlinker muldefs -Xlinker -Map=output.map 
        -T ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/starup/MIMXRT1064_Arduino.ld 
        -static
        )

# C & C++ Flags Settings
add_compile_flags(BOTH
        -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 
        -Wall -fno-common -ffunction-sections 
        -fdata-sections -ffreestanding -fno-builtin 
        -mthumb -mapcs 
        -MMD  -MP
        -g -Og -w
        -nostdlib --param max-inline-insns-single=500
        -DNDEBUG
        -DCPU_${CPU} 
        -DXIP_EXTERNAL_FLASH=1 
        -DXIP_BOOT_HEADER_ENABLE=1 
        -DSERIAL_PORT_TYPE_UART=1
        -DUSB_STACK_BM
        -DARDUINO_MAIN=1
        -DSDK_OS_FREE_RTOS
        -DMCUXPRESSO_SDK
        -DCR_INTEGER_PRINTF 
        -DPRINTF_FLOAT_ENABLE=0 
        -D__USE_CMSIS 
        -DDEBUG 
        -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING 
        -DSD_ENABLED 
        -DDISABLE_WDOG 
        -D__STARTUP_INITIALIZE_NONCACHEDATA       
        -DARDUINO 
        -DCONNECT_AI_CAMERA
        -DCONNECT_EP
        )

add_compile_flags(C 
        -std=gnu99 
        )

add_compile_flags(CXX 
                -std=gnu++11 
                -fno-rtti 
                -fno-exceptions
                -fno-threadsafe-statics
                )

