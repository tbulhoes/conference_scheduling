#####################################################################
# This file permits to find Bapcod. The following list describes all
# the variables definied here:
# 
# * BAPCOD_INCLUDE_DIRS: all bapcod include directories (include, include_dev or
#                        include_utils)
# * BAPCOD_INCLUDE_DIR : include
# * BAPCOD_INCLUDE_DEV_DIR : include_dev
# * BAPCOD_INCLUDE_UTIL_DIR: include_utils
# * BAPCOD_SOURCE_DIR: src 
# * BAPCOD_LIBRARY: bapcod library
#
#####################################################################

if ("${BAPCOD_INCLUDE_DIRS}" STREQUAL "" OR "${BAPCOD_INCLUDE_DIRS}" STREQUAL "BAPCOD_INCLUDE_DIRS-NOTFOUND") 
  
  find_path(BAPCOD_INCLUDE_DIR
      NAMES bcModelVarC.hpp
      PATHS "${CMAKE_CURRENT_SOURCE_DIR}/include"
      PATHS "C:/Program Files/BaPCodFramework/BaPCod/include"
      PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/include"
      PATHS "/Applications/BaPCodFramework/BaPCod/include"
      PATHS "/opt/BaPCodFramework/2.0.0/BaPCod/include"
      PATHS "/opt/BaPCodFramework/BaPCod/include")

  find_path(BAPCOD_INCLUDE_DEV_DIR
      NAMES bcModelC.hpp
      PATHS "${CMAKE_CURRENT_SOURCE_DIR}/include_dev"
      PATHS "C:/Program Files/BaPCodFramework/BaPCod/include_dev"
      PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/include_dev"
      PATHS "/Applications/BaPCodFramework/BaPCod/include_dev"
#PATHS "/opt/BaPCodFramework/2.0.0/BaPCod/include_dev"
      PATHS "~/shared_test_niteroi_fork-dev_rcsp/include_dev"
      PATHS "/opt/BaPCodFramework/BaPCod/include_dev")
      
  find_path(BAPCOD_INCLUDE_UTIL_DIR
      NAMES bcDeveloperUtilities.hpp
      PATHS "${CMAKE_CURRENT_SOURCE_DIR}/include_utils"
      PATHS "C:/Program Files/BaPCodFramework/BaPCod/include_utils"
      PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/include_utils"
      PATHS "/Applications/BaPCodFramework/BaPCod/include_utils"
      PATHS "/opt/BaPCodFramework/2.0.0/BaPCod/include_utils"    
      PATHS "/opt/BaPCodFramework/BaPCod/include_utils")    
 
  find_path(BAPCOD_INCLUDE_SOLVER_DIR
      bcMathProgSolverInterfaceC.hpp 
      PATHS "${CMAKE_CURRENT_SOURCE_DIR}/include_solvers"
      PATHS "C:/Program Files/BaPCodFramework/BaPCod/include_solvers"
      PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/include_solvers"
      PATHS "/Applications/BaPCodFramework/BaPCod/include_solvers" 
      PATHS "/opt/BaPCodFramework/2.0.0/BaPCod/include_solverInterfaces" 
      PATHS "/opt/BaPCodFramework/BaPCod/include_solvers") 
      
  find_path(BAPCOD_VERSION_DIR 
	  NAMES bcBapcodVersion.h.in 
	  NAMES bcBapcodVersion.h 
	  PATHS "${CMAKE_CURRENT_BINARY_DIR}" 
	  PATHS "${CMAKE_CURRENT_SOURCE_DIR}"
	  PATHS "C:/Program Files/BaPCodFramework/BaPCod"
	  PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod"
	  PATHS "/Applications/BaPCodFramework/BaPCod" 
      PATHS "/opt/BaPCodFramework/BaPCod")

  find_path(BAPCOD_SOURCE_DIR
    NAMES bcModelC.cpp
    PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PATHS "C:/Program Files/BaPCodFramework/BaPCod/src"
    PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/src"
    PATHS "/Applications/BaPCodFramework/BaPCod/src" 
    PATHS "/opt/BaPCodFramework/BaPCod/src"
    DOC "BaPCod source directories")

  if (WIN32)
      if(MSVC10)
          find_library(BAPCOD_LIBRARY_DEBUG 
            NAMES bapcod_vc100_debug
            PATHS "C:/Program Files/BaPCodFramework/BaPCod/lib"
            PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/lib"
            DOC "Debug BaPCod library")
            
          find_library(BAPCOD_LIBRARY_RELEASE 
            NAMES bapcod_vc100_release
            PATHS "C:/Program Files/BaPCodFramework/BaPCod/lib"
            PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/lib"
            DOC "Release BaPCod library")
      elseif(MSVC11)
          find_library(BAPCOD_LIBRARY_DEBUG 
            NAMES bapcod_vc110_debug
            PATHS "C:/Program Files/BaPCodFramework/BaPCod/lib"
            PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/lib"
            DOC "Debug BaPCod library")
            
          find_library(BAPCOD_LIBRARY_RELEASE 
            NAMES bapcod_vc110_release
            PATHS "C:/Program Files/BaPCodFramework/BaPCod/lib"
            PATHS "C:/Program Files (x86)/BaPCodFramework/BaPCod/lib"
            DOC "Release BaPCod library")
      else()
          message(FATAL_ERROR "BaPCod is only build for Visual Studio 2010 or Visual Studio 2012")
      endif()
      
  else() # Unix or Apple system      
      find_library(BAPCOD_LIBRARY_DEBUG 
        NAMES bapcod_debug
        PATHS "/Applications/BaPCodFramework/BaPCod/lib" 
        PATHS "/opt/BaPCodFramework/BaPCod/lib"
        DOC "Debug BaPCod library")
        
      find_library(BAPCOD_LIBRARY_RELEASE 
        NAMES bapcod_release
        PATHS "/Applications/BaPCodFramework/BaPCod/lib" 
        PATHS "/opt/BaPCodFramework/BaPCod/lib"
        DOC "Release BaPCod library")
        
      if ("${BAPCOD_LIBRARY_DEBUG}" STREQUAL "BAPCOD_LIBRARY_DEBUG-NOTFOUND")
          set(BAPCOD_LIBRARY_DEBUG ${BAPCOD_LIBRARY_RELEASE})
      endif()
  endif()

  
  set(BAPCOD_LIBRARY debug ${BAPCOD_LIBRARY_DEBUG}
                     optimized ${BAPCOD_LIBRARY_RELEASE})
    
  set(BAPCOD_INCLUDE_DIRS ${BAPCOD_INCLUDE_DIR} ${BAPCOD_VERSION_DIR} ${BAPCOD_INCLUDE_SOLVER_DIR})
  
  if (NOT "${BAPCOD_INCLUDE_DEV_DIR}" STREQUAL "BAPCOD_INCLUDE_DEV_DIR-NOTFOUND")
    list(APPEND BAPCOD_INCLUDE_DIRS ${BAPCOD_INCLUDE_DEV_DIR})
  endif()
  
  if (NOT "${BAPCOD_INCLUDE_UTIL_DIR}" STREQUAL "BAPCOD_INCLUDE_UTIL_DIR-NOTFOUND")
    list(APPEND BAPCOD_INCLUDE_DIRS ${BAPCOD_INCLUDE_UTIL_DIR})
  endif()
  
  if (NOT "${BAPCOD_VERSION_DIR}" STREQUAL "BAPCOD_VERSION_DIR-NOTFOUND")
    list(APPEND BAPCOD_INCLUDE_DIRS ${BAPCOD_VERSION_DIR})
  endif()
  
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(bapcod DEFAULT_MSG BAPCOD_INCLUDE_DIR)
  
  mark_as_advanced(BAPCOD_SOURCE_DIR                   
                   BAPCOD_LIBRARY_DEBUG
                   BAPCOD_LIBRARY_RELEASE
                   BAPCOD_INCLUDE_DEV_DIR
                   BAPCOD_INCLUDE_UTIL_DIR
                   BAPCOD_INCLUDE_SOLVER_DIR
                   BAPCOD_INCLUDE_DIRS
                   BAPCOD_VERSION_DIR)

endif()
