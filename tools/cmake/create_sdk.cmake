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

MESSAGE("CREATE SDK NAME: " ${SDK})

# ---------------------------------------------------------------
#                   sdk_components
# ---------------------------------------------------------------
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/osa/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/silicon_id/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart/*.c
)
set(sdk_components_files ${LIB_FILES})
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
#                   sdk_devices
# ---------------------------------------------------------------
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/drivers/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/debug_console/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/utilities/str/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/xip/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/${SDK}/system_${SDK}.c
)
set(sdk_devices_files ${LIB_FILES})
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
#                   sdk_middleware
# ---------------------------------------------------------------
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/*.c
)
set(sdk_middleware_files ${LIB_FILES})
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
#                   sdk_rtos
# ---------------------------------------------------------------
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/*.c
)
set(sdk_rtos_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(sdk_rtos STATIC ${sdk_rtos_files})
target_include_directories(sdk_rtos PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/rtos/freertos/portable/GCC/ARM_CM4F
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/imxrt
)

# ---------------------------------------------------------------
#                   rm_func
# ---------------------------------------------------------------     
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/source/*.c
)
set(rm_func_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(rm_func STATIC ${rm_func_files})
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
#                   arduinocore
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

