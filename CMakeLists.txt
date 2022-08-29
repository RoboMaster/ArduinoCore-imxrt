# set this will supress some warnings
set(BUILDING_SDK "yes" CACHE INTERNAL "")

# basic config
if (NOT TOOLCHAIN)
    message(FATAL_ERROR "TOOLCHAIN  not set, use ${TOOLCHAIN} as TOOLCHAIN. Also, you can set it manually. e.g. -DTOOLCHAIN=/opt/gcc-arm-none-eabi-8.2/bin ")
endif()

if (NOT PROJ)
    execute_process(COMMAND mkdir -p ../examples/Blink)
    message(FATAL_ERROR "PROJ not set, use ${PROJ} as PROJ. Also, you can set it manually. e.g. -DPROJ=Blink")
endif ()

if (NOT BOARD)
    message(FATAL_ERROR "BOARD not set, use ${BOARD} as BOARD. Also, you can set it manually. e.g. -DBOARD=arch_mix")
endif ()

cmake_minimum_required(VERSION 3.0)
# macros.cmake arm-none-toolchain.cmake compile-flags.cmake fix-9985.cmake
# 定义了宏 工具链路径 编译标志
include(${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/common.cmake)

project(${PROJ} C CXX ASM)


# 引入了 header_directories 这个宏
include(${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/macros.internal.cmake)
# 添加头文件路径 
header_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/arduino)
header_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries/SPI)
header_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries/Wire)
header_directories(${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD})
header_directories(${CMAKE_CURRENT_SOURCE_DIR}/examples/${PROJ})

# build library first
# 添加这些文件到编译列表中
add_source_files(${CMAKE_CURRENT_SOURCE_DIR}/libraries/SPI/*.cpp)
add_source_files(${CMAKE_CURRENT_SOURCE_DIR}/libraries/Wire/*.cpp)

FILE(GLOB_RECURSE PROJ_LIB_SRC_INO
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/${PROJ}/*.ino"
    )
SET_PROPERTY(SOURCE ${PROJ_LIB_SRC_INO} PROPERTY LANGUAGE CXX)
SET_SOURCE_FILES_PROPERTIES(${PROJ_LIB_SRC_INO} PROPERTIES COMPILE_FLAGS "-x c++")

# compile project
# 添加这些文件到编译列表中
add_source_files(examples/${PROJ}/*.c examples/${PROJ}/*.s examples/${PROJ}/*.S examples/${PROJ}/*.cpp)

FILE(GLOB_RECURSE ASSEMBLY_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/*.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/*.S"
    )

# 添加这些文件到编译列表中
add_source_files(${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/variant.c
                 ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/board/*.c
                )

            
SET_PROPERTY(SOURCE ${ASSEMBLY_FILES} PROPERTY LANGUAGE C)
SET_SOURCE_FILES_PROPERTIES(${ASSEMBLY_FILES} PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")

# 编译SDK生成静态链接库
include(${CMAKE_CURRENT_SOURCE_DIR}/sdk.cmake)

# 编译文件
include(${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/executable.cmake)

# 往source文件中追加源文件
target_sources(${PROJECT_NAME} PUBLIC 
            ${PROJ_LIB_SRC_INO}
            ${ASSEMBLY_FILES} 
)

# Copy lib files
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMENT "Copy lib files to /tools/lib/${SDK} ..."
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsdk_components.a ${CMAKE_CURRENT_SOURCE_DIR}/tools/lib/${SDK}/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsdk_devices.a ${CMAKE_CURRENT_SOURCE_DIR}/tools/lib/${SDK}/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsdk_middleware.a ${CMAKE_CURRENT_SOURCE_DIR}/tools/lib/${SDK}/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libsdk_rtos.a ${CMAKE_CURRENT_SOURCE_DIR}/tools/lib/${SDK}/
)
