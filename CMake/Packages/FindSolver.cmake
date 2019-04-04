# - Try to find a solver

function(find_solver SOLVER VERSION INCLUDE_SOLVER LIBRARY_SOLVER REQUIRED) 
	# Clear cache allows to rewrite variables
	unset(${SOLVER}_LIBRARY CACHE)
	unset(${SOLVER}_INCLUDE_DIR CACHE)
	
	# Create an internal CMake identifier for the solver in cache
	set(SOLVER_ID "${SOLVER}_${VERSION}")

	# Search the path to the include dir
	find_path(${SOLVER_ID}_INCLUDE_DIR ${INCLUDE_SOLVER}.h ${ARGN})

	# Search the path to the library filename
	find_library(${SOLVER_ID}_LIBRARY NAMES ${LIBRARY_SOLVER} PATHS ${ARGN})

	# If both previous paths are found, the solver is found
	if (${SOLVER_ID}_INCLUDE_DIR AND ${SOLVER_ID}_LIBRARY)
   		set(${SOLVER_ID}_FOUND ON)
        add_definitions("-D_${SOLVER}_FOUND")
        include_directories("${${SOLVER_ID}_INCLUDE_DIR}")
        set(SOLVERS_LIBRARIES "${SOLVERS_LIBRARIES};${${SOLVER_ID}_LIBRARY}" CACHE INTERNAL "")
    endif (${SOLVER_ID}_INCLUDE_DIR AND ${SOLVER_ID}_LIBRARY)
	
	# Then if mode quiet is not activated we print a message found or not found
	if (${SOLVER_ID}_FOUND)
   		if (NOT ${SOLVER}_FIND_QUIETLY)
      			message(STATUS "Find solver: ${SOLVER} ${VERSION}")
   		endif (NOT ${SOLVER}_FIND_QUIETLY)
	else (${SOLVER_ID}_FOUND)
   		if (${SOLVER}_FIND_REQUIRED)
      			message(FATAL_ERROR "Could NOT find required solver: ${SOLVER} ${VERSION}")
   		endif (${SOLVER}_FIND_REQUIRED)
	endif (${SOLVER_ID}_FOUND)
endfunction(find_solver)
