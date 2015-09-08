SET(TARGET_NAME ${timeutil_target})
SET(DESTINATION_NAME ${timeutil_dest})
SET(TARGET_IMPL_NAME ${timeutil_impl})

PKG_CHECK_MODULES(platform_pkgs_time REQUIRED icu-i18n icu-io icu-le icu-lx icu-uc )

#This module should be public by default since it is being used directly
#by other modules for example Calendar.
#Therefore ADD_DEFINITIONS("-fvisibility=hidden") is not present here.
#

INCLUDE_DIRECTORIES(
    ${INCLUDE_COMMON}
    ${platform_pkgs_time_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    JSTimeUtil.cpp
    JSTZDate.cpp
    JSTimeDuration.cpp
    TimeDuration.cpp
    TimeUtil.cpp
    TZDate.cpp
    TimeUtilTools.cpp
    TimeUtilConverter.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
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
INSTALL(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/timeutil
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
