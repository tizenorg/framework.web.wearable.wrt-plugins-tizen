SET(TARGET_NAME ${refimpl_target}_test)

include_directories(
  ${TOP}/Common
  ${TOP}/RefImpl  
  ${TOP}/Common/StandaloneConsole
)

set(CMAKE_INSTALL_RPATH
  ${CMAKE_INSTALL_RPATH}
  ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${tizen_dest}
  ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${refimpl_dest}
)

set(SRCS
    main.cpp
)

ADD_EXECUTABLE(${TARGET_NAME} ${SRCS})

target_link_libraries(${TARGET_NAME}
  ${refimpl_impl}
  ${LIBS_COMMON}
  ${LIBS_TEST}
)

