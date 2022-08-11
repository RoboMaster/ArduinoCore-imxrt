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

# ---------------------------------------------------------------
#                   生成静态链接库 u8g2
# ---------------------------------------------------------------
# 添加库文件 将其列表生成输出到 LIB_FILES 中
add_lib_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/devel/U8g2/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/devel/U8g2/src/*.cpp
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