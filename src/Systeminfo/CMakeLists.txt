SET(TARGET_NAME ${systeminfo_target})
SET(DESTINATION_NAME ${systeminfo_dest})
SET(TARGET_IMPL_NAME ${systeminfo_impl})

SET(PLATFORM_PKGS_FOR_SYSTEMINFO
    capi-system-info
    capi-system-runtime-info
    capi-network-connection
    capi-system-device
    capi-system-system-settings
    sensor
)

IF(ENABLE_OPTIONAL_BT)
SET(PLATFORM_PKGS_FOR_SYSTEMINFO
    ${PLATFORM_PKGS_FOR_SYSTEMINFO}
    capi-network-bluetooth
)
ENDIF(ENABLE_OPTIONAL_BT)

IF(ENABLE_OPTIONAL_WI_FI)
SET(PLATFORM_PKGS_FOR_SYSTEMINFO
    ${PLATFORM_PKGS_FOR_SYSTEMINFO}
    capi-network-wifi
)
ENDIF(ENABLE_OPTIONAL_WI_FI)

IF(ENABLE_OPTIONAL_TELEPHONY)
SET(PLATFORM_PKGS_FOR_SYSTEMINFO
    ${PLATFORM_PKGS_FOR_SYSTEMINFO}
    tapi
)
ENDIF(ENABLE_OPTIONAL_TELEPHONY)

PKG_CHECK_MODULES(platform_pkgs_systeminfo REQUIRED
    ${PLATFORM_PKGS_FOR_SYSTEMINFO}
)

ADD_DEFINITIONS("-fvisibility=hidden")

INCLUDE_DIRECTORIES(
    ${INCLUDE_COMMON}
    ${platform_pkgs_systeminfo_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    JSSystemInfoDeviceCapability.cpp
    SystemInfoDeviceCapability.cpp
    JSSystemInfoProperty.cpp
    SystemInfoProperty.cpp
    JSSystemInfoBattery.cpp
    SystemInfoBattery.cpp
    JSSystemInfoLocale.cpp
    SystemInfoLocale.cpp
    JSSystemInfoStorage.cpp
    SystemInfoStorage.cpp
    JSSystemInfoStorageUnit.cpp
    SystemInfoStorageUnit.cpp
    JSSystemInfoDisplay.cpp
    SystemInfoDisplay.cpp
    JSSystemInfoDeviceOrientation.cpp
    SystemInfoDeviceOrientation.cpp
    JSSystemInfo.cpp
    SystemInfo.cpp
    JSSystemInfoCpu.cpp
    SystemInfoCpu.cpp
    JSSystemInfoBuild.cpp
    SystemInfoBuild.cpp
    JSSystemInfoPeripheral.cpp
    SystemInfoPeripheral.cpp
    SystemInfoPropertySuccessCallbackData.cpp
    JSSystemInfoNetwork.cpp
    SystemInfoNetwork.cpp
    JSSystemInfoMemory.cpp
    SystemInfoMemory.cpp
    SystemInfoUtil.cpp
)

IF(ENABLE_OPTIONAL_WI_FI)
SET(SRCS_IMPL
    ${SRCS_IMPL}
    JSSystemInfoWifiNetwork.cpp
    SystemInfoWifiNetwork.cpp
)
ENDIF(ENABLE_OPTIONAL_WI_FI)

IF(ENABLE_OPTIONAL_TELEPHONY)
SET(SRCS_IMPL
    ${SRCS_IMPL}
    JSSystemInfoCellularNetwork.cpp
    SystemInfoCellularNetwork.cpp
    JSSystemInfoSIM.cpp
    SystemInfoSIM.cpp
)
ENDIF(ENABLE_OPTIONAL_TELEPHONY)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
    ${platform_pkgs_systeminfo_LIBRARIES}
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
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/systeminfo
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
