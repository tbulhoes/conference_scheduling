################################################################
# This files permits to find MathProgSolverBuilder source code
# (this code permits to use different solver with BaPCod)
#
# * MPSB_INCLUDE_DIR: MathProgSolverBuilder include dir
# * MPSB_SOURCE_DIR : MathProgSolverBuilder source dir 
#
################################################################

if ("${MPSB_INCLUDE_DIR}" STREQUAL "" OR "${MPSB_INCLUDE_DIR}" STREQUAL "MPSB_INCLUDE_DIR-NOTFOUND")
  find_path(MPSB_INCLUDE_DIR
    NAMES bcMathProgSolverBuilder.hpp
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/tools/MathProgSolverBuilder/include
    PATHS "/Applications/BaPCodFramework/BaPCod/tools/MathProgSolverBuilder/include" # For Mac OS X
    PATHS "C:/Program Files/BaPCodFramework/BaPCod/tools/MathProgSolverBuilder/include" # For Windows 32 or 64 bits
    PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/tools/MathProgSolverBuilder/include" # For Windwos 64 bits
    PATHS "/opt/BaPCodFramework/BaPCod/tools/MathProgSolverBuilder"
    DOC "MathProgSolverBuilder include dir")

  # get_filename_component(MPSB_INCLUDE_DIR ${MPSB_INCLUDE_DIR} PATH)
  find_path(MPSB_SOURCE_DIR 
    NAMES bcMathProgSolverBuilder.cpp
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/tools/MathProgSolverBuilder/src
    PATHS "/Applications/BaPCodFramework/BaPCod/tools/MathProgSolverBuilder/src" 
    PATHS "C:/Program Files/BaPCodFramework/BaPCod/tools/MathProgSolverBuilder/src"
    PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/tools/MathProgSolverBuilder/src"
    PATHS "/opt/BaPCodFramework/BaPCod/tools/MathProgSolverBuilder/src"
    DOC "MathProgSolverBuilder src dir")

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(MathProgSolverBuilder DEFAULT_MSG
    MPSB_INCLUDE_DIR MPSB_SOURCE_DIR)
  mark_as_advanced(MPSB_SOURCE_DIR MPSB_INCLUDE_DIR)
endif()
