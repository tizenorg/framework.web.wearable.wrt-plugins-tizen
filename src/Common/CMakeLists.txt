SET(SRCS
  AsyncCallbackManager.cpp
  CallbackUserData.cpp
  GlobalContextManager.cpp
  ArgumentValidator.cpp
  JSUtil.cpp
  PlatformException.cpp
  MultiCallbackUserData.cpp
  JSStringRefWrapper.cpp
  JSWebAPIErrorFactory.cpp
  JSWebAPIError.cpp
  JSWebAPIException.cpp
  WebAPIError.cpp
  PropertyBag.cpp
  JSArray.cpp
  SecurityAccessor.cpp
  JSTizenSecurityObject.cpp
  WebKitProxy.cpp
  DBusOperation.cpp
)

ADD_LIBRARY(${COMMON_TARGET_NAME} SHARED ${SRCS})

TARGET_LINK_LIBRARIES(${COMMON_TARGET_NAME}
	${LIBS_COMMON}
)

INSTALL(TARGETS ${COMMON_TARGET_NAME} LIBRARY DESTINATION ${DESTINATION_LIB_PREFIX}/${COMMON_DESTINATION_NAME})
INSTALL(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DESTINATION_HEADER_PREFIX}/common
	FILES_MATCHING PATTERN "*.h" PATTERN "CMakeFiles" EXCLUDE
)
