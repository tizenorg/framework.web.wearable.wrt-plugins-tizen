SET(TARGET_NAME ${application_target})
SET(DESTINATION_NAME ${application_dest})
SET(TARGET_IMPL_NAME ${application_impl})

PKG_CHECK_MODULES(platform_pkgs_application REQUIRED
	capi-appfw-app-manager
	capi-appfw-application
	capi-appfw-package-manager
	pkgmgr
	pkgmgr-info
	wrt-plugins-ipc-message
)

ADD_DEFINITIONS("-fvisibility=hidden")

INCLUDE_DIRECTORIES(
	${INCLUDE_COMMON}
	${TOP}/Application
	${platform_pkgs_application_INCLUDE_DIRS}
)

SET(CMAKE_INSTALL_RPATH
	${CMAKE_INSTALL_RPATH}
	${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${tizen_dest}
	${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${DESTINATION_NAME}
)

SET(SRCS_IMPL
	ApplicationContext.cpp
	ApplicationControl.cpp
	ApplicationControlData.cpp
	RequestedApplicationControl.cpp
	ApplicationFactory.cpp
	ApplicationInformation.cpp
	ApplicationCert.cpp
	ApplicationMetaData.cpp
	IApplicationManager.cpp
	ApplicationManager.cpp
	AppManagerWrapper.cpp
	Application.cpp
	ApplicationAsyncCallbackManager.cpp
	ApplicationController.cpp
	ApplicationConverter.cpp
	ApplicationListenerManager.cpp
	ApplicationUtil.cpp
	JSApplication.cpp
	JSApplicationManager.cpp
	JSApplicationContext.cpp
	JSApplicationControl.cpp
	JSApplicationControlData.cpp
	JSRequestedApplicationControl.cpp
	JSApplicationEventCallbackManager.cpp
	JSApplicationInformation.cpp
	JSApplicationCert.cpp
	JSApplicationMetaData.cpp
)

ADD_LIBRARY(${TARGET_IMPL_NAME} SHARED ${SRCS_IMPL})

TARGET_LINK_LIBRARIES(${TARGET_IMPL_NAME}
	${LIBS_COMMON}
	${tizen_impl}
	${platform_pkgs_application_LIBRARIES}
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
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/application
	FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
