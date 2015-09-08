SET(TARGET_NAME ${alarm_target})
SET(DESTINATION_NAME ${alarm_dest})
SET(TARGET_IMPL_NAME ${alarm_impl})

PKG_CHECK_MODULES(platform_pkgs_alarm REQUIRED
    capi-appfw-application
    capi-appfw-alarm
)

ADD_DEFINITIONS("-fvisibility=hidden")

INCLUDE_DIRECTORIES(
    ${platform_pkgs_alarm_INCLUDE_DIRS}
    ${INCLUDE_COMMON}
    ${TOP}/Application
    ${TOP}/TimeUtil
)

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${tizen_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${timeutil_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${application_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    AlarmUtil.cpp
    AlarmAbsolute.cpp
    AlarmConverter.cpp
    AlarmRelative.cpp
    JSAlarmAbsolute.cpp
    JSAlarmManager.cpp
    JSAlarmRelative.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
    ${platform_pkgs_alarm_LIBRARIES}
    ${tizen_impl}
    ${application_impl}
    ${timeutil_impl}
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
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/alarm
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
