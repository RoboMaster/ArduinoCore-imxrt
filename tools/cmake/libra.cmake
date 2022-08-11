IF(${CPU} MATCHES "MIMXRT1052.*")
    set(SDK "MIMXRT1052")
ELSEIF(${CPU} MATCHES "MIMXRT1062.*")
    MESSAGE(${CPU})
    set(SDK "MIMXRT1062")
ELSEIF(${CPU} MATCHES "MIMXRT1064.*")
    MESSAGE("CPU NAME: " ${CPU})
    set(SDK "MIMXRT1064")
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported CPU!")
ENDIF()

MESSAGE("SDK NAME: " ${SDK})

# ---------------------------------------------------------------
#                   生成静态链接库 u8g2
# ---------------------------------------------------------------
# 添加库文件 将其列表生成输出到 LIB_FILES 中
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/U8g2/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/U8g2/src/*.cpp
)
set(u8g2_files ${LIB_FILES})
# 清空 LIB_FILES
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(u8g2 STATIC ${u8g2_files})
target_include_directories(u8g2 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/silicon_id
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart

    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/imxrt
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino    
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/U8g2/src/clib
    ${CMAKE_CURRENT_SOURCE_DIR}/libraries/U8g2/src
)

# ---------------------------------------------------------------
#                   生成静态链接库 sdk_components
# ---------------------------------------------------------------
# 添加库文件 将其列表生成输出到 LIB_FILES 中
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/silicon_id/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart/*.c
)
# 将 sdk_components_files 宏定义为 LIB_FILES 的值
set(sdk_components_files ${LIB_FILES})
# 清空 LIB_FILES
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(sdk_components STATIC ${sdk_components_files})
target_include_directories(sdk_components PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/imxrt
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/silicon_id
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart
)

# ---------------------------------------------------------------
#                   生成静态链接库 sdk_devices
# ---------------------------------------------------------------
# 添加库文件 将其列表生成输出到 LIB_FILES 中
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/str/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/xip/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/system_${SDK}.c
)
# 将 sdk_devices_files 宏定义为 LIB_FILES 的值
set(sdk_devices_files ${LIB_FILES})
# 清空 LIB_FILES
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(sdk_devices STATIC ${sdk_devices_files})
target_include_directories(sdk_devices PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/imxrt
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/silicon_id
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/str
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/xip
)

# ---------------------------------------------------------------
#                   生成静态链接库 sdk_middleware
# ---------------------------------------------------------------
# 添加库文件 将其列表生成输出到 LIB_FILES 中
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/*.c
)
# 将 sdk_middleware_files 宏定义为 LIB_FILES 的值
set(sdk_middleware_files ${LIB_FILES})
# 清空 LIB_FILES
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(sdk_middleware STATIC ${sdk_middleware_files})
target_include_directories(sdk_middleware PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/imxrt
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/silicon_id
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/str
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/xip
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/cdc
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/video
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source/ehci
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/phy
)

# ---------------------------------------------------------------
#                   生成静态链接库 sdk_rtos
# ---------------------------------------------------------------
# 添加库文件 将其列表生成输出到 LIB_FILES 中
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/*.c
)
# 将 sdk_rtos_files 宏定义为 LIB_FILES 的值
set(sdk_rtos_files ${LIB_FILES})
# 清空 LIB_FILES
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(sdk_rtos STATIC ${sdk_rtos_files})
target_include_directories(sdk_rtos PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/imxrt
)

# ---------------------------------------------------------------
#                   生成静态链接库 rm_func
# ---------------------------------------------------------------     
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/*.c
)
set(rm_func_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
# 生成静态链接库文件
add_library(rm_func STATIC ${rm_func_files})
# 指定目标 fsl_xip_drivers 所需要包含的头文件
target_include_directories(rm_func PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/imxrt
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/silicon_id
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/str
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/xip
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/cdc
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/video
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source/ehci
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/phy
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/user_app
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/user_driver
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/user_libs
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/user_libs/crc
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/user_libs/ext_protocol
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/user_libs/log
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/user_libs/open_protocol
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/user_libs/open_protocol/crc
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/user_middleware/cmsis_os
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/user_usb
)

# ---------------------------------------------------------------
#                   生成静态链接库 arduinocore
# ---------------------------------------------------------------                         
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/*.c
)
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/*.cpp
)
set(arduinocore_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(arduinocore STATIC  ${arduinocore_files})
target_include_directories(arduinocore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/imxrt
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/cdc
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/video
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source/ehci
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/phy
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/silicon_id
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/portable/GCC/ARM_CM4F
)

