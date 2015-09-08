SET(TARGET_NAME ${tizen_target})
SET(DESTINATION_NAME ${tizen_dest})
SET(TARGET_IMPL_NAME ${tizen_impl})
SET(TARGET_CONFIG_NAME ${tizen_config})

PKG_CHECK_MODULES(platform_pkgs_tizen REQUIRED wrt-plugins-plugin-manager)

INCLUDE_DIRECTORIES(
    ${TOP}/Common
    ${platform_pkgs_tizen_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    JSTizen.cpp
    PluginPriv.cpp
    Any.cpp
    AbstractFilter.cpp
    JSAttributeFilter.cpp
    AttributeFilter.cpp
    JSAttributeRangeFilter.cpp
    AttributeRangeFilter.cpp
    JSCompositeFilter.cpp
    CompositeFilter.cpp
    JSSortMode.cpp
    SortMode.cpp
    JSSimpleCoordinates.cpp
    SimpleCoordinates.cpp
    FilterIterator.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
    ${LIBS_WIDGETDB}
    ${platform_pkgs_tizen_LIBRARIES}
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
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/tizen
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
