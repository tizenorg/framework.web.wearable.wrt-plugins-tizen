SET(TARGET_NAME ${messageport_target})
SET(DESTINATION_NAME ${messageport_dest})
SET(TARGET_IMPL_NAME ${messageport_impl})

IF(ENABLE_OPTIONAL_MESSAGE_PORT)
PKG_CHECK_MODULES(platform_pkgs_messageport REQUIRED message-port)

INCLUDE_DIRECTORIES(
    ${INCLUDE_COMMON}
    ${platform_pkgs_messageport_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    ILocalMessagePort.cpp
    IMessagePortManager.cpp
    IRemoteMessagePort.cpp
    MessagePortFactory.cpp
    LocalMessagePort.cpp
    MessagePortManager.cpp
    MessagePortManagerProxy.cpp
    MessagePortUtility.cpp
    RemoteMessagePort.cpp
    JSLocalMessagePort.cpp
    JSMessagePortManager.cpp
    JSRemoteMessagePort.cpp
    LocalMessagePortController.cpp
    MessagePortJSUtil.cpp
    MessagePortListenerManager.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
    ${platform_pkgs_messageport_LIBRARIES}
)

SET(SRCS
    plugin_initializer.cpp
)

ADD_LIBRARY(${TARGET_NAME} SHARED ${SRCS})

TARGET_LINK_LIBRARIES(${TARGET_NAME}
    ${TARGET_IMPL_NAME}
)

INSTALL(TARGETS ${TARGET_NAME} ${TARGET_IMPL_NAME} LIBRARY DESTINATION ${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME})
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.xml DESTINATION ${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME})
ENDIF(ENABLE_OPTIONAL_MESSAGE_PORT)
INSTALL(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/messageport
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
