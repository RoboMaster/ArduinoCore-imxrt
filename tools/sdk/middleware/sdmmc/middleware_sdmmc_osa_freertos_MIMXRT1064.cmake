include_guard()
message("middleware_sdmmc_osa_freertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/osa/fsl_sdmmc_osa.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/osa
)


include(component_osa_free_rtos_MIMXRT1064)

