SET(TARGET_NAME ${systemsetting_target})
SET(DESTINATION_NAME ${systemsetting_dest})
SET(TARGET_IMPL_NAME ${systemsetting_impl})

PKG_CHECK_MODULES(platform_pkgs_systemsetting REQUIRED capi-system-system-settings)

INCLUDE_DIRECTORIES(
    ${INCLUDE_COMMON}
    ${TOP}/Tizen
    ${TOP}/Filesystem
    ${platform_pkgs_systemsetting_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${tizen_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${filesystem_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    SystemSetting.cpp
    ISystemSetting.cpp
    SystemSettingFactory.cpp
    JSSystemSetting.cpp
    SystemSettingResponseDispatcher.cpp
    ConverterSystemSetting.cpp
    SystemSettingAsyncCallbackManager.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
    ${tizen_impl}
    ${filesystem_impl}
    ${platform_pkgs_systemsetting_LIBRARIES}
)

SET(SRCS
    plugin_config.cpp
    plugin_initializer.cpp
)

ADD_LIBRARY(${TARGET_NAME} SHARED ${SRCS})

TARGET_LINK_LIBRARIES(${TARGET_NAME}
    ${TARGET_IMPL_NAME}
)

INSTALL(TARGETS ${TARGET_NAME} ${TARGET_IMPL_NAME} LIBRARY DESTINATION ${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME})
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.xml DESTINATION ${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME})
INSTALL(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/systemsetting
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
