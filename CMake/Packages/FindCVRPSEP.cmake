if ("${CVRPSEP_INCLUDE_DIR}" STREQUAL "" OR "${CVRPSEP_INCLUDE_DIR}" STREQUAL "CVRPSEP_INCLUDE_DIR-NOTFOUND")
  find_path(CVRPSEP_INCLUDE_DIR
    NAMES basegrph.h
    PATHS ${CMAKE_SOURCE_DIR}/Tools/CVRPSEP/include # for dev apps
    PATHS ENV{BAPCOD_ROOT}/tools/CVRPSEP/include) # for users apps

  find_path(CVRPSEP_SOURCE_DIR
    NAMES basegrph.cpp
    PATHS ${CMAKE_SOURCE_DIR}/Tools/CVRPSEP/src # for dev apps
    PATHS $ENV{BAPCOD_ROOT}/tools/CVRPSEP/src) # for users apps

  # include(FindPackageHandleStandardArgs)
  # find_package_handle_standard_args(RheccSep DEFAULT_MSG RHECCSEP_INCLUDE_DIR RHECCSEP_SOURCE_DIR)

  mark_as_advanced(CVRPSEP_SOURCE_DIR CVRPSEP_INCLUDE_DIR)
endif()
