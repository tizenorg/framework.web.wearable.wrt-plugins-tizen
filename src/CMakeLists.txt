# -----------------------------------------------------------------------------
# Common Module
# -----------------------------------------------------------------------------
SET(COMMON_TARGET_NAME       "wrt-plugins-tizen-common")
SET(COMMON_DESTINATION_NAME  "tizen-tizen")

SET(CMAKE_INSTALL_RPATH
  ${CMAKE_INSTALL_RPATH}
  ${CMAKE_INSTALL_PREFIX}/${DESTINATION_LIB_PREFIX}/${COMMON_DESTINATION_NAME}
)

configure_and_install_pkg(wrt-plugins-tizen-common.pc)
add_subdir(Common)

SET(LIBS_COMMON
  ${LIBS_COMMON}
  ${COMMON_TARGET_NAME}
)

IF(ENABLE_TIME_TRACER)
add_subdir(Common/TimeTracer)

SET(LIBS_COMMON
  ${LIBS_COMMON}
  "timetracer"
)

ENDIF(ENABLE_TIME_TRACER)

SET(LIBS_TEST
  ${LIBS_COMMON}
  "standaloneconsole"
)


MESSAGE(Status, "LIBS_COMMON is: ${LIBS_COMMON}")
MESSAGE(Status, "INCLUDE_COMMON is: ${INCLUDE_COMMON}")

# -----------------------------------------------------------------------------
# Plugin Modules
# -----------------------------------------------------------------------------

SET(MANDATORY_PLUGINS
    Alarm
    Application
    Badge
    Content
    Filesystem
    MessagePort
    Package
    Power
    Sound
    Systeminfo
    TimeUtil
    Tizen
)

IF(ENABLE_OPTIONAL_DOWNLOAD)
SET(OPTIONAL_PLUGINS
  ${OPTIONAL_PLUGINS}
  Download
)
ENDIF(ENABLE_OPTIONAL_DOWNLOAD)

IF(ENABLE_OPTIONAL_HAM)
SET(OPTIONAL_PLUGINS
  ${OPTIONAL_PLUGINS}
  HumanActivityMonitor
)
ENDIF(ENABLE_OPTIONAL_HAM)

IF(ENABLE_OPTIONAL_MEDIAKEY)
SET(OPTIONAL_PLUGINS
  ${OPTIONAL_PLUGINS}
  Mediakey
)
ENDIF(ENABLE_OPTIONAL_MEDIAKEY)

IF(ENABLE_OPTIONAL_SENSOR)
SET(OPTIONAL_PLUGINS
  ${OPTIONAL_PLUGINS}
  Sensor
)
ENDIF(ENABLE_OPTIONAL_SENSOR)

IF(ENABLE_OPTIONAL_SYSTEM_SETTING)
SET(OPTIONAL_PLUGINS
  ${OPTIONAL_PLUGINS}
  SystemSetting
)
ENDIF(ENABLE_OPTIONAL_SYSTEM_SETTING)

add_plugin(
  ${MANDATORY_PLUGINS}
  ${OPTIONAL_PLUGINS}
)
