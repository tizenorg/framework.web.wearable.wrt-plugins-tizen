SET(TARGET_NAME ${nfc_target})
SET(DESTINATION_NAME ${nfc_dest})
SET(TARGET_IMPL_NAME ${nfc_impl})

IF(ENABLE_OPTIONAL_NFC)
PKG_CHECK_MODULES(platform_pkgs_nfc REQUIRED capi-appfw-application capi-network-nfc capi-system-info)

ADD_DEFINITIONS("-fvisibility=hidden")

INCLUDE_DIRECTORIES(
    ${INCLUDE_COMMON}
    ${platform_pkgs_nfc_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
${CMAKE_INSTALL_RPATH}
${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    JSNFCManager.cpp
    NFCManager.cpp
    JSNdefMessage.cpp
    NdefMessage.cpp
    JSNdefRecord.cpp
    NdefRecord.cpp
    JSNdefRecordText.cpp
    NdefRecordText.cpp
    JSNdefRecordURI.cpp
    NdefRecordURI.cpp
    JSNdefRecordMedia.cpp
    NdefRecordMedia.cpp
    JSNFCAdapter.cpp
    NFCAdapter.cpp
    JSNFCTag.cpp
    NFCTag.cpp
    JSNFCPeer.cpp
    NFCPeer.cpp
    NFCUtil.cpp
    NFCCallbackData.cpp
    NFCPeerDetectCallback.cpp
    NFCTagDetectCallback.cpp
    JSNdefRecordVector.cpp
    plugin_config.cpp
    JSHCEEventData.cpp
    JSAIDData.cpp
    HCEEventData.cpp
    AIDData.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
    ${platform_pkgs_nfc_LIBRARIES}
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
ENDIF(ENABLE_OPTIONAL_NFC)
INSTALL(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/nfc
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
