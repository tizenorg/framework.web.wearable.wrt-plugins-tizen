
SET(TARGET_NAME ${notification_target})
SET(DESTINATION_NAME ${notification_dest})
SET(TARGET_IMPL_NAME ${notification_impl})

PKG_CHECK_MODULES(platform_pkgs_notification REQUIRED notification capi-appfw-application)

INCLUDE_DIRECTORIES(
    ${INCLUDE_COMMON}
    ${TOP}/Application
    ${TOP}/Filesystem
    ${platform_pkgs_notification_INCLUDE_DIRS}
    ${notification_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${tizen_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${application_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${filesystem_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    JSNotificationManager.cpp
    JSStatusNotification.cpp
    JSNotificationDetailInfo.cpp
    NotificationManager.cpp
    StatusNotification.cpp
    NotificationDetailInfo.cpp
    NotificationUtil.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
    ${notification_LIBRARIES}
    ${application_impl}
    ${filesystem_impl}
    ${filesystem_config}
    ${platform_pkgs_notification_LIBRARIES}
)

SET(SRCS
    plugin_config.cpp
    plugin_initializer.cpp
)

ADD_LIBRARY(${TARGET_NAME} SHARED ${SRCS})

TARGET_LINK_LIBRARIES(${TARGET_NAME}
    ${TARGET_IMPL_NAME}
        ${tizen_impl}
    ${timeutil_impl}
)

INSTALL(TARGETS ${TARGET_NAME} ${TARGET_IMPL_NAME} LIBRARY DESTINATION ${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME})
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.xml DESTINATION ${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME})
INSTALL(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/notification
        FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
