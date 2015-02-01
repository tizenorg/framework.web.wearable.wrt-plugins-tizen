SET(TARGET_NAME ${humanactivitymonitor_target})
SET(DESTINATION_NAME ${humanactivitymonitor_dest})
SET(TARGET_IMPL_NAME ${humanactivitymonitor_impl})

pkg_check_modules(platform_pkgs_humanactivitymonitor REQUIRED
    glib-2.0
    capi-system-info
    libcore-context-manager
    sensor
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_COMMON}
    ${TOP}/HumanActivityMonitor
    ${platform_pkgs_humanactivitymonitor_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${tizen_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    JSMotionManager.cpp
    JSPedometerInfo.cpp
    MotionManager.cpp
    PedometerInfo.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
    ${tizen_impl}
    ${platform_pkgs_humanactivitymonitor_LIBRARIES}
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
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/humanactivitymonitor
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
