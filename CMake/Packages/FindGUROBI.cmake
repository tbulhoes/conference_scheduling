# - Try to find Gurobi
if("${GUROBI_INCLUDE_DIR}" STREQUAL "" OR "${GUROBI_INCLUDE_DIR}" STREQUAL "GUROBI_INCLUDE_DIR-NOTFOUND")
  include(Solver)

  if (WIN32)
    if (CMAKE_CL_64)
      find_path(GUROBI_INCLUDE_DIR gurobi_c.h PATHS "C:/gurobi550/win64/include")
    else()
      find_path(GUROBI_INCLUDE_DIR gurobi_c.h PATHS "C:/gurobi550/win32/include")
    endif()
  else()
    find_path(GUROBI_INCLUDE_DIR gurobi_c.h PATHS "/Library/gurobi550/mac64/include"
      "/opt/gurobi550/linux64/include")
  endif()
                                              
  if (WIN32)
    if (CMAKE_CL_64)
      find_library(GUROBI_LIBRARY gurobi55 PATHS "C:/gurobi550/win64/lib")
    else()
      find_library(GUROBI_LIBRARY gurobi55 PATHS "C:/gurobi550/win32/lib")
    endif()
  else()
    find_library(GUROBI_LIBRARY gurobi55 PATHS "/Library/gurobi550/mac64/include"
      "/opt/gurobi550/linux64/include")
  endif()

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Gurobi DEFAULT_MSG
    GUROBI_LIBRARY GUROBI_INCLUDE_DIR)

  mark_as_advanced(GUROBI_LIBRARY GUROBI_INCLUDE_DIR GUROBI_LIBRARY_DIR)

endif()

if (NOT ("${GUROBI_LIBRARY}" STREQUAL "GUROBI_LIBRARY-NOTFOUND"))
  get_filename_component(GUROBI_LIBRARY_DIR "${GUROBI_LIBRARY}" PATH)

  append_solver_libraries(${GUROBI_LIBRARY})
  append_solver_library_dirs(${GUROBI_LIBRARY_DIR})
  append_solver_definitions("-D_GUROBI_FOUND")
endif()
                                        
if(NOT "${GUROBI_INCLUDE_DIR}" STREQUAL "GUROBI_INCLUDE_DIR-NOTFOUND")
    append_solver_include_dirs(${GUROBI_INCLUDE_DIR})
endif()                                   
    



