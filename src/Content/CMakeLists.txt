
SET(TARGET_NAME ${content_target})
SET(DESTINATION_NAME ${content_dest})
SET(TARGET_IMPL_NAME ${content_impl})
SET(TARGET_CONFIG_NAME ${content_config})

PKG_CHECK_MODULES(platform_pkgs_content REQUIRED capi-content-media-content capi-media-metadata-extractor)

ADD_DEFINITIONS("-fvisibility=hidden")

SET(INCLUDE_DIRS
    ${filesystem_include}
    ${tizen_include}
    ${platform_pkgs_content_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES(${INCLUDE_COMMON} ${INCLUDE_DIRS})
MESSAGE(STATUS "${TARGET_NAME} module includes: INCLUDE_COMMON ${INCLUDE_DIRS}")

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${tizen_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${filesystem_dest}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    JSContentManager.cpp
    JSContentDirectory.cpp
    JSContent.cpp
    JSVideoContent.cpp
    JSAudioContentLyrics.cpp
    JSAudioContent.cpp
    JSImageContent.cpp
    ContentManager.cpp
    ContentChangeCallback.cpp
    ContentDirectory.cpp
    Content.cpp
    VideoContent.cpp
    AudioContentLyrics.cpp
    AudioContent.cpp
    ImageContent.cpp
    ContentHelper.cpp
    ContentUtility.cpp
    AudioLyricsUtil.cpp
    ContentFilterQueryGenerator.cpp
    ContentCallback.cpp

    AddBatchCallbackUserData.cpp
    BaseCallbackUserData.cpp
    CreatePlaylistCallbackUserData.cpp
    GetPlaylistItemsRequest.cpp
    GetPlaylistsRequest.cpp
    JSPlaylist.cpp
    JSPlaylistItem.cpp
    MoveItemCallbackUserData.cpp
    PlaylistArraySuccessCallbackUserData.cpp
    PlaylistContentCallback.cpp
    PlaylistCopyUtil.cpp
    Playlist.cpp
    PlaylistItem.cpp
    PlaylistUtils.cpp
    RemoveBatchCallbackUserData.cpp
    RemovePlaylistCallbackUserData.cpp
    SetItemsOrderCallbackUserData.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})


SET(LINK_LIBS
    ${tizen_impl}
    ${filesystem_impl}
    ${filesystem_config}
    ${platform_pkgs_content_LIBRARIES}
    ${TARGET_CONFIG_NAME}
)

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME} ${LIBS_COMMON} ${LINK_LIBS})
MESSAGE(STATUS "${TARGET_NAME} module links: LIBS_COMMON ${LINK_LIBS}")


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
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/content
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
