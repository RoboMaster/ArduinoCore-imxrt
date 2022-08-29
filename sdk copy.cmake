IF(${CPU} MATCHES "MIMXRT1052.*")
    set(SDK "MIMXRT1052")
ELSEIF(${CPU} MATCHES "MIMXRT1062.*")
    MESSAGE(${CPU})
    set(SDK "MIMXRT1062")
ELSEIF(${CPU} MATCHES "MIMXRT1064.*")
    MESSAGE(${CPU})
    set(SDK "MIMXRT1064")
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported CPU!")
ENDIF()

MESSAGE(${SDK})

# ---------------------------------------------------------------
#                   生成静态链接库 fsl_bsp
# ---------------------------------------------------------------
# 添加库文件 将其列表生成输出到 LIB_FILES 中
add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers/*.c)
# 将 fsl_bsp_drivers_files 宏定义为 LIB_FILES 的值
set(fsl_bsp_drivers_files ${LIB_FILES})
# 清空 LIB_FILES
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(fsl_bsp STATIC  ${fsl_bsp_drivers_files}
                            ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/system_${SDK}.c
                            )
target_include_directories(fsl_bsp PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include 
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/xip
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/str
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists
                                        )

# ---------------------------------------------------------------
#                   生成静态链接库 fsl_xip_drivers
# ---------------------------------------------------------------
# add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/xip/*.c)
# add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${CPU}/xip/*.c)
add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/xip/*.c)
set(fsl_xip_drivers_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
# 生成静态链接库文件
add_library(fsl_xip_drivers STATIC  ${fsl_xip_drivers_files})
# 指定目标 fsl_xip_drivers 所需要包含的头文件
target_include_directories(fsl_xip_drivers PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino
                    )

# ---------------------------------------------------------------
#                   生成静态链接库 fsl_usb_drivers
# ---------------------------------------------------------------
add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/cores/USB/*.c
              ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/*.c
              ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console/*.c
              ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/str/*.c
              ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists/*.c
              ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa/*.c
              ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager/fsl_component_serial_manager.c
              ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager/fsl_component_serial_port_uart.c
              ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/silicon_id/*.c
              ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart/*.c
)
set(fsl_usb_drivers_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(fsl_usb_drivers STATIC  ${fsl_usb_drivers_files})
target_include_directories(fsl_usb_drivers PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/cdc
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/video
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source/ehci
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/phy
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/str
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/silicon_id
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/USB
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/GCC/ARM_CM4F
                    )

# ---------------------------------------------------------------
#                   生成静态链接库 rtos
# ---------------------------------------------------------------     
add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/*.c)
set(fsl_rtos_drivers_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
# 生成静态链接库文件
add_library(fsl_rtos_drivers STATIC  ${fsl_rtos_drivers_files})
# 指定目标 fsl_xip_drivers 所需要包含的头文件
target_include_directories(fsl_rtos_drivers PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/GCC/ARM_CM4F
                    )

# ---------------------------------------------------------------
#                   生成静态链接库 vender source
# ---------------------------------------------------------------     
add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/*.c)
set(fsl_vendor_drivers_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
# 生成静态链接库文件
add_library(fsl_vendor_drivers STATIC  ${fsl_vendor_drivers_files})
# 指定目标 fsl_xip_drivers 所需要包含的头文件
target_include_directories(fsl_vendor_drivers PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/xip
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/str
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/GCC/ARM_CM4F
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/cdc
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/video
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source/ehci
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/phy
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists
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
add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/cores/*.c)
add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/cores/*.cpp)
set(arduinocore_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(arduinocore STATIC  ${arduinocore_files})
target_include_directories(arduinocore PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/cdc
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class/video
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/class
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device/source/ehci
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/phy
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/USB
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
                    )
