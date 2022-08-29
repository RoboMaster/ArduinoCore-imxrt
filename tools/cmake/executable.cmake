
# 去掉重复子串
removeDuplicateSubstring(${CMAKE_C_FLAGS} CMAKE_C_FLAGS)
removeDuplicateSubstring(${CMAKE_CXX_FLAGS} CMAKE_CXX_FLAGS)

# 生成名为${PROJECT_NAME}的可执行文件， ${SOURCE_FILES}是源文件列表
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# 将工程${PROJECT_NAME}的LINKER_LANGUAGE属性设置为c
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

# 为 target 添加需要链接的共享库
target_link_libraries(${PROJECT_NAME}
            -Wl,--start-group
            gcc m c nosys
            -Wl,--whole-archive
            sdk_components
            sdk_devices
            sdk_middleware
            sdk_rtos
            rm_func
            arduinocore
            -Wl,--no-whole-archive
            -Wl,--end-group
        )

SET (SUFFIX ".elf")

IF(SUFFIX)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SUFFIX ${SUFFIX})
ENDIF()

# Build target
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}${SUFFIX} ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
        DEPENDS ${PROJECT_NAME}
        COMMAND ${CMAKE_SIZE} -A -x ${CMAKE_BINARY_DIR}/${PROJECT_NAME}${SUFFIX}
        COMMENT "Generating .bin file ..."
        )

# show information
include(${CMAKE_CURRENT_LIST_DIR}/dump-config.cmake)