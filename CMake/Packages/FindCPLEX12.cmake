# - Try to find Cplex 12.4/5 on Linux (32/64 bits), Mac (32/64 bits), and Windows (32/64 bits)

if("${CPLEX_INCLUDE_DIR}" STREQUAL "" OR "${CPLEX_INCLUDE_DIR}" STREQUAL "CPLEX_INCLUDE_DIR-NOTFOUND")

  include(Solver)
  
  # Set possible paths to include
  find_path(CPLEX_INCLUDE_DIR
    cplex.h
    PATHS "/opt/ibm/ILOG/CPLEX_Studio124/cplex/include/ilcplex"
    PATHS "/opt/ibm/ILOG/CPLEX_Studio1251/cplex/include/ilcplex"
    PATHS "/opt/ilog/cplex/include/ilcplex"
    PATHS "C:/Program Files*/IBM/ILOG/CPLEX_Studio12*/cplex/include/ilcplex")
  
  if(UNIX)	
    find_library(CPLEX_LIBRARY
      cplex
      PATHS "/opt/ibm/ILOG/CPLEX_Studio124/cplex/lib/x86-64_sles10_4.1/static_pic/"
      PATHS "/opt/ibm/ILOG/CPLEX_Studio1251/cplex/lib/x86-64_sles10_4.1/static_pic/"
      PATHS "/opt/ilog/cplex/lib/*/static_pic/"
      PATHS "/opt/ILOG/CPLEX_Studio12*/cplex/lib/x86-64_darwin9_gcc4.0/static_pic/"
      PATHS "/opt/ILOG/CPLEX_Studio12*/cplex/lib/x86_darwin9_gcc4.0/static_pic/")
  else(UNIX)
    find_library(CPLEX_LIBRARY
      NAMES cplex124 cplex125
      PATHS "C:/Program Files*/IBM/ILOG/CPLEX_Studio12*/cplex/lib/*/stat_mda")
  endif(UNIX)
  
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(CPLEX12 DEFAULT_MSG
    CPLEX_INCLUDE_DIR CPLEX_LIBRARY)  
  
  mark_as_advanced(CPLEX_LIBRARY CPLEX_INCLUDE_DIR CPLEX_LIBRARY_DIRS)

endif()

if(NOT "${CPLEX_LIBRARY}" STREQUAL "CPLEX_LIBRARY-NOTFOUND")
  find_path(CPLEX_LIBRARY_DIRS ${CPLEX_LIBRARY} NAMES libcplex.a cplex124.lib cplex125.lib)
  

     find_library(LIBILO
	   ilocplex
           PATHS "/opt/ibm/ILOG/CPLEX_Studio128/cplex/lib/x86-64_linux/static_pic/"
	   PATHS "/opt/ilog/cplex/lib/x86-64_sles10_4.1/static_pic/")
      find_library(LIBCONCERT
	   concert
           PATHS "/opt/ibm/ILOG/CPLEX_Studio128/concert/lib/x86-64_linux/static_pic/"
	   PATHS "/opt/ilog/cplex/lib/x86-64_sles10_4.1/static_pic/")
      find_library(LIBCLIQUESEP
	   CliqueSep
           PATHS "/home/teobaldo/conference_scheduling/")
      find_library(LIBCVRPSEP
	   CVRPSEP
           PATHS "/home/teobaldo/conference_scheduling/")
      find_library(LIBRHECCSEP
	   RheccSep
           PATHS "/home/teobaldo/conference_scheduling/")
	
    append_solver_libraries(${LIBILO})
    append_solver_libraries(${CPLEX_LIBRARY})
    append_solver_libraries(${LIBCONCERT})
    append_solver_libraries(${LIBCLIQUESEP})
    append_solver_libraries(${LIBCVRPSEP})
    append_solver_libraries(${LIBRHECCSEP})

    append_solver_library_dirs(${CPLEX_LIBRARY_DIRS})
    append_solver_definitions("-D_CPLEX_FOUND")
endif()                                              

if(NOT "${CPLEX_INCLUDE_DIR}" STREQUAL "CPLEX_INCLUDE_DIR-NOTFOUND")
  append_solver_include_dirs(${CPLEX_INCLUDE_DIR})

  find_path(MY_CPLEX_INCLUDE_DIR
    ilcplex/cplex.h
    PATHS "/opt/ibm/ILOG/CPLEX_Studio128/cplex/include"
    PATHS "/opt/ilog/cplex/include")
  find_path(MY_CONCERT_INCLUDE_DIR
    ilconcert/iloenv.h
    PATHS "/opt/ibm/ILOG/CPLEX_Studio128/concert/include"
    PATHS "/opt/ilog/concert/include")
  
  append_solver_include_dirs(${MY_CPLEX_INCLUDE_DIR})
  append_solver_include_dirs(${MY_CONCERT_INCLUDE_DIR})

endif()                                   
