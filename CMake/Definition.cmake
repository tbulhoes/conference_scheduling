# - This file configure the definitions for all the project

# Set the default Timer option
set(TIMER "ON"
    CACHE BOOL "Activate/Desactivate timers used by Bapcod")

# Activate/Desactivate Timer Option
if(TIMER)
	set(TIMER_DEFINITION "TIMER")
else(TIMER)
	set(TIMER_DEFINITION "")
endif()

# Default definitions flags for the compilation
set(OPTIONAL_DEFINITIONS
    -D__ANSIC 
    #-D__cplusplus
    CACHE STRING "Optional definitions flags for compiling")

# All definitions flags needed in order to build Bapcod's module or Bapcod's library
set(DEFINITIONS 
    "${SOLVER_DEFINITION}" 
    "${OPTIONAL_DEFINITIONS}")

# Define some specific defintion for macOS
if(APPLE)
	set(CMAKE_EXE_LINKER_FLAGS "-framework IOKit -framework CoreFoundation -Wl,-no_compact_unwind" 
	    CACHE STRING " Flags used by the linker." FORCE)
endif()
