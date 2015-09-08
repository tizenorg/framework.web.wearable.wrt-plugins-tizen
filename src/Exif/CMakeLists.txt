SET(TARGET_NAME ${exif_target})
SET(DESTINATION_NAME ${exif_dest})
SET(TARGET_IMPL_NAME ${exif_impl})

PKG_CHECK_MODULES(platform_pkgs_exif REQUIRED libexif)

ADD_DEFINITIONS("-fvisibility=hidden")

SET(INCLUDE_DIRS
    ${filesystem_include}
    ${timeutil_include}
    ${tizen_include}
    ${platform_pkgs_exif_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES(${INCLUDE_COMMON} ${INCLUDE_DIRS})
MESSAGE(STATUS "${TARGET_NAME} module includes: INCLUDE_COMMON ${INCLUDE_DIRS}")

SET(CMAKE_INSTALL_RPATH

    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${filesystem_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${timeutil_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${tizen_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    JSExifManager.cpp
    ExifManager.cpp
    JSExifInformation.cpp
    ExifInformation.cpp
    ExifManagerCallbacks.cpp
    ExifUtil.cpp
    ExifTagSaver.cpp
    ExifGPSLocation.cpp
    ExifGPSTime.cpp
    Rational.cpp
    JpegFile.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

SET(LINK_LIBS
    ${tizen_impl}
    ${timeutil_impl}
    ${filesystem_impl}
    ${filesystem_config}
    ${platform_pkgs_exif_LIBRARIES}
)

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME} ${LIBS_COMMON} ${LINK_LIBS})
MESSAGE(STATUS "${TARGET_NAME} module links: LIBS_COMMON ${LINK_LIBS}")

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
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/exif
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
