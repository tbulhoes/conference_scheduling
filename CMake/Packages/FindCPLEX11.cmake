# - Try to find Cplex 11.0 on Linux (32 and 64 bits)

find_package(Solver REQUIRED)

# Set possible paths to include
set(INCLUDE /usr/include #Default include path on Unix
	    /usr/ilog/cplex110-x86_64/include/ilcplex #Ilog path to include
	    )

# Set possible paths to the callable library
set(LIBRARY /usr/lib64 #Linux 64 bits
	    /usr/lib   #Linux 32 bits
	    /usr/ilog/cplex110-x86_64/lib/x86-64_rhel4.0_3.4/static_pic #Ilog path to Linux 64 bits
	    /usr/ilog/cplex110/lib/x86_rhel4.0_3.4/static_pic           #Ilog path to Linux 32 bits
	    )

# Then we are using FindSolver that will do all the good work with previously infomations
find_solver(CPLEX #Solver name
	    11
	    cplex #Include main file name 
	    cplex #Library name
	    ${CPLEX_FIND_REQUIRED} #Required or not
	    ${INCLUDE} #Paths to include
	    ${LIBRARY} #Paths to libraries
	    )