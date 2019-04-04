################################################################
# This files permits to find Backtrace (libraries to make a 
# trace of a code.
#
# * BACKTRACE_INCLUDE_DIR: backtrace include dir
# * BACKTRACE_SOURCE_DIR : backtrace source dir 
#
################################################################

if ("${BACKTRACE_INCLUDE_DIR}" STREQUAL "" OR "${BACKTRACE_INCLUDE_DIR}" STREQUAL "BACKTRACE_INCLUDE_DIR-NOTFOUND")
  find_path(BACKTRACE_INCLUDE_DIR
    NAMES boost/backtrace.hpp
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/tools/backtrace
    PATHS "/Applications/BaPCodFramework/BaPCod/tools/backtrace" # For Mac OS X
    PATHS "C:/Program Files/BaPCodFramework/BaPCod/tools/backtrace" # For Windows 32 or 64 bits
    PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/tools/backtrace" # For Windwos 64 bits
    PATHS "/opt/BaPCodFramework/BaPCod/tools/backtrace"
    HINTS boost
    PATH_SUFFIXES boost
    DOC "Backtrace include dir")

  # get_filename_component(BACKTRACE_INCLUDE_DIR ${BACKTRACE_INCLUDE_DIR} PATH)
  find_path(BACKTRACE_SOURCE_DIR 
    NAMES backtrace.cpp 
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/tools/backtrace/libs/backtrace/src 
    PATHS "/Applications/BaPCodFramework/BaPCod/tools/backtrace/libs/backtrace/src" 
    PATHS "C:/Program Files/BaPCodFramework/BaPCod/tools/backtrace/libs/backtrace/src"
    PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/tools/backtrace/libs/backtrace/src"
    PATHS "/opt/BaPCodFramework/BaPCod/tools/backtrace"
    DOC "Backtrace src dir")
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(backtrace DEFAULT_MSG
    BACKTRACE_INCLUDE_DIR BACKTRACE_SOURCE_DIR)

  mark_as_advanced(BACKTRACE_SOURCE_DIR BACKTRACE_INCLUDE_DIR)
endif()
