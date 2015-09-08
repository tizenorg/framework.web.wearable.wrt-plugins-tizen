SET(TARGET_NAME ${filesystem_target})
SET(DESTINATION_NAME ${filesystem_dest})
SET(TARGET_IMPL_NAME ${filesystem_impl})
SET(TARGET_CONFIG_NAME ${filesystem_config})

PKG_CHECK_MODULES(platform_pkgs_filesystem REQUIRED capi-appfw-application storage libpcrecpp ecore capi-system-info)

ADD_DEFINITIONS("-fvisibility=hidden")

INCLUDE_DIRECTORIES(
    ${INCLUDE_COMMON}
    ${platform_pkgs_filesystem_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${tizen_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    EventResolve.cpp
    EventGetStorage.cpp
    EventListStorages.cpp
    EventCopy.cpp
    EventMove.cpp
    EventListNodes.cpp
    EventOpen.cpp
    NodeFilter.cpp
    StorageProperties.cpp
    EventStorageStateChanged.cpp
    Manager.cpp
    Node.cpp
    Path.cpp
    Stream.cpp
    NodeFilterMatcher.cpp
    JSFilesystemManager.cpp
    JSFile.cpp
    File.cpp
    JSFilestream.cpp
    JSStorage.cpp
    JSErrors.cpp
    Converter.cpp
    Encodings.cpp
    FilesystemUtils.cpp
    FilesystemPathUtils.cpp
    FilesystemExternalUtils.cpp
    FilesystemErrorUtil.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
    ${LIBS_WIDGETDB}
    ${tizen_impl}
    ${platform_pkgs_filesystem_LIBRARIES}
)

SET(SRCS_CONFIG
    plugin_config.cpp
)

ADD_LIBRARY(${TARGET_CONFIG_NAME} SHARED ${SRCS_CONFIG})

TARGET_LINK_LIBRARIES(${TARGET_CONFIG_NAME}
    ${LIBS_COMMON}
)

SET(SRCS
    plugin_initializer.cpp
)

ADD_LIBRARY(${TARGET_NAME} SHARED ${SRCS})

TARGET_LINK_LIBRARIES(${TARGET_NAME}
    ${TARGET_IMPL_NAME}
    ${TARGET_CONFIG_NAME}
)

INSTALL(TARGETS ${TARGET_NAME} ${TARGET_CONFIG_NAME} ${TARGET_IMPL_NAME} LIBRARY DESTINATION ${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME})
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.xml DESTINATION ${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME})
INSTALL(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/filesystem
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
