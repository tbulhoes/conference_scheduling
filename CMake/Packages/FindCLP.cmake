# - Try to find Glpk
# - Try to find Gurobi
if("${CLP_INCLUDE_DIR}" STREQUAL "" OR "${CLP_INCLUDE_DIR}" STREQUAL "CLP_INCLUDE_DIR-NOTFOUND")
  include(Solver)
  
  find_path(CLP_INCLUDE_DIR
    NAMES ClpSolve.hpp 
    PATHS "/usr/local/include/coin"
    PATHS "C:/Program Files/CoinCLP/include")
  
  find_library(CLP_LIBRARY
    NAMES Clp
    PATHS "/usr/local/lib/coin"
    PATHS "C:/Program Files/CoinCLP/lib")
  find_library(COIN_LIBRARY
    NAMES CoinUtils
    PATHS "/usr/local/lib/coin"
    PATHS "C:/Program Files/CoinCLP/lib")

  include(FindPackageHandleStandardArgs)
  # handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
  # if all listed variables are TRUE
  find_package_handle_standard_args(Clp DEFAULT_MSG
    CLP_LIBRARY CLP_INCLUDE_DIR COIN_LIBRARY)

  mark_as_advanced(CLP_INCLUDE_DIR CLP_LIBRARY CLP_LIBRARY_DIR COIN_LIBRARY COIN_LIBRARY_DIR)
  
endif()

if (NOT (("${CLP_LIBRARY}" STREQUAL "CLP_LIBRARY-NOTFOUND") AND ("${COIN_LIBRARY}" STREQUAL "COIN_LIBRARY-NOTFOUND")))
  find_package(LAPACK REQUIRED)
  get_filename_component(CLP_LIBRARY_DIR "${CLP_LIBRARY}" PATH)
  get_filename_component(COIN_LIBRARY_DIR "${COIN_LIBRARY}" PATH)

  append_solver_libraries(${CLP_LIBRARY} ${COIN_LIBRARY} ${LAPACK_LIBRARIES})
  append_solver_library_dirs(${CLP_LIBRARY_DIR} ${COIN_LIBRARY_DIR})
  append_solver_definitions("-D_CLP_FOUND")
endif()
                                        
if(NOT "${CLP_INCLUDE_DIR}" STREQUAL "CLP_INCLUDE_DIR-NOTFOUND")
    append_solver_include_dirs(${CLP_INCLUDE_DIR})
endif()                                   

