SET(TARGET_NAME ${bluetooth_target})
SET(DESTINATION_NAME ${bluetooth_dest})
SET(TARGET_IMPL_NAME ${bluetooth_impl})

IF(ENABLE_OPTIONAL_BT)
PKG_CHECK_MODULES(platform_pkgs_bluetooth REQUIRED
    capi-appfw-application
    capi-network-bluetooth
    capi-system-info
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_COMMON}
    ${TOP}/Bluetooth
    ${platform_pkgs_bluetooth_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
    ${CMAKE_INSTALL_RPATH}
    ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
    JSBluetoothManager.cpp
    JSBluetoothAdapter.cpp
    JSBluetoothDevice.cpp
    JSBluetoothSocket.cpp
    JSBluetoothClass.cpp
    JSBluetoothClassDeviceMajor.cpp
    JSBluetoothClassDeviceMinor.cpp
    JSBluetoothClassDeviceService.cpp
    JSBluetoothGATTService.cpp
    JSBluetoothServiceHandler.cpp
    JSBluetoothHealthApplication.cpp
    JSBluetoothHealthChannel.cpp
    JSBluetoothProfileHandler.cpp
    JSBluetoothHealthProfileHandler.cpp
    BluetoothAdapter.cpp
    BluetoothDevice.cpp
    BluetoothSocket.cpp
    BluetoothClass.cpp
    BluetoothClassDeviceMajor.cpp
    BluetoothClassDeviceMinor.cpp
    BluetoothClassDeviceService.cpp
    BluetoothServiceHandler.cpp
    BluetoothCallbackUtil.cpp
    BluetoothCallback.cpp
    BluetoothGattClientWrapper.cpp
    BluetoothGATTService.cpp
    BluetoothHealthApplication.cpp
    BluetoothHealthChannel.cpp
    BluetoothHealthProfileHandlerCallback.cpp
    BluetoothHealthProfileHandler.cpp
    BluetoothUtil.cpp
    JSBluetoothLowEnergyAdapter.cpp
    BluetoothLowEnergyAdapter.cpp
    JSBluetoothLEDevice.cpp
    BluetoothLEDevice.cpp
    BluetoothLEScanCallback.cpp
    JSBluetoothGATTDescriptor.cpp
    BluetoothGATTDescriptor.cpp
    BluetoothConverter.cpp
    JSBluetoothGATTBase.cpp
    BluetoothGATTBase.cpp
    JSBluetoothGATTCharacteristic.cpp
    BluetoothGATTCharacteristic.cpp
    JSBluetoothLEAdvertiseData.cpp
    BluetoothLEAdvertiseData.cpp
    JSBluetoothLEManufacturerData.cpp
    BluetoothLEManufacturerData.cpp
    JSBluetoothLEServiceData.cpp
    BluetoothLEServiceData.cpp
)


ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
    ${LIBS_COMMON}
    ${tizen_impl}
    ${platform_pkgs_bluetooth_LIBRARIES}
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
ENDIF(ENABLE_OPTIONAL_BT)
INSTALL(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/bluetooth
    FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
